#include "UI_Window.h"
#include <stdio.h>
#include <stdlib.h>

void destroyWindow(Widget* src);
void handleWindowEvent(Widget* src, SDL_Event* event);
int drawWindow(Widget* src, SDL_Renderer* render);


/** Creates a new window widget with the given parameters specified
 *
 * @param title - title of the window
 * @param x,y,w,h - the location of the widget in the screen
 * @param additionalData - additional data the window should have
 *
 * @return
 * NULL ,if an allocation error occured,
 * A new instance of the widget ,otherwise
 * */
Widget* createWindow(char *title, int x, int y, int w, int h, void (*destroyData)(void*))
{
	//create the widget
	Widget* res = createWidget(NULL, (SDL_Rect){.x=x, .y=y, .w=w, .h=h}, drawWindow, handleWindowEvent, destroyWindow, NULL);
	if (res == NULL){
		return NULL;
	}

	Window* data = (Window*) malloc(sizeof(Window));
	if (data == NULL) {
		destroyWindow(res);
		return NULL;
	}

	SDL_Window* window = SDL_CreateWindow(title, x, y, w, h, SDL_WINDOW_OPENGL|SDL_WINDOW_HIDDEN);

	// make sure window was created successfully
	if (window == NULL ) {
		destroyWindow(res);
		return NULL;
	}

	//create renderer for the window
	SDL_Renderer* rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (rend == NULL) {
		destroyWindow(res);
		return NULL;
	}
	// ensure renderer supports transparency
	if (SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND) == -1){
		destroyWindow(res);
		return NULL;;
	}


	// store window details
	data->rend = rend;
	data->window = window;
	data->show = showWindow;
	data->hide = hideWindow;
	data->hidden = true;
	data->root = createPanel(res, 0,0,w,h);
	if (data->root == NULL){
		destroyWindow(res);
		return NULL;;
	}
	data->root->parent = data->root;

	data->data = NULL;
	data->destroyData = destroyData;

	//set the widget data
	res -> data = data;
	return res;
}

/** Destroys the given window widget src
 *  in case src == NULL, does nothing
 *
 *  @param src - the window widget src
 *
 * */
void destroyWindow (Widget* src)
{
	if(src == NULL){
		return;
	}

	Window* window = (Window*) src->data;
	if (window == NULL){
		free(src);
		return;
	}

	if (window->destroyData != NULL){
		window->destroyData(window->data);
	}

	SDL_Window *sdlWindow = window->window;
	if (sdlWindow == NULL){
		free(src);
		free(window);
		return;
	}



	SDL_Renderer *rend = window->rend;
	if (rend == NULL){
		free(src);
		free(window);
		SDL_DestroyWindow(sdlWindow);
		return;
	}


	destroyWidget(window->root);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(sdlWindow);

	free(window);
	free(src);
}

/** Handles the specified event
 * by sending it  to all the widgets within the window
 *
 * @param src - the window widget src
 * @param event - the event to be handled
 *
 * */
void handleWindowEvent(Widget* src, SDL_Event* e) {
	Window *window = src->data;
	if(window->hidden){
		return;
	}
	window->root -> handleEvent(window->root, e);
}

/** Draws all the widgets contained in the window
 *
 * @param src - the window widget src
 * @param render - the renderer
 *
 * @return
 * 0, on success
 * -1, on error
 *
 * */
int drawWindow(Widget* src, SDL_Renderer* render) {
	Window *window = src->data;
	if(window->hidden){
		return 0;
	}
	SDL_SetRenderDrawColor(render, 251, 228, 213, 255);
	SDL_RenderClear(render);
	return window->root->draw(window->root, render);
}

/** Shows the specified window
 *
 * @param src - the window widget src
 *
 * */
void showWindow(Widget* src) {
	Window* window = (Window*) src->data;
	window->hidden = false;
	//window->root->draw(window->root, window->rend);
	SDL_ShowWindow(window->window);
}

/** Hides the specified window
 *
 * @param src - the window widget src
 *
 * */
void hideWindow(Widget* src) {
	Window* window = (Window*) src->data;
	window->hidden = true;
	SDL_HideWindow(window->window);
}

/** Adds the specified widget to window
 * so it will be the first one to be drawn
 *
 * @param src - the window widget src
 * @param widget - the widget should be added to the window
 *
 * */
void addFirstToWindow(Widget *src, Widget *widget){
	Window* window = (Window*) src->data;
	addFirstToPanel(window->root, widget);
}

/** Adds the specified widget to window
 * so it will be the last one to be drawn
 *
 * @param src - the window widget src
 * @param widget - the widget should be added to the window
 *
 * */
void addLastToWindow(Widget *src, Widget *widget){
	Window* window = (Window*) src->data;
	addLastToPanel(window->root, widget);
}
