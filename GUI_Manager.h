#ifndef GUI_MANGER_H_
#define GUI_MANGER_H_

#include <SDL.h>
#include <SDL_video.h>
#include <stdbool.h>

//definitions
#define HISTORY_SIZE 6

/**
 * Starts the program in gui mode
 *
 * @return
 * -1, on error
 * 0, on success
 *
 */
int guiMain();

/** Presents the main menu window
 * and hides the previous window
 * */
void goToMainMenu();

/** Presents the previous window*/
void goToPrevWindow();

/** Presents the game window and hides the previous window*/
void goToGameWindow();

/**Presents the loadSave window in load state
 * and hides the previous window*/
void goToLoadWindow();

/**Presents the loadSave window in save state
 * and hides the previous window*/
void goToSaveWindow(bool toQuit, bool toMenu);

/**Presents the gameMode window
 * and hides the previous window*/
void goToGameModeWindow();

/**Presents the difficulty window
 * and hides the previous window*/
void goToDifficultyWindow();

/**Presents the color window
 * and hides the previous window*/
void goToColorWindow();

/** Quits the game*/
void quitGame();





#endif /* GUI_MANGER_H_ */
