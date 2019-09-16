#include "UI_Label.h"
#include <stdio.h>
#include <stdlib.h>

void destroyLabel(Widget* src);
void handleLabelEvent(Widget* src, SDL_Event* event);
int drawLabel(Widget* src, SDL_Renderer* renderer);


/** Creates a new label widget
 *
 * @param parentWindow - the window contains the widget
 * @param renderer - the renderer
 * @param image - a path to bmp image
 * @param location - the location of the widget in the screen
 *
 * @return
 * NULL ,if an allocation error occured,
 * A new instance of the widget ,otherwise
 * */
Widget* createLabel(
		Widget *parentWindow,
		SDL_Renderer* renderer,
		const char* image,
		SDL_Rect location)
{
	//create the widget
	Widget *res = createWidget(parentWindow, location, drawLabel, handleLabelEvent, destroyLabel, NULL);
	if (res == NULL){
		destroyLabel(res);
		return NULL;
	}

	Label* data = (Label*) malloc(sizeof(Label));
	if (data == NULL) {
		destroyLabel(res);
		return NULL;
	}

	SDL_Texture *texture = getTexture(renderer, image);
	if (texture == NULL){
		destroyLabel(res);
		return NULL;
	}

	// store label details
	data->texture = texture;
	data->render = renderer;
	data->hidden = false;

	res->data = data;

	return res;
}

/** Destroys the given label widget src
 *  in case src == NULL, does nothing
 *
 *  @param src- the widget src
 *
 * */
void destroyLabel(Widget* src)
{
	if (src == NULL){
		return;
	}
	Label* label = (Label*) src->data;
	if (label == NULL){
		free(src);
		return;
	}
	SDL_Texture *texture = label->texture;
	if (texture == NULL){
		free(label);
		free(src);
		return;
	}
	SDL_DestroyTexture(label->texture);
	free(label);
	free(src);
}

/** Handles the specified event
 *
 * @param src - the label widget src
 * @param event the event to be handled
 *
 * */
void handleLabelEvent(Widget* src, SDL_Event* e)
{
	if (!src || !e){
		return;
	}
}

/** Draws the label to the screen
 *
 * @param src - the label widget src
 * @param renderer - the renderer
 *
 * @return
 * 0, on success
 * -1, on error*/
int drawLabel(Widget* src, SDL_Renderer* render)
{
	Label* label = (Label*) src->data;
	if (label->hidden){
		return 0;
	}
	SDL_Texture *texture = label->texture;

	int textureWidth, textureHeight;
	SDL_QueryTexture(texture, NULL, NULL, &textureWidth, &textureHeight);

	updateLocation(src);
	updateSrcRect(src, textureWidth, textureHeight);

	return SDL_RenderCopy(render, texture, &src->srcRect, &src->location);
}

/**Hides/shows the specified label
 *
 * @param src - the label widget src
 * @param hide - whether to hide/show the label
 *
 * */
void hideLabel (Widget *src, bool hide){
	Label *lbl = (Label*)src->data;
	lbl->hidden = hide;
}
