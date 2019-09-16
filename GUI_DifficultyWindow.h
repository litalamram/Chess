#ifndef GUI_DIFFICULTYWINDOW_H_
#define GUI_DIFFICULTYWINDOW_H_

#include <SDL.h>
#include "UI_Widget.h"

//definitions
#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 400

//paths to images
#define backImg "images/general/back.bmp"
#define nextImg "images/general/next.bmp"
#define difficultyImg "images/difficultyWindow/difficulty.bmp"

#define amatuerImg "images/difficultyWindow/amateur.bmp"
#define easyImg "images/difficultyWindow/easy.bmp"
#define moderateImg "images/difficultyWindow/moderate.bmp"
#define hardImg "images/difficultyWindow/hard.bmp"
#define expertImg "images/difficultyWindow/expert.bmp"

#define amatuerSelectedImg "images/difficultyWindow/amateur_selected.bmp"
#define easySelectedImg "images/difficultyWindow/easy_selected.bmp"
#define moderateSelectedImg "images/difficultyWindow/moderate_selected.bmp"
#define hardSelectedImg "images/difficultyWindow/hard_selected.bmp"
#define expertSelectedImg "images/difficultyWindow/expert_selected.bmp"

#define NUM_DIFFICULTIES 5

/**type used to store relevant data of the window*/
typedef struct difficulty_window_data_t {
	Widget *difficulties[NUM_DIFFICULTIES];
} DifficultyWindowData;

/** Creates a new window that enables the user to specify the difficulty level
 *
 * @return
 * NULL, if an error occurred
 * A new instance of the window ,otherwise
 *
 * */
Widget *createDifficultyWindow();

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
int drawDifficultyWindow(Widget *src, SDL_Renderer *renderer);


#endif /* GUI_DIFFICULTYWINDOW_H_ */
