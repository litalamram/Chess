#include "GUI_DifficultyWindow.h"
#include "GUI_Manager.h"
#include "SPChessGame.h"
#include "Parser.h"
#include "Settings.h"
#include "UI_Window.h"
#include "UI_Button.h"
#include "UI_Panel.h"
#include "UI_Label.h"


void difficultyClick(Widget *src, SDL_Event *e);
void destroyDifficultyWindData (void *src);

extern SPChessGame *game;

/**Sets the difficulty level according to the selected button
 *
 * @param src - the button was clicked
 * @param e - the event
 *
 * */
void difficultyClick(Widget *src, SDL_Event *e){
	if (!src || !e){
		return;
	}
	Button *btn = (Button*)src->data;
	CCommand command;
	command.cmd = SET_DIFF;
	command.arg = atoi(btn->val);
	command.validArg = true;
	handleDifficultyCommand(game, command, false);
}

/** Creates a new window that enables the user to specify the difficulty level
 *
 * @return
 * NULL, if an error occurred
 * A new instance of the window ,otherwise
 *
 * */
Widget *createDifficultyWindow(){

	Widget *difficultyWindow = createWindow("Settings Window",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			WINDOW_WIDTH,
			WINDOW_HEIGHT,
			destroyDifficultyWindData);
	if (difficultyWindow == NULL){
		return NULL;
	}

	Window *window = difficultyWindow->data;

	DifficultyWindowData *windowData = (DifficultyWindowData *)malloc(sizeof(DifficultyWindowData));
	if (windowData == NULL){
		destroyWidget(difficultyWindow);
		return NULL;
	}
	window->data = windowData;

	Widget *difficultyPanel = createPanel(difficultyWindow, 0, 95, WINDOW_WIDTH, 200);
	if (difficultyPanel == NULL){
		destroyWidget(difficultyWindow);
		return NULL;
	}
	addFirstToWindow(difficultyWindow, difficultyPanel);

	SDL_Rect rect = { .x = 140, .y = 0, .w = 120, .h = 25 };
	Widget *lblDifficulty = createLabel(difficultyWindow, window->rend, difficultyImg, rect);
	if (lblDifficulty == NULL){
		destroyWidget(difficultyWindow);
		return NULL;
	}
	addFirstToPanel(difficultyPanel, lblDifficulty);

	//create difficulty buttons
	char *images[NUM_DIFFICULTIES] = {amatuerImg, easyImg, moderateImg, hardImg, expertImg};
	char *selectedImages[NUM_DIFFICULTIES] = {amatuerSelectedImg, easySelectedImg, moderateSelectedImg, hardSelectedImg, expertSelectedImg};
	SDL_Rect locations[NUM_DIFFICULTIES] = {{53, 55, 120, 40}, {226, 55, 120, 40}, {53, 100, 120, 40}, {226, 100, 120, 40}, {140, 150, 120, 40}};
	char *vals[NUM_DIFFICULTIES] = {"1","2","3","4","5"};
	Widget *btnDifficulty;
	for (int i=0;i<NUM_DIFFICULTIES;i++){
		btnDifficulty = createButton(difficultyWindow, window->rend, images[i], NULL, selectedImages[i], locations[i], difficultyClick, vals[i]);
		if (btnDifficulty == NULL){
			destroyWidget(difficultyWindow);
			return NULL;
		}
		addFirstToPanel(difficultyPanel, btnDifficulty);
		windowData->difficulties[i] = btnDifficulty;
	}

	//create bottom panel
	Widget *bottomPanel = createPanel(difficultyWindow, 0, 350, 400, 30);
	if (bottomPanel==NULL){
		destroyWidget(difficultyWindow);
		return NULL;
	}
	addFirstToWindow(difficultyWindow, bottomPanel);

	Widget *btnBack = createButton(difficultyWindow, window->rend, backImg, NULL, NULL, (SDL_Rect){0,0,60,30}, goToGameModeWindow, NULL);
	if (btnBack==NULL){
		destroyWidget(difficultyWindow);
		return NULL;
	}
	addFirstToPanel(bottomPanel, btnBack);

	Widget *btnNext = createButton(difficultyWindow, window->rend, nextImg, NULL, NULL, (SDL_Rect){340,0,60,30}, goToColorWindow, NULL);
	if (btnNext==NULL){
		destroyWidget(difficultyWindow);
		return NULL;
	}
	addFirstToPanel(bottomPanel, btnNext);

	return difficultyWindow;
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
int drawDifficultyWindow(Widget *src, SDL_Renderer *renderer){
	Window *window = (Window*)src->data;
	DifficultyWindowData *windowData = (DifficultyWindowData*)window->data;
	Widget **difficulties = windowData->difficulties;

	int diff = game->difficulty;
	//unselect all difficulty buttons
	for (int i=0;i<NUM_DIFFICULTIES;i++){
		selectButton(difficulties[i], false);
	}
	//select the current difficulty button
	selectButton(difficulties[diff-1], true);

	return src->draw(src, renderer);
}

/**Destroys the window difficulty data
 *
 * @param data - the difficultyWindowData src
 *
 * */
void destroyDifficultyWindData (void *data){
	if (data != NULL){
		free(data);
	}
}
