#ifndef VSCROLLBAR_H_
#define VSCROLLBAR_H_

#include "UI_Panel.h"
#include "UI_Widget.h"

/**type used to represent a vertical scroll bar*/
typedef struct vscrollbar_t VScrollbar;
struct vscrollbar_t {
	Widget *panel;
	int scrolled;
	Widget *upButton;
	Widget *downButton;
};

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
Widget* createScrollBar(Widget *parentWindow, SDL_Rect location, SDL_Renderer* renderer, Widget *panel);

#endif /* VSCROLLBAR_H_ */
