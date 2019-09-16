#include <SDL.h>
#include <SDL_video.h>
#include <stdio.h>
#include "GUI_LoadSaveWindow.h"
#include "GUI_Manager.h"
#include "Settings.h"
#include "ConsoleMode.h"
#include "SPChessGame.h"
#include "SPChessGameSettings.h"
#include "UI_Window.h"
#include "UI_Button.h"
#include "UI_Label.h"
#include "UI_vScrollbar.h"
#include "GUI_MessageBox.h"
#include "Parser.h"

void handleLoadSave (Widget *src, SDL_Event *event);
void destroyLoadSaveWindData (void *data);

extern SPChessGame *game;

/** Called when one of the slots is clicked
 *
 * If we are currently displaying a load window,
 * loads the game from the selected slot, and goes to game window.
 *
 * If we are currently displaying a save window,
 * saves the game to the selected slot,
 * and quit/go to game window/ go to main menu
 * (according to the parameters specified at showLoadSaveWindow).
 *
 * If an error occured reading/writing the file, an error message box is displayed
 *
 * @param src - the slot widget src
 * @param event - the event
 *
 * */
void handleLoadSave (Widget *src, SDL_Event *event){
	if (!src || !event){
		return;
	}
	Widget *window = src->parentWindow;
	LoadSaveWindowData *windowData = ((Window*)window->data)->data;
	bool isLoad = windowData->isLoad;
	bool toQuit = windowData->toQuit;
	bool toMenu = windowData->toMenu;

	Button *btn = (Button*)src->data;
	CCommand command;
	command.cmd = (isLoad) ? LOAD : SAVE;
	strcpy(command.path, btn->val);
	command.validArg = true;
	if (isLoad){
		command.cmd = LOAD;
		if(handleLoadCommand(game, command, false) == SP_GAME_STATUS_IO_EXCEPTION){
			showMessageBox("ERROR", "File doesn't exist or cannot be opened\n");
			printf("ERROR: File doesn't exist or cannot be opened\n");
		}
		else {
			goToGameWindow();
		}
	}
	else{
		command.cmd = SAVE;
		if (handlePlayerCommand(game, command, false) == SP_GAME_STATUS_IO_EXCEPTION){
			showMessageBox("ERROR", "File cannot be created or modified\n");
			printf("ERROR: File cannot be created or modified\n");
		}
		else {
			game->isSaved = 1;
			if (toQuit){
				quitGame();
			}
			else if (toMenu){
				goToMainMenu();
			}
			else {
				goToGameWindow();
			}
		}
	}
}

/** Creates a new window that enables
 * the user to load/save a game to/from a given slot
 *
 * @return
 * NULL, if an error occured
 * A new instance of the window, otherwise
 *
 * */
Widget* createLoadSaveWindow(){

	Widget *window = createWindow("Load Window",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			WINDOW_WIDTH,
			WINDOW_HEIGHT,
			destroyLoadSaveWindData);

	if (window == NULL){
		return NULL;
	}

	Window *data = window->data;

	LoadSaveWindowData *windowData = (LoadSaveWindowData*)malloc(sizeof(LoadSaveWindowData));
	if (windowData == NULL){
		destroyWidget(window);
		return NULL;
	}
	data->data = windowData;
	windowData->isLoad = false;
	windowData->toQuit = false;
	windowData->toMenu = false;

	char* arrFileNames[NUM_SLOTS] = FILE_NAMES;
	char* arrSlotsImags[NUM_SLOTS] = SLOTS_IMAGES;
	char* arrInactiveImages[NUM_SLOTS] = SLOTS_INACTIVE_IMAGES;


	Widget *panel = createPanel(window, 0, 50, 400, 60*NUM_SLOTS);
	windowData->lblLoad = createLabel(window ,data->rend, loadLabelImg, (SDL_Rect){50,0,300,40});
	if (windowData->lblLoad == NULL){
		destroyWidget(window);
		return NULL;
	}
	addFirstToWindow(window, windowData->lblLoad);
	windowData->lblSave = createLabel(window ,data->rend, saveLabelImg, (SDL_Rect){50,0,300,40});
	if (windowData->lblSave == NULL){
		destroyWidget(window);
		return NULL;
	}
	addFirstToWindow(window, windowData->lblSave);

	Widget *sb = createScrollBar(window, (SDL_Rect){5,50,400,300},data->rend,panel);
	if (sb == NULL){
		destroyWidget(window);
		return NULL;
	}
	addFirstToWindow(window, sb);

	//create slots
	for (int i=0;i<NUM_SLOTS;i++){
		windowData->slots[i] = createButton(window, data->rend, arrSlotsImags[i], arrInactiveImages[i], arrSlotsImags[i], (SDL_Rect){.x = 100, .y = 60*i, .w = 200, .h = 50}, handleLoadSave, arrFileNames[i]);
		if (windowData->slots[i]==NULL){
			destroyWidget(window);
			return NULL;
		}
		addFirstToPanel(panel, windowData->slots[i]);
	}

	/*Widget* button = createButton(data->rend, loadImg, NULL, NULL, (SDL_Rect){.x = 300, .y = 350, .w = 100, .h = 50}, handleLoad, 0);
	addFirstToWindow(window, button);*/
	Widget *btnBack = createButton(window, data->rend, backImg, NULL, NULL, (SDL_Rect){.x = 5, .y = 350, .w = 60, .h = 30}, goToPrevWindow, 0);
	if (btnBack == NULL){
		destroyWidget(window);
		return NULL;
	}
	addFirstToWindow(window, btnBack);

	return window;

}

/**Presents the window to the screen
 *
 * @param src - the loadSave window src
 * @param load - whether it should be a load ro save window
 * @param quit - should we quit after the save operation completes
 * @param menu - should we go to main menu after the save operation completes
 * */
void showLoadSaveWindow(Widget *src, bool load, bool quit, bool menu){
	Window *window = (Window*)src->data;
	LoadSaveWindowData *windowData = (LoadSaveWindowData*)window->data;
	windowData->isLoad = load;
	windowData->toQuit = quit;
	windowData->toMenu = menu;
	char *title;
	if (windowData->isLoad){ //load window
		title = "Load Window";
		//show load label
		hideLabel(windowData->lblLoad,false);
		hideLabel(windowData->lblSave,true);
		//disable unsaved slots
		char* arrFileNames[NUM_SLOTS] = FILE_NAMES;
		for (int i=0;i<NUM_SLOTS;i++){
			FILE *f = fopen(arrFileNames[i],"r");
			if (f==NULL){//file doesn't exist
				//disable slot
				disableButton(windowData->slots[i], true);
			}
			else {
				//enable slot
				disableButton(windowData->slots[i], false);
				fclose(f);
			}
		}
	}

	else { //save window
		title = "Save Window";
		//show save label
		hideLabel(windowData->lblLoad,true);
		hideLabel(windowData->lblSave,false);
		for (int i=0;i<NUM_SLOTS;i++){
			//enable slot
			disableButton(windowData->slots[i], false);

		}
	}
	SDL_SetWindowTitle((SDL_Window*)window->window, title);
	showWindow(src);
}

/**Destroys the LoadSaveWindowData
 *
 * @param data - the gameWindowData src
 *
 * */
void destroyLoadSaveWindData (void *data){
	if (data != NULL){
		free(data);
	}
}
