/*
 * Parser.h
 *
 *  Created on: Mar 7, 2018
 *      Author: Netab
 */

#ifndef PARSER_H_
#define PARSER_H_

#include <stdbool.h>

//specify the maximum line length
#define SP_MAX_LINE_LENGTH 3000
//specify the number of arguments a command can have
#define SP_NUM_COMMANDS_ARGS 4

//a type used to represent a command
typedef enum {
	MOVE,
	GET_MOVES,
	SAVE,
	LOAD,
	UNDO,
	RESET,
	QUIT,
	INVALID_LINE,

	SET_MODE,
	SET_DIFF,
	SET_COLOR,
	SET_DEFAULT,
	PRINT_SETTING,
	START

} COMMAND;

//a new type that is used to encapsulate a parsed line
typedef struct command_t {
	COMMAND cmd;
	bool validArg; //is set to true if the line contains a valid argument
	int arg;
	int args[SP_NUM_COMMANDS_ARGS];
	char path[SP_MAX_LINE_LENGTH];
} CCommand;


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
CCommand cParserPraseLine(const char* str);

#endif /* PARSER_H_ */
