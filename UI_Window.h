#ifndef WINDOW_H_
#define WINDOW_H_

#include "UI_Widget.h"
#include "UI_Panel.h"
#include <SDL.h>
#include <SDL_video.h>
#include <stdbool.h>

/**type used to represent a window*/
typedef struct window_t Window;
struct window_t {
	SDL_Renderer* rend;
	SDL_Window *window;
	Widget *root;
	void (*show)(Widget* src);
	void (*hide)(Widget* src);
	bool hidden;
	void *data;
	void (*destroyData)(void*);
};

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
Widget* createWindow(char *title, int x, int y, int w, int h, void (*destroyData)(void*));

/** Shows the specified window
 *
 * @param src - the window widget src
 *
 * */
void showWindow(Widget* src);

/** Hides the specified window
 *
 * @param src - the window widget src
 *
 * */
void hideWindow(Widget* src);

/** Adds the specified widget to window
 * so it will be the first one to be drawn
 *
 * @param src - the window widget src
 * @param widget - the widget should be added to the window
 *
 * */
void addFirstToWindow(Widget *src, Widget *widget);

/** Adds the specified widget to window
 * so it will be the last one to be drawn
 *
 * @param src - the window widget src
 * @param widget - the widget should be added to the window
 *
 * */
void addLastToWindow(Widget *src, Widget *widget);

#endif /* WINDOW_H_ */
