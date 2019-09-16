#ifndef WIDGET_H_
#define WIDGET_H_

#include <SDL.h>
#include <SDL_video.h>
#include <stdbool.h>

/**type used to represent a widget*/
typedef struct widget_t Widget;
struct widget_t {
	Widget *next;
	Widget *parent;
	Widget *parentWindow;
	SDL_Rect srcRect;
	SDL_Rect location;
	SDL_Rect originalLocation;
	int (*draw)(Widget*, SDL_Renderer*);
	void (*handleEvent)(Widget*, SDL_Event*);
	void (*destroy)(Widget*);
	void* data;
};

/** Creates a new widget with the given parameters specified
 *
 * @param parentWindow - the window contains the widget
 * @param loction - the location of the widget in the screen
 * @param draw - a pointer to function for drawing the widget
 * @param handleEvent - a pointer to function for handling events
 * @param destroy - a pointer to function destroys the widget
 * @param data - additional data the widget should have
 *
 * @return
 * NULL ,if an allocation error occured,
 * A new instance of the widget ,otherwise
 * */
Widget* createWidget(
		Widget *parentWindow,
		SDL_Rect location,
		int (*draw)(Widget*, SDL_Renderer*),
		void (*handleEvent)(Widget*, SDL_Event*),
		void (*destroy)(Widget*),
		void* data);

/** Destroys the given widget src
 *  in case src == NULL, does nothing
 *
 *  @param src- the widget src
 *
 * */
void destroyWidget(Widget* src);

/** Update the location of the widget in the screen
 *  according to its parent's boundaries
 *
 *  @param src - the widget src
 * */
void updateLocation(Widget *src);

/** Update which part of the widget should be drawn to the screen
 *  according to its location and parent's boundaries
 *
 *  @param src - the widget src
 *  @param w - the original width of the widget
 *  @param h - the original height of the widget
 * */
void updateSrcRect (Widget *src, int w, int h);

/** Creates a texture from the given image path
 *
 * @src renderer - the renderer
 * @src img - a path to bmp image
 *
 * @return
 * NULL, if an error occurred
 * The texture, otherwise
 * */
SDL_Texture* getTexture (SDL_Renderer* renderer, const char* image);

/**Returns if the the point (x,y) is inside the specified rectangle*/
bool isInRect (int x, int y, SDL_Rect r);


#endif /* WIDGET_H_ */
