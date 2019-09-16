#include "UI_vScrollbar.h"
#include "UI_Button.h"
#include <stdio.h>


void handleScrollBarEvent(Widget* src, SDL_Event* event);
int drawScrollBar(Widget* src, SDL_Renderer* render);
void destroyScrollBar(Widget* src);
void dummyFunc (Widget* src, SDL_Event* event);


/**dummy function*/
void dummyFunc (Widget* src, SDL_Event* event){
	if (!src || !event){
		return;
	}

}

/** Creates a new vertical scrollbar widget,
 * in which the specified panel can be scrolled vertically
 *
 * @param parentWindow - the window contains the widget
 * @param loction - the location of the widget in the screen
 * @param panel - the panel
 *
 * @return
 * NULL ,if an allocation error occured,
 * A new instance of the widget ,otherwise
 * */
Widget* createScrollBar(Widget *parentWindow, SDL_Rect location, SDL_Renderer* renderer, Widget *panel){

	//create the widget
	Widget *res = createWidget(parentWindow, location, drawScrollBar, handleScrollBarEvent, destroyScrollBar, NULL);
	if (res == NULL){
		destroyScrollBar(res);
		return NULL;
	}
	VScrollbar *data = (VScrollbar*)malloc(sizeof(VScrollbar));
	if (data == NULL){
		destroyScrollBar(res);
		return NULL;
	}

	data->panel = panel;
	data->scrolled = 0;

	panel->parent = res;
	updateLocation(panel);

	Widget *up = createButton(parentWindow, renderer, "images/upArrow.bmp", "images/upArrow_disabled.bmp", NULL, (SDL_Rect){location.x, location.y, 30, 30}, dummyFunc, 0);
	if (up == NULL){
		destroyScrollBar(res);
		return NULL;
	}
	up->parent = res;
	data->upButton = up;

	Widget *down = createButton(parentWindow, renderer, "images/downArrow.bmp", "images/downArrow_disabled.bmp", NULL, (SDL_Rect){location.x, location.y+location.h-35, 30, 30}, dummyFunc, 0);
	if (down == NULL){
		destroyScrollBar(res);
		return NULL;
	}
	down->parent = res;
	data->downButton = down;

	if (panel->originalLocation.y == panel->location.y){ //disable up scrolling
		disableButton(up, true);
	}

	if (panel->originalLocation.h == panel->location.h){ //disable down scrolling
		disableButton(down, true);
	}

	res->data = data;

	return res;
}

/** Destroys the given scrollbar widget src
 *  in case src == NULL, does nothing
 *
 *  @param src- the widget src
 *
 * */
void destroyScrollBar(Widget* src){
	if (src == NULL){
		return;
	}
	VScrollbar *data = (VScrollbar*)src->data;
	if (data == NULL){
		free(src);
		return;
	}
	destroyWidget(data->panel);
	destroyWidget(data->upButton);
	destroyWidget(data->downButton);
	free(data);
	free(src);

}

/** If the specified event is a scroll event,
 * updates the location of widgets inside the panel according to the amount scrolled.
 * Also, sends the event to all the widgets in the panel
 *
 * @param src - the vScrollbar widget
 * @param event - the event to be handled
 *
 * */
void handleScrollBarEvent(Widget* src, SDL_Event* event){
	VScrollbar *data = (VScrollbar*)src->data;
	Widget *panel = data->panel;
	Panel *p = (Panel*)panel->data;


	int diff = 0;
	Widget* up = data->upButton;
	Widget* down = data->downButton;

	bool upDisabled = ((Button*)up->data)->disabled;
	bool downDisabled = ((Button*)down->data)->disabled;


	if(event->type == SDL_MOUSEWHEEL) //mouse wheel
	{
		diff = event->wheel.y;
	}

	else if (event->type == SDL_KEYDOWN){ //up\down key
		switch (event->key.keysym.sym) {
		case SDLK_DOWN:
			if (!downDisabled)
				diff = -10;
			break;
		case SDLK_UP:
			if (!upDisabled)
				diff = 10;
			break;
		}
	}
	else { //click on up\down arrow button
		bool inUpRect = isInRect(event->button.x, event->button.y, up->location);
		bool inDownRect = isInRect(event->button.x, event->button.y, down->location);
		if (inUpRect && event->type == SDL_MOUSEBUTTONUP && !upDisabled) {
			diff = 10;
		}
		else if (inDownRect && event->type == SDL_MOUSEBUTTONDOWN && !downDisabled) {
			diff = -10;
		}
	}
	data->scrolled -= diff;
	if (diff != 0){
		if (data->scrolled < panel->originalLocation.y - panel->location.y){ //disable up scrolling
			data->scrolled += diff;
			diff = 0;
			disableButton(up, true);
		}
		else {
			disableButton(up, false);
		}

		if (data->scrolled > panel->originalLocation.h - panel->location.h){ //disable down scrolling
			data->scrolled += diff;
			diff = 0;
			disableButton(down, true);
		}
		else{
			disableButton(down, false);
		}
	}



	//change the locations of the widgets inside the panel according to the amount scrolled
	for (Widget *curr = p->head; curr!=NULL; curr = curr->next){
		curr->originalLocation.y += diff;
		curr->location.y = curr->originalLocation.y;
	}
	panel->handleEvent(panel,event);
}

/** Draws the vertical scroll bar and all the
 * widgets contained within it to the screen
 *
 * @param src - the vertical scroll bar
 * @param render - the renderer
 *
 * @return
 * 0, on success
 * -1, on error
 * */
int drawScrollBar(Widget* src, SDL_Renderer* render){
	VScrollbar *data = (VScrollbar*)src->data;
	data->upButton->draw(data->upButton, render);
	data->downButton->draw(data->downButton, render);
	Widget *panel = data->panel;
	return panel->draw(panel, render);
}

