#include <SDL.h>
#include "GUI_MainMenuWindow.h"
#include "GUI_Manager.h"
#include "UI_Window.h"
#include "UI_Button.h"
#include "UI_Panel.h"


void newGameClick();
void loadClick ();

/**Called when the new game button was clicked.
 * Goes to game mode window
 * */
void newGameClick(){
	goToGameModeWindow();
}

/**Called when the load button was clicked
 * Goes to load window.
 * */
void loadClick (){
	goToLoadWindow();
}

/** Creates a new window enables the user to start a new game, load a game or quit
 *
 * @return
 * NULL, if an error occured
 * A new instance of the window otherwise
 *
 * */
Widget *createMainMenuWindow(){

	Widget *mainWindow = createWindow("Main Window",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			WINDOW_WIDTH,
			WINDOW_HEIGHT,
			NULL);
	if (mainWindow == NULL){
		return NULL;
	}

	Window *window = mainWindow->data;

	Widget *panel = createPanel(mainWindow, 100, 0, 200, 400);
	if (panel == NULL){
		destroyWidget(mainWindow);
		return NULL;
	}
	addFirstToWindow(mainWindow, panel);

	Widget *buttons[3];
	char *images[3] = {newGameImg, loadGameImg, quitImg};
	SDL_Rect locations[3] = {{0,62,200,50},{0,174,200,50},{0,286,200,50}};
	void (*actions[3])(Widget*, SDL_Event*) = {goToGameModeWindow, goToLoadWindow, quitGame};

	for (int i=0;i<3;i++){
		buttons[i] = createButton(mainWindow, window->rend, images[i], NULL, NULL, locations[i], actions[i], 0);
		if (buttons[i]==NULL){
			destroyWidget(mainWindow);
			return NULL;
		}
		addFirstToPanel(panel, buttons[i]);
	}

	return mainWindow;
}
