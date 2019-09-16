/*
 * GUI_GameModeWindow.c
 *
 *  Created on: 9 באפר 2018
 *      Author: lital
 */
#include <SDL.h>
#include "GUI_GameModeWindow.h"
#include "SPChessGame.h"
#include "Parser.h"
#include "Settings.h"
#include "GUI_Manager.h"
#include "UI_Window.h"
#include "UI_Button.h"
#include "UI_Panel.h"
#include "UI_Label.h"


void gameModeClick(Widget *src, SDL_Event *e);
void nextClick();
void destroyGameModeWindData (void *src);

extern SPChessGame *game;

/** Called in case one of the difficulties levels buttons were clicked.
	Sets the current game mode according to the button selected
 *
 * @param src - the button was clicked
 * @param e - the event
 *
 * */
void gameModeClick(Widget *src, SDL_Event *e){
	if (!src || !e){
		return;
	}
	Button *btn = (Button*)src->data;
	CCommand command;
	command.cmd = SET_MODE;
	command.arg = atoi(btn->val);
	command.validArg = true;
	handleGameModeCommand(game, command, false);
}

/** Called in case the next button was clicked.
 *	If the current game mode is set to one player, goes to the difficulty window
 *	Otherwise, goes to to the game  window
 * */
void nextClick(){
	if (game->game_mode == 1){
		goToDifficultyWindow();
	}
	else {
		goToGameWindow();
	}
}

/** Creates a new window that enables the user to specify the game mode
 *
 * @return
 * NULL, if an error occurred
 * A new instance of the window ,otherwise
 *
 * */
Widget *createGameModeWindow(){
	Widget *gameModeWindow = createWindow("Settings Window",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			WINDOW_WIDTH,
			WINDOW_HEIGHT,
			destroyGameModeWindData);
	if (gameModeWindow == NULL){
		return NULL;
	}

	Window *window = gameModeWindow->data;

	gameModeWindowData *windowData = (gameModeWindowData*)malloc(sizeof(gameModeWindowData));
	if (windowData == NULL) {
		destroyWidget(gameModeWindow);
		return NULL;
	}

	window->data = windowData;


	Widget *panel = createPanel(gameModeWindow, 0, 95, 400, 95);
	if (panel == NULL){
		destroyWidget(gameModeWindow);
		return NULL;
	}
	addFirstToWindow(gameModeWindow, panel);

	Widget *lblGameMode = createLabel(gameModeWindow, window->rend, gameModeImg, (SDL_Rect){140,0,120,25});
	if (lblGameMode == NULL){
		destroyWidget(gameModeWindow);
		return NULL;
	}
	addFirstToPanel(panel, lblGameMode);

	Widget *onePlayer = createButton(gameModeWindow, window->rend, onePlayerImg, NULL, onePlayerSelectedImg, (SDL_Rect){53, 55, 120, 40}, gameModeClick,"1");
	if (onePlayer == NULL){
		destroyWidget(gameModeWindow);
		return NULL;
	}
	addFirstToPanel(panel, onePlayer);

	windowData->btnOnePlayer = onePlayer;

	Widget *twoPlayer = createButton(gameModeWindow, window->rend, twoPlayerImg, NULL, twoPlayerSelectedImg, (SDL_Rect){226, 55, 120, 40}, gameModeClick,"2");
	if (twoPlayer == NULL){
		destroyWidget(gameModeWindow);
		return NULL;
	}
	addFirstToPanel(panel, twoPlayer);

	windowData->btnTwoPlayer = twoPlayer;


	//create bottom panel
	Widget *bottomPanel = createPanel(gameModeWindow, 0, 350, 400, 30);
	if (bottomPanel == NULL){
		destroyWidget(gameModeWindow);
		return NULL;
	}
	addFirstToWindow(gameModeWindow, bottomPanel);

	Widget *btnBack = createButton(gameModeWindow, window->rend, backImg, NULL, NULL, (SDL_Rect){0,0,60,30}, goToMainMenu, NULL);
	if (btnBack == NULL){
		destroyWidget(gameModeWindow);
		return NULL;
	}
	addFirstToPanel(bottomPanel, btnBack);

	Widget *btnNext = createButton(gameModeWindow, window->rend, nextImg, NULL, NULL, (SDL_Rect){340,0,60,30}, nextClick, NULL);
	if (btnNext == NULL){
		destroyWidget(gameModeWindow);
		return NULL;
	}
	addFirstToPanel(bottomPanel, btnNext);
	windowData->btnNext = btnNext;

	Widget *btnStart = createButton(gameModeWindow, window->rend, startImg, NULL, NULL, (SDL_Rect){340,0,60,30}, nextClick, NULL);
	if (btnStart == NULL){
		destroyWidget(gameModeWindow);
		return NULL;
	}
	addFirstToPanel(bottomPanel, btnStart);
	windowData->btnStart = btnStart;

	return gameModeWindow;
}

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
int drawGameModeWindow(Widget *src, SDL_Renderer *renderer){
	Window *window = (Window*)src->data;
	gameModeWindowData *windowData = (gameModeWindowData*)window->data;

	//select the current game mode button
	int gameMode = game->game_mode;
	if(gameMode == 1){
		selectButton(windowData->btnOnePlayer, true);
		selectButton(windowData->btnTwoPlayer, false);
		hideButton(windowData->btnNext, false);
		hideButton(windowData->btnStart, true);
	}
	else {
		selectButton(windowData->btnOnePlayer, false);
		selectButton(windowData->btnTwoPlayer, true);
		hideButton(windowData->btnNext, true);
		hideButton(windowData->btnStart, false);
	}
	//draw window
	return src->draw(src, renderer);
}

/**Destroys the gameModeWindowData
 *
 * @param data - the gameModeWindowData src
 *
 * */
void destroyGameModeWindData (void *data){
	if (data != NULL){
		free(data);
	}
}
