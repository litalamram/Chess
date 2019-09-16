#include "GUI_ColorWindow.h"
#include "GUI_Manager.h"
#include "SPChessGame.h"
#include "Parser.h"
#include "Settings.h"
#include "UI_Window.h"
#include "UI_Button.h"
#include "UI_Panel.h"
#include "UI_Label.h"


void destroyColorWindData (void *data);
void userColorClick(Widget *src, SDL_Event *e);

extern SPChessGame *game;

/** Called in case one of the color buttons were clicked.
	Sets the user color according to the selected button
 *
 * @param src - the button was clicked
 * @param event - the event
 * */
void userColorClick(Widget *src, SDL_Event *e){
	if (!src || !e){
		return;
	}
	Button *btn = (Button*)src->data;
	CCommand command;
	command.cmd = SET_COLOR;
	command.arg = atoi(btn->val);
	command.validArg = true;
	handleUserColorCommand(game, command, false);
}

/** Creates a new window that enables the user to specify the color of its pieces
 *
 * @return
 * NULL, if an error occurred
 * A new instance of the window ,otherwise
 *
 * */
Widget *createColorWindow(){
	Widget *colorWindow = createWindow("Settings Window",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			WINDOW_WIDTH,
			WINDOW_HEIGHT,
			destroyColorWindData);
	if (colorWindow == NULL){ //failed creating window
		return NULL;
	}
	Window *window = colorWindow->data;

	ColorWindowData *windowData = (ColorWindowData*)malloc(sizeof(ColorWindowData));
	if (windowData == NULL){
		destroyWidget(colorWindow);
		return NULL;
	}
	window->data = windowData;


	Widget *userColorPanel = createPanel(colorWindow, 0, 95, WINDOW_WIDTH, 100);
	if (userColorPanel == NULL){
		destroyWidget(colorWindow);
		return NULL;
	}
	addFirstToWindow(colorWindow, userColorPanel);

	Widget *lblUserColor = createLabel(colorWindow, window->rend, userColorImg, (SDL_Rect){140,0,120,25});
	if (lblUserColor == NULL){
		destroyWidget(colorWindow);
		return NULL;
	}
	addFirstToPanel(userColorPanel, lblUserColor);

	Widget *btnWhite = createButton(colorWindow, window->rend, whiteImg, NULL, whiteSelectedImg, (SDL_Rect){53, 55, 120, 40}, userColorClick, "1");
	if (btnWhite == NULL){
		destroyWidget(colorWindow);
		return NULL;
	}
	addFirstToPanel(userColorPanel, btnWhite);
	Widget *btnBlack = createButton(colorWindow, window->rend, blackImg, NULL, blackSelectedImg, (SDL_Rect){226, 55, 120, 40}, userColorClick, "0");
	if (btnBlack == NULL){
		destroyWidget(colorWindow);
		return NULL;
	}
	addFirstToPanel(userColorPanel, btnBlack);

	Widget *bottomPanel = createPanel(colorWindow, 0, 350, 400, 30);
	if (bottomPanel == NULL){
		destroyWidget(colorWindow);
		return NULL;
	}
	addFirstToWindow(colorWindow, bottomPanel);

	Widget *btnBack = createButton(colorWindow, window->rend, backImg, NULL, NULL, (SDL_Rect){0,0,60,30}, goToDifficultyWindow, NULL);
	if (btnBack == NULL){
		destroyWidget(colorWindow);
		return NULL;
	}
	addFirstToPanel(bottomPanel, btnBack);

	Widget *btnNext = createButton(colorWindow, window->rend, startImg, NULL, NULL, (SDL_Rect){340,0,60,30}, goToGameWindow, NULL);
	if (btnNext == NULL){
		destroyWidget(colorWindow);
		return NULL;
	}
	addFirstToPanel(bottomPanel, btnNext);

	windowData->btnBlack = btnBlack;
	windowData->btnWhite = btnWhite;

	return colorWindow;
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
int drawColorWindow(Widget *src, SDL_Renderer *renderer){
	Window *window = (Window*)src->data;
	ColorWindowData *windowData = (ColorWindowData*)window->data;

	if(game->userColor == SP_CHESS_GAME_BLACK){
		selectButton(windowData->btnBlack, true);
		selectButton(windowData->btnWhite, false);
	}
	else {
		selectButton(windowData->btnBlack, false);
		selectButton(windowData->btnWhite, true);
	}
	return src->draw(src, renderer);
}

/** Destroys color window data
 *
 * @param data - the data src
 *
 * */
void destroyColorWindData (void *data){
	if (data != NULL){
		free(data);
	}
}
