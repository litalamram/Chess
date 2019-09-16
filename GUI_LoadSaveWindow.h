#ifndef GUI_LOADWINDOW_H_
#define GUI_LOADWINDOW_H_

#include <stdbool.h>
#include <SDL.h>
#include <SDL_video.h>
#include <stdio.h>
#include "Settings.h"
#include "ConsoleMode.h"
#include "SPChessGame.h"
#include "SPChessGameSettings.h"
#include "UI_Widget.h"

//definitions
#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 400

#define NUM_SLOTS 5
#define FILE_NAMES {"files/slot1","files/slot2","files/slot3","files/slot4","files/slot5"}
#define SLOTS_IMAGES {"images/loadSaveWindow/slots/slot1.bmp","images/loadSaveWindow/slots/slot2.bmp","images/loadSaveWindow/slots/slot3.bmp","images/loadSaveWindow/slots/slot4.bmp","images/loadSaveWindow/slots/slot5.bmp"}
#define SLOTS_INACTIVE_IMAGES {"images/loadSaveWindow/slots/slot1_inactive.bmp","images/loadSaveWindow/slots/slot2_inactive.bmp","images/loadSaveWindow/slots/slot3_inactive.bmp","images/loadSaveWindow/slots/slot4_inactive.bmp","images/loadSaveWindow/slots/slot5_inactive.bmp"}
#define backImg "images/general/back.bmp"
#define loadLabelImg "images/loadSaveWindow/loadLabel.bmp"
#define saveLabelImg "images/loadSaveWindow/saveLabel.bmp"

/**type used to store data about the window*/
typedef struct load_save_window_data {
	bool isLoad; //is it load or save window
	bool toQuit; //does the user requested to quit afterwards
	bool toMenu; //does the user requested to go to main menu afterwards
	Widget *lblLoad; //load label
	Widget *lblSave; //save label
	Widget *slots[NUM_SLOTS];
} LoadSaveWindowData;

/** Creates a new window that enables
 * the user to load/save a game to/from a given slot
 *
 * @return
 * NULL, if an error occured
 * A new instance of the window, otherwise
 *
 * */
Widget* createLoadSaveWindow();

/**Presents the window to the screen
 *
 * @param src - the loadSave window src
 * @param load - whether it should be a load ro save window
 * @param quit - should we quit after the save operation completes
 * @param menu - should we go to main menu after the save operation completes*/
void showLoadSaveWindow(Widget *src, bool load,  bool quit, bool menu);

#endif /* GUI_LOADWINDOW_H_ */
