/*
 * Parser.c
 *
 *  Created on: Mar 7, 2018
 *      Author: Netab
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include "Parser.h"

#define MOVE_TXT "move"
#define GET_MOVES_TXT "get_moves"
#define SAVE_TXT "save"
#define LOAD_TXT "load"
#define UNDO_TXT "undo"
#define QUIT_TXT "quit"
#define RESET_TXT "reset"
#define SET_MODE_TXT "game_mode"
#define SET_DIFF_TXT "difficulty"
#define SET_COLOR_TXT "user_color"
#define SET_DEFAULT_TXT "default"
#define PRINT_SETTING_TXT "print_settings"
#define START_TXT "start"

void cParseMoveCommand(CCommand* command, char* xy, char *to, char* ij);
void cParseGetMovesCommand(CCommand* command, char* xy);
bool cParserIsLetter(char c);
bool cParserIsInt(const char* str);
bool cParserWithoutArg(CCommand* newCommand);
void cParseOneInt(CCommand* newCommand, char* number);
bool cParserIsDigit(char c);
int cParserNumOfComma (char *str);
bool cParserHasBlankSpace (char *str);
bool cParseIsWellFormattedCoordinate (char *str);


/**
 * Checks if a specified string represents a valid one digit integer.
 *
 * @return
 * true if the string represents a valid integer, and false otherwise.
 */
bool cParserIsInt(const char* str){
	if(str == NULL || strlen(str)!=1 || str[0] < '0' || str[0] > '9'){
		return false;
	}
	return true;
}

/**
 * Checks if a specified char represents a valid one digit integer.
 *
 * @return
 * true if the char represents a valid integer, and false otherwise.
 */
bool cParserIsDigit(char c){
	if(c < '0' || c > '9'){
		return false;
	}
	return true;
}

/**Returns the number of commas (,) the specified str contains*/
int cParserNumOfComma (char *str){
	int cnt = 0;
	while (*str != '\0'){
		if (*str == ','){
			cnt++;
		}
		str++;
	}
	return cnt;
}

/**Returns if the specified string contains a blank space*/
bool cParserHasBlankSpace (char *str){
	while (*str != '\0'){
		if (isspace(*str)){
			return true;
		}
		str++;
	}
	return false;
}

/**Returns if the specifies string represents a well formatted coordinate */
bool cParseIsWellFormattedCoordinate (char *str){
	if (str == NULL || str[0]!='<' || str[strlen(str)-1]!='>' || cParserNumOfComma(str)!=1 || cParserHasBlankSpace(str)){
		return false;
	}
	return true;
}

/**Parses a move command .
 *
 * If the specified params are not valid parameters of a move command,
 * then command->cmd is set to INVALID_LINE.
 * If xy and ij are not of the form "<%c,%c>", then command->validArg is set to false.
 * Otherwise, command->validArg set to true, and command->args are set according to xy,ij.
 *
 * @param command - the command to be filled
 * @param xy - the first command parameter
 * @param to - the second command parameter
 * @param ij - the third command parameter
 *
 * */
void cParseMoveCommand(CCommand* command, char* xy, char* to, char* ij){
	//not a valid move command
	if (xy == NULL || to == NULL || ij == NULL){
		command->cmd = INVALID_LINE;
		return;
	}
	if (strcmp(to, "to")!=0 || !cParseIsWellFormattedCoordinate(xy) || !cParseIsWellFormattedCoordinate(ij)){
		command->cmd = INVALID_LINE;
		return;
	}
	//coordinates are'nt valid
	if (strlen(xy)!=5 || strlen(ij)!=5){
		command->validArg = false;
		return;
	}
	//parse the coordinates
	char x = xy[1];
	char y = xy[3];
	char i = ij[1];
	char j = ij[3];
	if (cParserIsDigit(x) && cParserIsDigit(i) && cParserIsLetter(y)&& cParserIsLetter(j)){
		command->validArg = true;
		command->args[0] = '8'-x;
		command->args[1] = y-'A';
		command->args[2] = '8'-i;
		command->args[3] = j-'A';
	}
	else {
		command->validArg = false;
	}
}

/**Parses a get_moves command.
 *
 * If the specified params are not valid parameters of a get_moves command,
 * then command->cmd is set to INVALID_LINE.
 * If xy is not of the form "<%c,%c>", then command->validArg is set to false.
 * Otherwise, command->validArg set to true, and command->args are set according to xy.
 *
 *
 * @param command - the command to be filled
 * @param xy - the first parameter of the command
 * */
void cParseGetMovesCommand(CCommand* command, char* xy){
	//not a valid move command
	if (xy == NULL || !cParseIsWellFormattedCoordinate(xy)){
		command->cmd = INVALID_LINE;
		return;
	}
	//coordinate isn't valid
	if (strlen(xy)!=5){
		command->validArg = false;
		return;
	}
	//parse the coordinate
	char x = xy[1];
	char y = xy[3];
	if (cParserIsDigit(x) && cParserIsLetter(y)){
		command->validArg = true;
		command->args[0] = '8'-x;
		command->args[1] = y-'A';
	}
	else {
		command->validArg = false;
	}
}

/**
 * Checks if a specified char represents a valid Capital Letter.
 *
 * @return
 * true if the char represents a valid Capital Letter, and false otherwise.
 */
bool cParserIsLetter(char c){
	if (c >= 'A' && c <= 'Z'){
		return 1;
	}
	return 0;
}

/** Parse a number argument.
 *
 * If the specified number represents a valid one digit integer,
 * then command->arg is set to the actual number and command->validArg is set to true.
 * Otherwise, doesn't chang the command
 *
 * @param command - the command to be filled
 * @param number - the number represntation
 *
 * */
void cParseOneInt(CCommand* command, char* number){
	if (cParserIsInt(number)){   // the argument is a valid int
		command->arg = atoi(number);
		command->validArg = true;
	}
}

/**
 * Checks if a specified string is a command that doesn't need arguments.
 *
 * @return
 * true if the string is a command that not need arguments, and false otherwise.
 */
bool cParserWithoutArg(CCommand* command){
	if (command->cmd == QUIT){
		return true;
	}
	else if(command->cmd == SET_DEFAULT){
		return true;
	}
	else if(command->cmd == PRINT_SETTING){
		return true;
	}
	else if(command->cmd == START){
		return true;
	}
	else if(command->cmd == UNDO){
		return true;
	}
	else if(command->cmd == RESET){
		return true;
	}
	else if(command->cmd == INVALID_LINE){
		return true;
	}
	return false;
}

/**
 * Parses a specified string to a command type.
 *
 * @return
 * the command type if the string is valid , and INVALID_LINE if it's invalid
 */
COMMAND cParserParseCommand(const char* str){

	if (strcmp(str, MOVE_TXT)==0){
		return MOVE;
	}
	if (strcmp(str, UNDO_TXT)==0){
		return UNDO;
	}
	if (strcmp(str, GET_MOVES_TXT)==0){
		return GET_MOVES;
	}
	if (strcmp(str, QUIT_TXT)==0){
		return QUIT;
	}
	if (strcmp(str, RESET_TXT)==0){
		return RESET;
	}
	if (strcmp(str, SAVE_TXT)==0){
		return SAVE;
	}
	if (strcmp(str, LOAD_TXT)==0){
		return LOAD;
	}
	if (strcmp(str, SET_MODE_TXT)==0){
		return SET_MODE;
	}
	if (strcmp(str, SET_DIFF_TXT)==0){
		return SET_DIFF;
	}
	if (strcmp(str, SET_COLOR_TXT)==0){
		return SET_COLOR;
	}
	if (strcmp(str, SET_DEFAULT_TXT)==0){
		return SET_DEFAULT;
	}
	if (strcmp(str, PRINT_SETTING_TXT)==0){
		return PRINT_SETTING;
	}
	if (strcmp(str, START_TXT)==0){
		return START;
	}

	return INVALID_LINE;
}

/**
 * Parses a specified line.
 *
 * If the line is a command which has an integer argument,
 * then the argument is parsed and is saved in the field arg,
 * and the field validArg is set to true.
 *
 * If the line is Move command with 4 valid integers
 * the arguments are saved in the field args and validArg is set to true.
 *
 * In any other case then 'validArg' is set to false and the value 'arg'
 * is undefined
 *
 * @return
 * A parsed line such that:
 *   cmd - contains the command type, if the line is invalid then this field is
 *         set to INVALID_LINE
 *   validArg - is set to true if the command contains integer argument and the
 *              integer argument is valid
 *   arg      - the integer argument if the command has only one integer
 *              and validArg is set to true
 *   args     - the integers arguments in case of move command and validArg is set to true
 */
CCommand cParserPraseLine(const char* str){
	CCommand command;
	command.validArg = false;
	if(str == NULL){
		command.cmd = INVALID_LINE;
		return command;
	}
	char delimiter[] = " \t\r\n";
	char copy[SP_MAX_LINE_LENGTH];
	strcpy(copy, str);
	char *token = strtok(copy, delimiter);	//get first token

	if(token == NULL){
		command.cmd = INVALID_LINE;
		return command;
	}
	command.cmd = cParserParseCommand(token);

	if (!cParserWithoutArg(&command)){ //the command has an argument
		if (command.cmd == SAVE || command.cmd == LOAD){
			token = strtok(NULL, delimiter);  //get next token
			if (token != NULL){
				command.validArg = true;
				strcpy(command.path, token);
			}
			else {
				command.cmd = INVALID_LINE;
			}
		}
		else if (command.cmd != MOVE && command.cmd != GET_MOVES){ // not move or get_moves commands
			char* number = strtok(NULL, delimiter);  //get next token
			cParseOneInt(&command, number);
		}
		else if (command.cmd == MOVE){
			char* xy = strtok(NULL, delimiter);   //get next token
			char* to = strtok(NULL, delimiter);	  //get next token
			char* ij = strtok(NULL, delimiter);   //get next token
			cParseMoveCommand(&command, xy, to, ij);
		}
		else if (command.cmd == GET_MOVES){
			char* xy = strtok(NULL, delimiter);   //get next token
			cParseGetMovesCommand(&command, xy);
		}
		else{
			command.cmd = INVALID_LINE;
		}
	}
	return command;
}

