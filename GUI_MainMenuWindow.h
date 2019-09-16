#ifndef GUI_MAINMENUWINDOW_H_
#define GUI_MAINMENUWINDOW_H_

#include "UI_Widget.h"

//definitions

#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 400

//paths to images

#define newGameImg "images/mainMenu/newGame.bmp"
#define loadGameImg "images/general/loadGame.bmp"
#define quitImg "images/general/quitGame.bmp"

/** Creates a new window enables the user to start a new game, load a game or quit
 *
 * @return
 * NULL, if an error occured
 * A new instance of the window otherwise
 *
 * */
Widget *createMainMenuWindow();


#endif /* GUI_MAINMENUWINDOW_H_ */
