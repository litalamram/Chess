#include "SPChessGameSettings.h"
#include <stdio.h>
#include <string.h>

#define LINE_LEN 30

#define AMATEUER_TXT "amateur"
#define EASY_TXT "easy"
#define MODERATE_TXT "moderate"
#define HARD_TXT "hard"
#define EXPERT_TXT "expert"

#define AMATEUER_INT 1
#define EASY_INT 2
#define MODERATE_INT 3
#define HARD_INT 4
#define EXPERT_INT 5

#define WHITE_TXT "white"
#define BLACK_TXT "black"

#define SETTINGS_TXT "SETTINGS:"
#define GAME_MODE_TXT "GAME_MODE:"
#define DIFFICULTY_TXT "DIFFICULTY:"
#define USER_COLOR_TXT "USER_COLOR:"


/********************Prototypes**************************/

SP_CHESS_GAME_MESSAGE spChessGameSettingsPrintCurrentPlayerToFile (FILE *f, SPChessGame* src);
SP_CHESS_GAME_MESSAGE spChessGameSettingsSetCurrentPlayer (SPChessGame* src, int playerColor);
int unParseDifficulty (char *difficulty);
int unParseColor(char *color);
void parseAndSetLine (SPChessGame *src, char *line);
void setBoardRow (SPChessGame *src, char *line);

/*******************Implementation***********************/

/** Parses the specified difficulty level to string
 *
 * @param difficulty
 *
 * @return
 * AMATEUER_TXT - if difficulty==AMATEUER_INT
 * EASY_TXT - if difficulty==EASY_INT
 * MODERATE_TXT - if difficulty==MODERATE_INT
 * HARD_TXT - if difficulty==HARD_INT
 * EXPERT_TXT - if difficulty==EXPERT_INT
 * NULL - otherwise
 */
char* parseDifficulty (int difficulty){
	switch (difficulty){
	case AMATEUER_INT:
		return AMATEUER_TXT;
	case EASY_INT:
		return EASY_TXT;
	case MODERATE_INT:
		return MODERATE_TXT;
	case HARD_INT:
		return HARD_TXT;
	case EXPERT_INT:
		return EXPERT_TXT;
	}
	//should never get here
	return NULL;
}

/** Parses the specified color to string
 *
 * @param color
 * @return
 * BLACK_TXT - if color==SP_CHESS_GAME_BLACK
 * WHITE_TXT - otherwise
 */
char* parseColor(int color){
	if (color == SP_CHESS_GAME_BLACK){
		return BLACK_TXT;
	}
	return WHITE_TXT; //color == SP_CHESS_GAME_WHITE
}

/** Unparses the specified difficulty level to int
 *
 * @param difficulty
 *
 * @return
 * AMATEUER_INT - if difficulty is AMATEUER_TXT
 * EASY_INT - if difficulty is EASY_TXT
 * MODERATE_INT - if difficulty is MODERATE_TXT
 * HARD_INT - if difficulty is HARD_TXT
 * EXPERT_INT - if difficulty is EXPERT_TXT
 * -1 - otherwise
 */
int unParseDifficulty (char *difficulty){
	if (strcmp(difficulty, AMATEUER_TXT) == 0){
		return AMATEUER_INT;
	}
	else if (strcmp(difficulty, EASY_TXT) == 0){
		return EASY_INT;
	}
	else if (strcmp(difficulty, MODERATE_TXT) == 0){
		return MODERATE_INT;
	}
	else if (strcmp(difficulty, HARD_TXT) == 0){
		return HARD_INT;
	}
	else if (strcmp(difficulty, EXPERT_TXT) == 0){
		return EASY_INT;
	}

	return -1;
}

/** UnParse the specified color to string
 *
 * @param color
 * @return
 * SP_CHESS_GAME_BLACK if color is BLACK_TXT
 * SP_CHESS_GAME_WHITE - otherwise
 */
int unParseColor(char *color){
	if (strcmp(color, BLACK_TXT) == 0){
		return SP_CHESS_GAME_BLACK;
	}
	return SP_CHESS_GAME_WHITE;
}

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
SP_CHESS_GAME_MESSAGE spChessGameSettingsSetGameMode(SPChessGame* src, int gameMode){
	if (gameMode!=1 && gameMode!=2){
		return SP_CHESS_GAME_INVALID_ARGUMENT;
	}
	src->game_mode = gameMode;
	return SP_CHESS_GAME_SUCCESS;
}

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
SP_CHESS_GAME_MESSAGE spChessGameSettingsSetDifficulty (SPChessGame* src, int difficulty){
	if (difficulty<1 || difficulty>5){
		return SP_CHESS_GAME_INVALID_ARGUMENT;
	}
	src->difficulty = difficulty;
	return SP_CHESS_GAME_SUCCESS;
}

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
SP_CHESS_GAME_MESSAGE spChessGameSettingsSetUserColor (SPChessGame* src, int userColor){
	if (userColor != SP_CHESS_GAME_BLACK && userColor != SP_CHESS_GAME_WHITE){
		return SP_CHESS_GAME_INVALID_ARGUMENT;
	}
	src->userColor = userColor;
	return SP_CHESS_GAME_SUCCESS;
}

/**Sets the current player for the specified game
 * If the user color specified is not valid
 * then the function does nothing
 *
 * @param src - playerColor game source
 * @param gameMode - the game mode
 *
 * @return
 * SP_CHESS_GAME_INVALID_ARGUMENT - if the player color specified is neither
 * 									SP_CHESS_GAME_BLACK nor SP_CHESS_GAME_WHITE
 * SP_CHESS_GAME_SUCCESS - otherwise
 */
SP_CHESS_GAME_MESSAGE spChessGameSettingsSetCurrentPlayer (SPChessGame* src, int playerColor){
	if (playerColor != SP_CHESS_GAME_BLACK && playerColor != SP_CHESS_GAME_WHITE){
		return SP_CHESS_GAME_INVALID_ARGUMENT;
	}
	src->currentPlayer = playerColor;
	return SP_CHESS_GAME_SUCCESS;
}

/** Prints the current game settings to the specified file
 *
 * @param file - the output file
 * @param src - the game source
 *
 * @return
 * SP_CHESS_GAME_IO_EXCEPTION - if an error occurred writing to file
 * SP_CHESS_GAME_SUCCESS - otherwise
 */
SP_CHESS_GAME_MESSAGE spChessGameSettingsPrintToFile (FILE *f, SPChessGame* src){
	if (fprintf(f, "%s\n", SETTINGS_TXT)<0){
		return SP_CHESS_GAME_IO_EXCEPTION;
	}
	if (fprintf(f, "%s %d-player\n", GAME_MODE_TXT, src->game_mode)<0){
		return SP_CHESS_GAME_IO_EXCEPTION;
	}
	if(src->game_mode==1){
		if (fprintf(f, "%s %s\n", DIFFICULTY_TXT, parseDifficulty(src->difficulty))<0){
			return SP_CHESS_GAME_IO_EXCEPTION;
		}
		if (fprintf(f, "%s %s\n", USER_COLOR_TXT, parseColor(src->userColor))<0){
			return SP_CHESS_GAME_IO_EXCEPTION;
		}
	}
	return SP_CHESS_GAME_SUCCESS;
}

/** Prints the current player's color to the specified file
 * @param f - the output file
 * @param src - the source game
 *
 * @return
 * SP_CHESS_GAME_IO_EXCEPTION - if an error occurred writing to file
 * SP_CHESS_GAME_SUCCESS - otherwise
 *
 */
SP_CHESS_GAME_MESSAGE spChessGameSettingsPrintCurrentPlayerToFile (FILE *f, SPChessGame* src){
	int currentPlayer = spChessGameGetCurrentPlayer(src);
	char *color = parseColor(currentPlayer);
	if (fprintf(f, "%s\n", color) < 0){
		return SP_CHESS_GAME_IO_EXCEPTION;
	}
	return SP_CHESS_GAME_SUCCESS;
}

/** Fills the relevant row of the game board according to the line specified
 *
 * @param src - the src game
 * @param line - the linr to fill in the board
 *
 * @pre line is in the format line_number| %c %c ........ %c|*/
void setBoardRow (SPChessGame *src, char *line){
	int len = strlen(line);
	int row = spChessGameParseRow(line[0]);
	int col = 0;
	for (int k=0; k<len; k++){
		if (line[k]==SP_CHESS_GAME_EMPTY_ENTRY || (line[k]>='a'&&line[k]<='z') || (line[k]>='A'&&line[k]<='Z')){
			src->gameBoard[row][col] = line[k];
			col++;
		}
	}
}

/**Updates the src game settings according to the line specified
 *
 * @param src - the src game
 * @param line - the line
 *
 * If the line is equal to WHITE_TXT or BLACK_TXT, then the current player is set accordingly
 * If the line starts with GAME_MODE_TXT, then the game mode is set accordingly
 * If the line starts with DIFFICULTY_TXT , then the difficulty level is set accordingly
 * If the line starts with USER_COLOR_TXT , then the user color is set accordingly
 * If the line represents a row in the board game, then this row is set accordingly*/
void parseAndSetLine (SPChessGame *src, char *line){
	char lineCopy[LINE_LEN];
	strcpy(lineCopy, line);
	char *token;
	char *delimiter = " \r\n";
	token = strtok(lineCopy , delimiter); //get first token
	//current player
	if (strcmp(token , WHITE_TXT) == 0 || strcmp(token , BLACK_TXT) == 0){
		spChessGameSettingsSetCurrentPlayer(src, unParseColor(token));
	}

	//game mode
	else if (strcmp(token , GAME_MODE_TXT) == 0){
		token = strtok(NULL, delimiter); //get next token
		if (strcmp(token,"1-player")==0){
			spChessGameSettingsSetGameMode(src, 1);
		}
		else {
			spChessGameSettingsSetGameMode(src, 2);
		}
	}
	//difficulty
	else if (strcmp(token , DIFFICULTY_TXT) == 0){
		token = strtok(NULL, delimiter); //get next token
		spChessGameSettingsSetDifficulty(src, unParseDifficulty(token));
	}
	//user color
	else if (strcmp(token , USER_COLOR_TXT) == 0){
		token = strtok(NULL, delimiter); //get next token
		spChessGameSettingsSetUserColor(src, unParseColor(token));
	}
	//board line
	else if (token[0]>='1' && token[0]<='8'){
		setBoardRow(src, line);
	}
}

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
SP_CHESS_GAME_MESSAGE spChessGameSettingsLoad (SPChessGame* src, char *fileName){

	FILE *f;
	char line[LINE_LEN];

	if (!src && !fileName){
		return SP_CHESS_GAME_INVALID_ARGUMENT;
	}
	f = fopen(fileName, "r");
	if (!f) {
		return SP_CHESS_GAME_IO_EXCEPTION;
	}

	while (fgets(line, LINE_LEN, f) != NULL){
		parseAndSetLine(src, line);
	}


	if (!feof(f)){
		fclose(f);
		return SP_CHESS_GAME_IO_EXCEPTION;
	}

	fclose(f);

	spChessGameCleanHistory(src); //delete history

	return SP_CHESS_GAME_SUCCESS;
}

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
SP_CHESS_GAME_MESSAGE spChessGameSettingsSave (SPChessGame* src, char *fileName){
	FILE *f;
	if (!src && !fileName){
		return SP_CHESS_GAME_INVALID_ARGUMENT;
	}
	f = fopen(fileName, "w");
	if (!f) {
		return SP_CHESS_GAME_IO_EXCEPTION;
	}
	if (spChessGameSettingsPrintCurrentPlayerToFile(f, src) != SP_CHESS_GAME_SUCCESS){
		fclose(f);
		return SP_CHESS_GAME_IO_EXCEPTION;
	}
	if (spChessGameSettingsPrintToFile(f, src) != SP_CHESS_GAME_SUCCESS){
		fclose(f);
		return SP_CHESS_GAME_IO_EXCEPTION;
	}
	if (spChessGamePrintBoardToFile(f, src) != SP_CHESS_GAME_SUCCESS){
		fclose(f);
		return SP_CHESS_GAME_IO_EXCEPTION;
	}
	fclose(f);
	return SP_CHESS_GAME_SUCCESS;
}

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
SP_CHESS_GAME_MESSAGE spChessGameSettingsSetDefaultsSettings (SPChessGame* src){
	if (src == NULL){
		return SP_CHESS_GAME_INVALID_ARGUMENT;
	}
	src->difficulty = SP_CHESS_GAME_DEFAULT_DIFFICULTY;
	src->game_mode = SP_CHESS_GAME_DEFAULT_GAME_MODE;
	src->userColor = SP_CHESS_GAME_DEFAULT_USER_COLOR;
	return SP_CHESS_GAME_SUCCESS;
}
