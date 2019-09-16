#ifndef PANEL_H_
#define PANEL_H_

#include "UI_Widget.h"

/**type used to repredent a panel widget*/
typedef struct panel_t Panel;
struct panel_t {
	Widget *head;
	Widget *tail;
};

/** Creates a new panel widget
 *
 * @param parentWindow - the window contains the widget
 * @param x,y,w,h - the location of the widget in the screen
 *
 * @return
 * NULL ,if an allocation error occured,
 * A new instance of the widget ,otherwise
 * */
Widget* createPanel(Widget *parentWindow, int x, int y, int w, int h);

/** Adds the specified widget to the panel
 * so it will be the first one to be drawn
 *
 * @param src - the panel widget src
 * @param widget - the widget should be added to the panel
 *
 * */
void addFirstToPanel(Widget *src, Widget *widget);

/** Adds the specified widget to the panel
 * so it will be the last one to be drawn
 *
 * @param src - the panel widget src
 * @param widget - the widget should be added to the panel
 *
 * */
void addLastToPanel(Widget *src, Widget *widget);

#endif /* PANEL_H_ */
