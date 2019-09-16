#include <SDL.h>
#include <SDL_video.h>
#include <stdio.h>
#include "SPChessGame.h"
#include "UI_Window.h"
#include <ctype.h>
#include "GUI_LoadSaveWindow.h"
#include "GUI_MainMenuWindow.h"
#include "GUI_GameModeWindow.h"
#include "GUI_LoadSaveWindow.h"
#include "GUI_DifficultyWindow.h"
#include "GUI_ColorWindow.h"
#include "GUI_GameWindow.h"
#include "GUI_Manager.h"
#include "GUI_MessageBox.h"

//globals
int exitLoop = 0;
SPChessGame *game = NULL; //the game src
Widget *currWindow = NULL; //the current window
Widget *prevWindow = NULL; //the previous window
Widget *mainMenuWindow = NULL; //the main menu window
Widget *gameModeWindow = NULL; //the game mode window
Widget *difficultyWindow = NULL; //the difficulty window
Widget *colorWindow = NULL; //the color window
Widget *gameWindow = NULL; //the game window
Widget *loadSaveWindow = NULL; //the load/save window

/** Creates all the windows used by the program
 * @return
 * 0, on success
 * 1, if failed creating main menu window
 * 2, if failed creating game mode window
 * 3, if failed creating difficulty window
 * 4, if failed creating color window
 * 5, if failed creating game window
 * 6, if failed creating load\save window
 * */
int createWindows(){
	mainMenuWindow = createMainMenuWindow();
	if (mainMenuWindow ==NULL) return 1;
	gameModeWindow = createGameModeWindow();
	if (gameModeWindow ==NULL) return 2;
	difficultyWindow = createDifficultyWindow();
	if (difficultyWindow ==NULL) return 3;
	colorWindow = createColorWindow();
	if (colorWindow ==NULL) return 4;
	gameWindow = createGameWindow();
	if (gameWindow ==NULL) return 5;
	loadSaveWindow = createLoadSaveWindow();
	if (loadSaveWindow ==NULL) return 6;
	return 0;
}

void goToMainMenu(){
	spChessGameDestroy(game);
	game = NULL;
	hideWindow(currWindow);
	showWindow(mainMenuWindow);
	prevWindow = currWindow;
	currWindow = mainMenuWindow;
}

void goToPrevWindow(){
	if (prevWindow == mainMenuWindow){
		spChessGameDestroy(game);
		game = NULL;
	}
	hideWindow(currWindow);
	showWindow(prevWindow);
	Widget *tmp = currWindow;
	currWindow = prevWindow;
	prevWindow = tmp;
}

void goToGameWindow(){
	hideWindow(currWindow);
	prevWindow = currWindow;
	currWindow = gameWindow;
	showGameWindow(gameWindow);
}

void goToLoadWindow(){
	if (game == NULL){ //allocate new game
		game = spChessGameCreate(HISTORY_SIZE);
	}
	if(game == NULL){//error
		printf ("ERROR: unable to allocate memory for game\n");
		showMessageBox("ERROR", "unable to allocate memory for game\n");
		exitLoop = 1;
		return;
	}
	hideWindow(currWindow);
	prevWindow = currWindow;
	currWindow = loadSaveWindow;
	showLoadSaveWindow(loadSaveWindow, true, false, false);
}

void goToSaveWindow(bool toQuit, bool toMenu){
	hideWindow(currWindow);
	prevWindow = currWindow;
	currWindow = loadSaveWindow;
	showLoadSaveWindow(loadSaveWindow, false, toQuit, toMenu);
}

void goToGameModeWindow(){
	if (game == NULL){ //allocate new game
		game = spChessGameCreate(HISTORY_SIZE);
	}
	if(game == NULL){//error
		printf ("ERROR: unable to allocate memory for game\n");
		showMessageBox("ERROR", "unable to allocate memory for game\n");
		exitLoop = 1;
		return;
	}
	hideWindow(currWindow);
	showWindow(gameModeWindow);
	prevWindow = currWindow;
	currWindow = gameModeWindow;
}

void goToDifficultyWindow(){
	hideWindow(currWindow);
	showWindow(difficultyWindow);
	prevWindow = currWindow;
	currWindow = difficultyWindow;
}

void goToColorWindow(){
	hideWindow(currWindow);
	showWindow(colorWindow);
	prevWindow = currWindow;
	currWindow = colorWindow;
}

void quitGame(){
	exitLoop = 1;
}

/** Destroys all resources used by the program*/
void destroyAllResources(){
	spChessGameDestroy(game);
	destroyWidget(mainMenuWindow);
	destroyWidget(gameModeWindow);
	destroyWidget(difficultyWindow);
	destroyWidget(colorWindow);
	destroyWidget(gameWindow);
	destroyWidget(loadSaveWindow);
	SDL_Quit();
}

/** Draws the current window
 *
 * @return
 * 0, on success
 * -1, on error
 *
 * */
int drawCurrentWindow(SDL_Renderer *renderer){
	int drawStatus = 0;
	if (currWindow == gameWindow){
		drawStatus = drawGameWindow(currWindow, renderer);
	}
	else if (currWindow == colorWindow){
		drawStatus = drawColorWindow(currWindow, renderer);
	}
	else if (currWindow == difficultyWindow){
		drawStatus = drawDifficultyWindow(currWindow, renderer);
	}
	else if (currWindow == gameModeWindow){
		drawStatus = drawGameModeWindow(currWindow, renderer);
	}
	else {
		drawStatus = currWindow->draw(currWindow, renderer);
	}
	return drawStatus;

}

int guiMain(){
	// initialize SDL2 for video
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return -1;
	}

	int s = createWindows();
	if (s != 0){
		showMessageBox("ERROR:", "unable to create window\n");
		printf("ERROR: unable to create window :%s\n", SDL_GetError());
		destroyAllResources();
		return -1;
	}

	currWindow = mainMenuWindow;
	showWindow(currWindow);

	SDL_Renderer *renderer;
	//event loop
	SDL_Event e;
	while (!exitLoop) {
		//draw
		renderer = ((Window*)currWindow->data)->rend;
		if (drawCurrentWindow(renderer) == -1){
			showMessageBox("ERROR:", "unable to draw window\n");
			printf("ERROR: unable to draw window: %s\n", SDL_GetError());
			exitLoop = 1;
		}
		// present changes to user
		SDL_RenderPresent(renderer);

		//handle event
		while (SDL_PollEvent(&e)) {
			if (currWindow == gameWindow){
				handleEventGameWindow(currWindow, &e);
			}
			else{
				currWindow->handleEvent(currWindow, &e);
			}
			switch (e.type) {
			case SDL_QUIT:
				if (currWindow == gameWindow){
					quitClick();
				}
				else
					exitLoop = 1;
				break;
			case SDL_WINDOWEVENT:
				if (e.window.event == SDL_WINDOWEVENT_CLOSE){
					if (currWindow == gameWindow){
						quitClick();
					}
					else
						exitLoop = 1;
				}
				break;
			case SDL_KEYDOWN:
				if (e.key.keysym.sym == SDLK_ESCAPE){
					if (currWindow == gameWindow){
						quitClick();
					}
					else
						exitLoop = 1;
				}
				break;
			}
		}


		// small delay
		SDL_Delay(50);

	}


	// free everything and finish
	destroyAllResources();
	return 1;
}

