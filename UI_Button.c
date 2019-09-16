#include "UI_Button.h"
#include <stdio.h>
#include <stdlib.h>

void destroyButton(Widget* src);
void handleButtonEvent(Widget* src, SDL_Event* event);
int drawButton(Widget* src, SDL_Renderer* renderer);


/** Creates a new button widget
 *
 * @param parentWindow - the window contains the widget
 * @param renderer - the renderer
 * @param image - a path to .bmp image
 * @param disabledImage - the image the button should have when it's disabled
 * @param selectedImage - the image the button should have when it's selected
 * @param location - the location of the widget in the screen
 * @param action - a pointer to function to execute when the button is clicked
 * @param val - the value of the button
 *
 * @return
 * NULL ,if an allocation error occured,
 * A new instance of the widget ,otherwise
 * */
Widget* createButton(
		Widget *parentWindow,
		SDL_Renderer* renderer,
		const char* image,
		const char* disabledImage,
		const char* selectedImage,
		SDL_Rect location,
		void (*action)(Widget*, SDL_Event*),
		char* val)
{
	//create the widget
	Widget *res = createWidget(parentWindow, location, drawButton, handleButtonEvent, destroyButton, NULL);
	if (res == NULL){
		destroyButton(res);
		return NULL;
	}
	Button* data = (Button*) malloc(sizeof(Button));
	if (data == NULL) {
		destroyButton(res);
		return NULL;
	}

	//init textures
	data->texture = NULL;
	data->textureDisabled = NULL;
	data->textureSelected = NULL;

	SDL_Texture* texture = getTexture(renderer, image);
	if (texture == NULL) {
		destroyButton(res);
		return NULL ;
	}

	SDL_Texture* textureDisabled = NULL;
	if (disabledImage != NULL){
		textureDisabled = getTexture(renderer, disabledImage);
		if (textureDisabled == NULL) {
			destroyButton(res);
			return NULL ;
		}
	}

	SDL_Texture* textureSelected = NULL;
	if (selectedImage!=NULL){
		textureSelected = getTexture(renderer, selectedImage);
		if (textureSelected == NULL){
			destroyButton(res);
			return NULL ;
		}
	}


	// store button details
	data->texture = texture;
	data->textureDisabled = textureDisabled;
	data->textureSelected = textureSelected;
	data->action = action;
	data->disable = disableButton;
	data->disabled = false;
	data->hidden = false;
	data->selected = false;
	data->val = val;

	res->data = data;

	return res;
}

/** Destroys the given button widget src
 *  in case src == NULL, does nothing
 *
 *  @param src- the widget src
 *
 * */
void destroyButton(Widget* src)
{
	if (src == NULL){
		return;
	}
	Button* button = (Button*) src->data;
	if (button == NULL){
		free(src);
		return;
	}
	if (button->texture != NULL){
		SDL_DestroyTexture(button->texture);
	}
	if (button->textureDisabled != NULL){
		SDL_DestroyTexture(button->textureDisabled);
	}
	if (button->textureSelected != NULL){
		SDL_DestroyTexture(button->textureSelected);
	}
	free(button);
	free(src);
}

/** Handles the specified event
 *
 * @param src - the button widget src
 * @param event - the event to be handled
 * */
void handleButtonEvent(Widget* src, SDL_Event* e)
{
	Button* button = (Button*) src->data;
	if (button->disabled || button->hidden){
		return;
	}
	bool inRect = isInRect(e->button.x, e->button.y, src->location);
	if (inRect && e->type == SDL_MOUSEBUTTONUP) {
		button->selected = true;
		button->action(src, e);
	}
}

/** Draws the specified button to the screen
 *
 * @param src - the button widget src
 * @param renderer - the renderer
 *
 * @return -1, on error
 * 0, on success*/
int drawButton(Widget* src, SDL_Renderer* render)
{
	Button* button = (Button*) src->data;
	if (button->hidden){
		return 0;
	}

	SDL_Texture *texture = (button->disabled) ? button->textureDisabled : button->texture;
	texture = (button->selected && button->textureSelected!=NULL) ? button->textureSelected : texture;

	int textureWidth, textureHeight;
	SDL_QueryTexture(texture, NULL, NULL, &textureWidth, &textureHeight);

	updateLocation(src);
	updateSrcRect(src, textureWidth, textureHeight);
	return SDL_RenderCopy(render, texture, &src->srcRect, &src->location);
}

/** Disables/Enables the specified button src
 *
 * @param src - the button src
 * @param disable - whether to disable or enable the button
 *
 * */
void disableButton (Widget* src, bool disable){
	Button* button = (Button*) src->data;
	button->disabled = disable;
}

/**Hides/shows the specified button
 *
 * @param src - the button widget src
 * @param hide - whether to hide/show the button
 *
 * */
void hideButton (Widget *src, bool hide){
	Button *btn = (Button*)src->data;
	btn->hidden = hide;
}

/**Selects/unselect the specified button
 *
 * @param src - the button widget src
 * @param select - whether to select/unselect the button
 *
 * */
void selectButton (Widget *src, bool select){
	Button *btn = (Button*)src->data;
	btn->selected = select;
}

