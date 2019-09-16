#ifndef BUTTON_H_
#define BUTTON_H_

#include "UI_Widget.h"
#include <SDL.h>
#include <SDL_video.h>
#include <stdbool.h>

/**typed used to represent a button*/
typedef struct button_t Button;
struct button_t {
	SDL_Renderer* render;
	SDL_Texture* texture;
	SDL_Texture* textureDisabled;
	SDL_Texture* textureSelected;
	void (*action)(Widget*, SDL_Event*);
	void (*disable)(Widget* src, bool disable);
	bool disabled;
	bool hidden;
	bool selected;
	char* val;
};

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
		char *val);


/** Disables/Enables the specified button src
 *
 * @param src - the button src
 * @param disable - whether to disable or enable the button
 *
 * */
void disableButton (Widget* src, bool disable);

/**Hides/shows the specified button
 *
 * @param src - the button widget src
 * @param hide - whether to hide/show the button
 *
 * */
void hideButton (Widget *src, bool hide);

/**Selects/unselect the specified button
 *
 * @param src - the button widget src
 * @param select - whether to select/unselect the button
 *
 * */
void selectButton (Widget *src, bool select);



#endif /* BUTTON_H_ */
