/*
 * Settings.h
 *
 *  Created on: 14 áîøñ 2018
 *      Author: lital
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_

#include "SPChessGame.h"
#include "Parser.h"
#include "ConsoleMode.h"

/**  Repeatedly gets and handles user commands at the settings state of the game
 *
 *   @param src - the game source
 *
 *   @return
 *   SP_GAME_STATUS_SUCCESS - if the user enters the start command
 *   SP_GAME_STATUS_QUIT - if the user enters the quit command
 */
SP_GAME_STATUS getSettingsFromUser(SPChessGame *src);

/** Loads the game setting from the specified file
 *
 * @param src - the game source
 * @param command - the spParserPraseLine of the command that the player choose
 * @bool print - whether to print a message or not
 *
 * print "Error: File doesn’t exist or cannot be opened" -
 * 		  if the file does not exist or cannot be opened
 */
SP_GAME_STATUS handleLoadCommand (SPChessGame *src, CCommand command, bool print);

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
void handleGameModeCommand (SPChessGame *src, CCommand command, bool print);

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
void handleDifficultyCommand (SPChessGame *src, CCommand command, bool print);

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
void handleUserColorCommand (SPChessGame *src, CCommand command, bool print);

#endif /* SETTINGS_H_ */
