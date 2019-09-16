#include "UI_Panel.h"
#include <stdlib.h>

void destroyListRec(Widget *node);
void destroyPanel(Widget* src);
void handlePanelEvent(Widget* src, SDL_Event* event);
int drawPanel(Widget* src, SDL_Renderer* render);



/** Creates a new panel widget
 *
 * @param parentWindow - the window contains the widget
 * @param x,y,w,h - the location of the widget in the screen
 *
 * @return
 * NULL ,if an allocation error occured,
 * A new instance of the widget ,otherwise
 * */
Widget* createPanel(Widget *parentWindow, int x, int y, int w, int h){

	//create the widget
	Widget* res = createWidget(parentWindow, (SDL_Rect){x,y,w,h}, drawPanel, handlePanelEvent, destroyPanel, NULL);
	if (res == NULL){
		destroyPanel(res);
		return NULL;
	}

	Panel* data = (Panel*) malloc(sizeof(Panel));
	if (data == NULL) {
		destroyPanel(res);
		return NULL;
	}

	// store panel details
	data->head = NULL;
	data->tail = NULL;

	res->data = data;

	return res;
}

/**Destroys all the widgets in the list recursively*/
void destroyListRec(Widget *node){
	if (node != NULL){
		destroyListRec(node->next);
		destroyWidget(node);
	}
}

/** Destroys the given panel widget src
 *  and all of the widgets within it,
 *  in case src == NULL, does nothing
 *
 *  @param src- the widget src
 *
 * */
void destroyPanel(Widget* src){
	if(src == NULL){
		return;
	}
	Panel *data = src->data;
	if (data == NULL){
		free(src);
		return;
	}
	destroyListRec(data->head);
	free(data);
	free(src);
}

/** Draws all the widgets within the panel
 *
 * @param src - the panel widget src
 * @param render - the renderer
 *
 * @return
 * 0, on success
 * -1, on error*/
int drawPanel(Widget* src, SDL_Renderer* render){
	Panel* panel = (Panel*) src->data;
	Widget *n = panel->head;
	while (n != NULL){
		if (n->draw(n,render) == -1) //error drowing widget
			return -1;
		n = n->next;
	}
	return 0;
}

/** Sends the specified event to all the widgets within the panel
 *
 * @param src - the panel widget src
 * @param event - the event to be handled
 *
 * */
void handlePanelEvent(Widget* src, SDL_Event* event){
	Panel* panel = (Panel*) src->data;
	Widget *n = panel->head;
	while (n!=NULL){
		n->handleEvent(n, event);
		n = n->next;
	}
}

/** Adds the specified widget to the panel
 * so it will be the first one to be drawn
 *
 * @param src - the panel widget src
 * @param widget - the widget should be added to the panel
 *
 * */
void addFirstToPanel(Widget *src, Widget *widget){
	Panel* panel = (Panel*) src->data;
	widget->parent = src;
	widget->next = panel->head;
	panel->head = widget;
	if (panel->tail == NULL){
		panel->tail = widget;
	}
	widget->location.x += src->originalLocation.x;
	widget->location.y += src->originalLocation.y;
	widget->originalLocation.x += src->originalLocation.x;
	widget->originalLocation.y += src->originalLocation.y;
	updateLocation(widget);
}

/** Adds the specified widget to the panel
 * so it will be the last one to be drawn
 *
 * @param src - the panel widget src
 * @param widget - the widget should be added to the panel
 *
 * */
void addLastToPanel(Widget *src, Widget *widget){
	Panel* panel = (Panel*) src->data;
	widget->parent = src;
	if (panel->tail == NULL){
		panel->tail = widget;
		panel->head = widget;
	}
	else {
		panel->tail->next = widget;
		panel->tail = widget;
	}
	widget->location.x += src->originalLocation.x;
	widget->location.y += src->originalLocation.y;
	widget->originalLocation.x += src->originalLocation.x;
	widget->originalLocation.y += src->originalLocation.y;
	updateLocation(widget);

}
