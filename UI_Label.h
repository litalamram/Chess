#ifndef LABEL_H_
#define LABEL_H_

#include "UI_Widget.h"
#include <SDL.h>
#include <SDL_video.h>
#include <stdbool.h>

/**type used to represent a label*/
typedef struct label_t Label;
struct label_t {
	SDL_Renderer* render;
	SDL_Texture* texture;
	bool hidden;
};

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
		SDL_Rect location);

/**Hides/shows the specified label
 *
 * @param src - the label widget src
 * @param hide - whether to hide/show the label
 *
 * */
void hideLabel (Widget *src, bool hide);

#endif /* LABEL_H_ */
