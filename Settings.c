/*
 * Settings.c
 *
 *  Created on: 14 áîøñ 2018
 *      Author: lital
 */
#include "SPChessGameSettings.h"
#include "Settings.h"
#include "Parser.h"
#include "SPChessGame.h"
#include "ConsoleMode.h"

/********************Prototypes**************************/

void printSettingsRequest();
void handleDefaultCommand (SPChessGame *src);

/*******************Implementation***********************/

/**Prints "Specify game settings or type 'start' to begin a game with the current settings"
 */
void printSettingsRequest(){
	printf("Specify game settings or type 'start' to begin a game with the current settings:\n");
}

/**
 * Sets the current game mode according to the specified command
 *
 * @param game - the source game
 * @param command - the spParserPraseLine of the command that the player choose
 * @bool print - whether to print a message or not
 *
 * prints "Wrong game mode" - If a wrong game mode is entered
 * 							(wrong or invalid game mode, or no game mode parameter supplied)
 *otherwise, prints "Game mode is set to XYZ\n", where XYZ is either "2-player" or "1-player"
 */
void handleGameModeCommand (SPChessGame *src, CCommand command, bool print){
	if (command.validArg && (command.arg==1||command.arg==2)){
		spChessGameSettingsSetGameMode(src,command.arg);
		if (print)
			printf("Game mode is set to %d-player\n",command.arg);
	}
	else {
		if (print)
			printf("Wrong game mode\n");
	}
}

/**
 * Sets the difficulty level according to the specified command
 *
 * @param game - the source game
 * @param command - the spParserPraseLine of the command that the player choose
 * @bool print - whether to print a message or not
 *
 * prints "ERROR: invalid command" - if the current game mode is 2-Player
 * prints "Wrong difficulty level.
 * 		   The value should be between 1 to 5" - if difficulty is not in the range 1 to 5
 *otherwise, prints "Difficulty level is set to XYZ\n",
 *			 where XYZ is the name of the difficulty
 */
void handleDifficultyCommand (SPChessGame *src, CCommand command, bool print){
	if (src->game_mode==2){
		if (print)
			printf("ERROR: invalid command\n");
	}
	else if (command.validArg && (command.arg>=1&&command.arg<=5)){
		spChessGameSettingsSetDifficulty(src,command.arg);
		if (print)
			printf("Difficulty level is set to %s\n",parseDifficulty(command.arg));
	}
	else {
		if (print)
			printf("Wrong difficulty level. The value should be between 1 to 5\n");
	}
}

/**
 * Sets the user color according to the specified command
 *
 * @param game - the source game
 * @param command - the spParserPraseLine of the command that the player choose
 * @bool print - whether to print a message or not
 *
 * prints "ERROR: invalid command" - if the current game mode is 2-Player
 * prints "Wrong user color. The value should be 0 or 1" -
 * 		   if the color specified in the command is not 0 or 1
 *otherwise, prints "User color is set to XYZ\n" is printed, where XYZ is 'black' or 'white'
 */
void handleUserColorCommand (SPChessGame *src, CCommand command, bool print){
	if (src->game_mode==2){
		if (print)
			printf("ERROR: invalid command\n");
	}
	else if (command.validArg && (command.arg==0||command.arg==1)){
		spChessGameSettingsSetUserColor(src,command.arg);
		if (print)
			printf("User color is set to %s\n",parseColor(command.arg));
	}
	else {
		if (print)
			printf("Wrong user color. The value should be 0 or 1\n");
	}
}

/** Loads the game setting from the specified file
 *
 * @param src - the game source
 * @param command - the spParserPraseLine of the command that the player choose
 * @bool print - whether to print a message or not
 *
 * print "Error: File doesn’t exist or cannot be opened" -
 * 		  if the file does not exist or cannot be opened
 */
SP_GAME_STATUS handleLoadCommand (SPChessGame *src, CCommand command, bool print){
	if (command.validArg ){
		SP_CHESS_GAME_MESSAGE status = spChessGameSettingsLoad(src,command.path);
		if (status == SP_CHESS_GAME_IO_EXCEPTION){
			if (print)
				printf("Error: File doesn't exist or cannot be opened\n");
			return SP_GAME_STATUS_IO_EXCEPTION;
		}
	}
	else {
		if (print)
			printf("Error: File doesn't exist or cannot be opened\n");
		return SP_GAME_STATUS_IO_EXCEPTION;
	}
	return SP_GAME_STATUS_SUCCESS;
}

/** Resets all game setting to the default values.
 * Prints "All settings reset to default".
 *
 * @param src - the game source
 */
void handleDefaultCommand (SPChessGame *src){
	spChessGameSettingsSetDefaultsSettings(src);
	printf("All settings reset to default\n");
}

/**  Repeatedly gets and handles user commands at the settings state of the game
 *
 *   @param src - the game source
 *
 *   @return
 *   SP_GAME_STATUS_SUCCESS - if the user enters the start command
 *   SP_GAME_STATUS_QUIT - if the user enters the quit command
 */
SP_GAME_STATUS getSettingsFromUser(SPChessGame *src){
	printSettingsRequest();
	char command[MAX_COMMAND_LENGTH+1];
	CCommand parsedCommand;
	fgets(command,MAX_COMMAND_LENGTH,stdin);
	parsedCommand = cParserPraseLine(command);
	while (parsedCommand.cmd != START) {
		if (parsedCommand.cmd == QUIT){
			return SP_GAME_STATUS_QUIT;
		}
		else if (parsedCommand.cmd == SET_MODE){
			handleGameModeCommand(src, parsedCommand, true);
		}

		else if (parsedCommand.cmd == SET_DIFF){
			handleDifficultyCommand(src, parsedCommand, true);
		}
		else if (parsedCommand.cmd == SET_COLOR){
			handleUserColorCommand(src, parsedCommand, true);
		}
		else if (parsedCommand.cmd == LOAD){
			handleLoadCommand(src, parsedCommand, true);
		}
		else if (parsedCommand.cmd == SET_DEFAULT){
			handleDefaultCommand(src);
		}
		else if (parsedCommand.cmd == PRINT_SETTING){
			spChessGameSettingsPrintToFile(stdout,src);
		}
		else {
			printf("ERROR: invalid command\n");
		}
		fgets(command,MAX_COMMAND_LENGTH,stdin);
		parsedCommand = cParserPraseLine(command);
	}
	printf("Starting game...\n");
	return SP_GAME_STATUS_SUCCESS;

}

