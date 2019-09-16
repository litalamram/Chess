#ifndef GUI_COLORWINDOW_H_
#define GUI_COLORWINDOW_H_

#include <SDL.h>
#include "UI_Widget.h"

//definitions
#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 400

//paths to images
#define backImg "images/general/back.bmp"
#define startImg "images/general/start.bmp"
#define userColorImg "images/colorWindow/userColor.bmp"
#define whiteImg "images/colorWindow/white.bmp"
#define whiteSelectedImg "images/colorWindow/white_selected.bmp"
#define blackImg "images/colorWindow/black.bmp"
#define blackSelectedImg "images/colorWindow/black_selected.bmp"

/**type used to store relevant data of the window*/
typedef struct color_window_data_t {
	Widget *btnBlack;
	Widget *btnWhite;
} ColorWindowData;

/** Creates a new window that enables the user to specify the color of its pieces
 *
 * @return
 * NULL, if an error occurred
 * A new instance of the window ,otherwise
 *
 * */
Widget *createColorWindow();

/** Draws the window to the screen
 *
 * @param src - the colorWindow src
 * @param render - the renderer
 *
 * @return
 * 0 on success,
 * -1, on error
 *
 * */
int drawColorWindow(Widget *src, SDL_Renderer *renderer);


#endif /* GUI_COLORWINDOW_H_ */
