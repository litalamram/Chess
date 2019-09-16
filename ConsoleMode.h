#ifndef CONSOLEMODE_H_
#define CONSOLEMODE_H_

#include "SPChessGame.h"
#include "Parser.h"


#define MAX_COMMAND_LENGTH 3000

/* Type used for returning status codes from ConsoleMode functions
 */
typedef enum sp_game_status_t {
	SP_GAME_STATUS_SUCCESS,
	SP_GAME_STATUS_QUIT,
	SP_GAME_STATUS_RESTART,
	SP_GAME_STATUS_MALLOC_ERR,
	SP_GAME_STATUS_ILLEGAL_COMMAND,
	SP_GAME_STATUS_IO_EXCEPTION
} SP_GAME_STATUS;

/**
 * Start a new game in console mode and play until the game ended(someone win / a draw)
 * If memory error appears or the player choose to quit or restart
 * return SP_GAME_STATUS_MALLOC_ERR, SP_GAME_STATUS_QUIT, SP_GAME_STATUS_RESTART respectively
 *
 * @return SP_GAME_STATUS_MALLOC_ERR, SP_GAME_STATUS_QUIT or SP_GAME_STATUS_RESTART
 */
SP_GAME_STATUS consoleMain();

/**Executes the command specified
 *
 * @param game - the source game
 * @param command - the spParserPraseLine of the command that the player choose
 * @param print - whether to print messages or not
 *
 * @return -
 * SP_GAME_STATUS_ILLEGAL_COMMAND ,if the command is illegal
 * SP_GAME_STATUS_QUIT ,if player choose to quit ,
 * SP_GAME_STATUS_RESTART ,if player choose to restart
 * SP_GAME_STATUS_SUCCESS ,otherwise */
SP_GAME_STATUS handlePlayerCommand(SPChessGame *game, CCommand command, bool print);

/**
 * Makes the computer turn,
 * prints "Computer: move [pawn|bishop|knight|rook|queen] at <x,y> to <i,j>".
 * where each move is represented by the original position <x,y>
 * and the destination <i,j>.
 *
 * @param game - the source game
 * @bool print - whether to print a message or not
 *
 * @return -
 * SP_GAME_STATUS_MALLOC_ERR if malloc has failed
 * SP_GAME_STATUS_SUCCESS otherwise
 */
SP_GAME_STATUS computerTurn(SPChessGame *game, bool print);

#endif /* CONSOLEMODE_H_ */
