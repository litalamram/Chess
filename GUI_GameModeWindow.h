#include <SDL.h>
#include "UI_Widget.h"

//definitions

#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 400

//paths to images

#define gameModeImg "images/gameModeWindow/gameMode.bmp"
#define onePlayerImg "images/gameModeWindow/onePlayer.bmp"
#define onePlayerSelectedImg "images/gameModeWindow/onePlayer_selected.bmp"
#define twoPlayerImg "images/gameModeWindow/twoPlayer.bmp"
#define twoPlayerSelectedImg "images/gameModeWindow/twoPlayer_selected.bmp"

#define backImg "images/general/back.bmp"
#define nextImg "images/general/next.bmp"
#define startImg "images/general/start.bmp"

/**type used to store relevant information about the window*/
typedef struct gameMode_window_data_t {
	Widget *btnOnePlayer;
	Widget *btnTwoPlayer;
	Widget *btnNext;
	Widget *btnStart;
} gameModeWindowData;

/** Creates a new window that enables the user to specify the game mode
 *
 * @return
 * NULL, if an error occurred
 * A new instance of the window ,otherwise
 *
 * */
Widget *createGameModeWindow();

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
int drawGameModeWindow(Widget *src, SDL_Renderer *renderer);
