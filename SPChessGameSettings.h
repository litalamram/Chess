#ifndef SPCHESSGAMESETTINGS_H_
#define SPCHESSGAMESETTINGS_H_

#include "SPChessGame.h"

#define SP_CHESS_GAME_DEFAULT_GAME_MODE 1
#define SP_CHESS_GAME_DEFAULT_DIFFICULTY 2
#define SP_CHESS_GAME_DEFAULT_USER_COLOR 1


/**Sets the game mode for the specified game
 * If the game mode specified is not valid
 * then the function does nothing
 *
 * @param src - the game source
 * @param gameMode - the game mode
 *
 * @return
 * SP_CHESS_GAME_INVALID_ARGUMENT - if the game mode specified is not 1 or 2
 * SP_CHESS_GAME_SUCCESS - otherwise
 */
SP_CHESS_GAME_MESSAGE spChessGameSettingsSetGameMode(SPChessGame* src, int gameMode);

/**Sets the difficulty level for the specified game
 * If the difficulty specified is not valid
 * then the function does nothing
 *
 * @param src - the game source
 * @param difficulty - the game mode
 *
 * @return
 * SP_CHESS_GAME_INVALID_ARGUMENT - if the difficulty specified is not between 1 to 5
 * SP_CHESS_GAME_SUCCESS - otherwise
 */
SP_CHESS_GAME_MESSAGE spChessGameSettingsSetDifficulty (SPChessGame* src, int difficulty);

/**Sets the user color for the specified game
 * If the user color specified is not valid
 * then the function does nothing
 *
 * @param src - userColor game source
 * @param gameMode - the game mode
 *
 * @return
 * SP_CHESS_GAME_INVALID_ARGUMENT - if the user color specified is neither
 * 									SP_CHESS_GAME_BLACK nor SP_CHESS_GAME_WHITE
 * SP_CHESS_GAME_SUCCESS - otherwise
 */
SP_CHESS_GAME_MESSAGE spChessGameSettingsSetUserColor (SPChessGame* src, int userColor);

/** Loads the settings and game state
 *  from the specified file to the given src game
 *
 *  @param src - the game source
 *  @param fileName - the name of the file contains the settings
 *
 *  @return
 *  SP_CHESS_GAME_IO_EXCEPTION - if an error occurred reading the file
 *  SP_CHESS_GAME_SUCCESS - otherwise
 */
SP_CHESS_GAME_MESSAGE spChessGameSettingsLoad (SPChessGame* src, char *fileName);

/** Saves the settings and current game state
 *  of the given src game to the specified file
 *
 *  @param src - the game source
 *  @param fileName - the name of output file
 *
 *  @return
 *  SP_CHESS_GAME_IO_EXCEPTION - if an error occurred writing to the file
 *  SP_CHESS_GAME_SUCCESS - otherwise
 */
SP_CHESS_GAME_MESSAGE spChessGameSettingsSave (SPChessGame* src, char *fileName);

/** Initilizes the src game with default settings:
 * difficulty: SP_CHESS_GAME_DEFAULT_DIFFICULTY
 * game_mode: SP_CHESS_GAME_DEFAULT_GAME_MODE
 * userColor: SP_CHESS_GAME_DEFAULT_USER_COLOR
 *
 * @param src - the src game
 *
 * @return
 * SP_CHESS_GAME_INVALID_ARGUMENT - if src==NULL
 * SP_CHESS_GAME_SUCCESS - otherwise
 */
SP_CHESS_GAME_MESSAGE spChessGameSettingsSetDefaultsSettings (SPChessGame* src);

/** Prints the current game settings to the specified file
 *
 * @param file - the output file
 * @param src - the game source
 */
SP_CHESS_GAME_MESSAGE spChessGameSettingsPrintToFile (FILE *file, SPChessGame* src);

/** Parses the specified color to string
 *
 * @param color
 * @return
 * "black" - if color==SP_CHESS_GAME_BLACK
 * "white" - otherwise
 */
char* parseColor(int color);

/** Parses the specified difficulty level to string
 *
 * @param difficulty
 *
 * @return
 * "amateur" - if difficulty==1
 * "easy" - if difficulty==2
 * "moderate" - if difficulty==3
 * "hard" - if difficulty==4
 * "expert" - if difficulty==5
 * NULL - otherwise
 */

char* parseDifficulty (int difficulty);

#endif /* SPCHESSGAMESETTINGS_H_ */
