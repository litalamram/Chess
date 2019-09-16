#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include "Parser.h"
#include "SPArrayList.h"
#include "SPChessGame.h"
#include "Settings.h"
#include "SPChessGameSettings.h"
#include "SPMinimax.h"
#include "ConsoleMode.h"

#define HISTORY_SIZE 6

/********************Prototypes**************************/

void printFuncErr(char *funcName);
void printRestart();
void printExit();
void printMoveRequest(char *player);
void printUndoErr();
void printInvalidCommandErr();
void printUndoMovePlayer(char *player, SPCoordinate c1, SPCoordinate c2);
void printComputerMove(char* piece, SPCoordinate c1, SPCoordinate c2);
void printStatus (SP_GAME_STATUS status);
CCommand getCommandFromUser();
void printCurrentPlayerInCheck (SPChessGame *game);
SP_CHESS_GAME_MESSAGE handleMoveCommand(SPChessGame *game, CCommand command);
void printGetMoves (SPMove *moves, int len);
SP_GAME_STATUS handleGetMovesCommand(SPChessGame *game, CCommand command);
SP_GAME_STATUS handleUndoCommand(SPChessGame *game, bool print);
SP_GAME_STATUS handleSaveCommand(SPChessGame *src, CCommand comm, bool print);
SP_GAME_STATUS playerTurn(SPChessGame *game);
void printGameOver(int winner);
SP_GAME_STATUS play(SPChessGame *game);

/*******************Implementation***********************/

void printFuncErr(char *funcName){
	printf("Error: %s has failed\n",funcName);
}

void printRestart(){
	printf("Restarting...\n");
}

void printExit(){
	printf("Exiting...\n");
}

void printMoveRequest(char *player){
	printf("Enter your move (%s player):\n",player);
}

void printUndoErr(){
	printf("Empty history, no move to undo\n");
}

void printInvalidCommandErr(){
	printf("Error: invalid command\n");
}

void printUndoMovePlayer(char *player, SPCoordinate c1, SPCoordinate c2){
	char s1[6], s2[6];
	spChessGameParseCoordinate(c1, s1);
	spChessGameParseCoordinate(c2, s2);
	printf("Undo move for %s player: %s -> %s\n", player, s1, s2);
}

void printComputerMove(char* piece, SPCoordinate c1, SPCoordinate c2){
	char s1[6], s2[6];
	spChessGameParseCoordinate(c1, s1);
	spChessGameParseCoordinate(c2, s2);
	printf("Computer: move %s at %s to %s\n",piece, s1, s2);
}

void printStatus (SP_GAME_STATUS status){
	if (status == SP_GAME_STATUS_MALLOC_ERR){
		printFuncErr("malloc");
	}
	else if (status == SP_GAME_STATUS_QUIT){
		printExit();
	}
	else if (status == SP_GAME_STATUS_RESTART){
		printRestart();
	}
}


/**Gets input from user and parses it into SPCommand
 *
 * @return the parsed command*/
CCommand getCommandFromUser(){
	char command[MAX_COMMAND_LENGTH+1];
	fgets(command,MAX_COMMAND_LENGTH,stdin);
	return cParserPraseLine(command);
}

/** Prints "Check: XYZ king is threatened\n"
 *  where XYZ is the color of the current player: either 'black' or 'white'.
 *
 * @param game - the source game
 */
void printCurrentPlayerInCheck (SPChessGame *game){
	int player = spChessGameGetCurrentPlayer(game);
	printf("Check: %s king is threatened\n",parseColor(player));
}

/**
 * Applies the move specified in the command if it is a valid one,
 * Otherwise doesn't change the game.
 *
 * @param game - the source game
 * @param command - the spParserPraseLine of the command that the player choose
 *
 * print "Invalid position on the board" - If either one of the locations is invalid
 * print "The specified position does not contain your piece" - If start position  does not
 * 																contain a piece of the user's color
 * print "Illegal move" - If the move is illegal for the piece in the start position
 * print "Illegal move: king is still threatened" - if the king is threatened ("Check")
 * 													and will still be threatened after the move
 * print "Ilegal move: king will be threatened" - if the move causes the player's king to be threatened
 *
 * @return
 * SP_GAME_STATUS_SUCCESS - if the move is valid
 * SP_GAME_STATUS_ILLEGAL_COMMAND - otherwise
 *
 */
SP_CHESS_GAME_MESSAGE handleMoveCommand(SPChessGame *game, CCommand command){
	if (command.validArg){
		SPMove move;
		move.start.row = command.args[0];
		move.start.col = command.args[1];
		move.dest.row = command.args[2];
		move.dest.col = command.args[3];

		SP_CHESS_GAME_MESSAGE status = spChessGameisValidMove(game,move);
		if (status == SP_CHESS_GAME_LEGAL_MOVE){
			spChessGameSetMove(game,move);
			return SP_GAME_STATUS_SUCCESS;
		}
		else if (status == SP_CHESS_GAME_INVALID_ARGUMENT){
			printf("Invalid position on the board\n");
		}
		else if(status == SP_CHESS_GAME_NOT_YOUR_PIECE){
			printf("The specified position does not contain your piece\n");
		}
		else if(status == SP_CHESS_GAME_INVALID_MOVE){
			printf("Illegal move\n");
		}
		else if(status == SP_CHESS_GAME_KING_WILL_BE_THRETHEND){
			if (spChessGameIsPlayerInCheck(game, spChessGameGetCurrentPlayer(game))){
				printf("Illegal move: king is still threatened\n");
			}
			else {
				printf("Illegal move: king will be threatened\n");
			}
		}
	}
	else {
		printf("Invalid position on the board\n");
	}
	return SP_GAME_STATUS_ILLEGAL_COMMAND;
}

/** Prints the moves array was specified
 * Each move is printed as "<x,y>\n".
 * If the move is threatened by an opponent: "<x,y>*"
 * If the move captures a piece: "<x,y>^"
 * If the move satisfies both: "<x,y>*^".
 *
 * @param moves - array of available moves
 * @param len - the array length
 */
void printGetMoves (SPMove *moves, int len){
	for (int i=0;i<len;i++){
		spChessGamePrintCoord(moves[i].dest);
		if (moves[i].isThreatened){
			printf("*");
		}
		if(moves[i].doesCapturePiece){
			printf("^");
		}
		printf("\n");
	}
}

/** Prints all possible moves of the piece located at the specified location.
 *
 * @param game - the source game
 * @param command - the spParserPraseLine of the command that the player choose
 *
 * prints "Invalid position on the board" - if the specified location is invalid
 * prints "The specified position does not
 * 			 contain a player piece" - if the specified location doesn't contain a piece
 * otherwise, each possible move is printed as in printGetMoves function.
 */
SP_GAME_STATUS handleGetMovesCommand(SPChessGame *game, CCommand command){
	if (command.validArg){
		SPCoordinate coord = {.row = command.args[0],.col = command.args[1]};
		SPMove moves[SP_CHESS_GAME_MAX_NUMBER_OF_MOVES];
		int size;
		SP_CHESS_GAME_MESSAGE status = spChessGameGetMoves(game, coord, moves, &size, true);
		if (status == SP_CHESS_GAME_SUCCESS){
			printGetMoves(moves,size);
			return SP_GAME_STATUS_SUCCESS;
		}
		else if (status == SP_CHESS_GAME_INVALID_ARGUMENT){
			printf("Invalid position on the board\n");
		}
		else if(status == SP_CHESS_GAME_POSITION_NOT_CONTAIN_PIECE){
			printf("The specified position does not contain a player piece\n");
		}

	}
	else {
		printf("Invalid position on the board\n");
	}
	return SP_GAME_STATUS_ILLEGAL_COMMAND;
}

/**
 * If has 2 valid moves in the history, undoes the 2 recent moves
 * and prints "Undo move for XYZ player: <x,y> -> <w,z>",
 * where XYZ, <x,y> and <w,z> represents the player color ('black' or 'white'),
 * the position of the piece after the last move executed
 * and its original position, respectively
 *
 * If there is only one move to undo,
 * then the program will undo the previous move only.
 *
 * If doesn't have any moves in the history, doesn't change the game
 * and print "Empty history, no move to undo"
 *
 * @param game - the source game
 * @bool print - whether to print a message or not
 *
 * @return
 * SP_GAME_STATUS_SUCCESS if can undo
 * SP_GAME_STATUS_ILLEGAL_COMMAND otherwise
 */
SP_GAME_STATUS handleUndoCommand(SPChessGame *game, bool print){
	SPArrayListNode move1, move2;
	char* player1, *player2;
	int historySize = spArrayListSize(game->recentMoves);

	if (historySize >= 2) { //undo two moves
		move1 = spArrayListGetLast(game->recentMoves);
		spChessGameUndoPrevMove(game);
		player1 = parseColor(spChessGameGetCurrentPlayer(game));

		move2 = spArrayListGetLast(game->recentMoves);
		spChessGameUndoPrevMove(game);
		player2 = parseColor(spChessGameGetCurrentPlayer(game));

		if (print) {
			printUndoMovePlayer(player1, (SPCoordinate){move1.dRow, move1.dCol}, (SPCoordinate){move1.sRow, move1.sCol});
			printUndoMovePlayer(player2, (SPCoordinate){move2.dRow, move2.dCol}, (SPCoordinate){move2.sRow, move2.sCol});
			spChessGamePrintBoardToFile(stdout,game);
		}
	}
	else if (historySize == 1){ //undo last move
		move1 = spArrayListGetLast(game->recentMoves);
		spChessGameUndoPrevMove(game);
		player1 = parseColor(spChessGameGetCurrentPlayer(game));

		if (print) {
			printUndoMovePlayer(player1, (SPCoordinate){move1.dRow, move1.dCol}, (SPCoordinate){move1.sRow, move1.sCol});
			if (game->game_mode == 1){ //print board
				spChessGamePrintBoardToFile(stdout,game);
			}
		}
	}
	else { //no history
		if(print)
			printUndoErr();
		return SP_GAME_STATUS_ILLEGAL_COMMAND;
	}


	return SP_GAME_STATUS_SUCCESS;
}

/** Saves the current game state to the specified file
 *
 * @param src - the game source
 * @param command - the spParserPraseLine of the command that the player choose
 * @bool print - whether to print a message or not
 * print "Error: File cannot be created or modified" - if an IO exception occurred
 * print "Game saved to: XYZ",
 * 		  where XYZ is the filename provided by the user.
 * 		  if no error occurred
 *
 *
 * @return SP_GAME_STATUS_SUCCESS
 */
SP_GAME_STATUS handleSaveCommand(SPChessGame *src, CCommand comm, bool print){
	if (comm.validArg ){
		SP_CHESS_GAME_MESSAGE status = spChessGameSettingsSave(src,comm.path);
		if (status == SP_CHESS_GAME_IO_EXCEPTION){
			if (print)
				printf("File cannot be created or modified\n");
			return SP_GAME_STATUS_IO_EXCEPTION;
		}
		else {
			if (print)
				printf("Game saved to: %s\n",comm.path);
		}
	}
	else {
		if (print)
			printf("File cannot be created or modified\n");
		return SP_GAME_STATUS_IO_EXCEPTION;
	}
	return SP_GAME_STATUS_SUCCESS;
}

/** Restarts the game with the current game settings
 *
 * @param src - the game source
 * @bool print - whether to print a message or not
 * print "Restarting..."
 *
 * @return SP_GAME_STATUS_RESTART
 */
SP_GAME_STATUS handleRestartCommand (SPChessGame *src, bool print){
	if (print)
		printStatus(SP_GAME_STATUS_RESTART);
	spChessGameRestartGame(src);
	return SP_GAME_STATUS_RESTART;
}

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
SP_GAME_STATUS handlePlayerCommand(SPChessGame *game, CCommand command, bool print){
	if (command.cmd == MOVE){
		return handleMoveCommand(game, command);
	}
	else if(command.cmd == UNDO){
		return handleUndoCommand(game, print);
	}
	else if(command.cmd == QUIT){
		return SP_GAME_STATUS_QUIT;
	}
	else if (command.cmd == RESET){
		return handleRestartCommand(game, print);//SP_GAME_STATUS_RESTART;
	}
	else if (command.cmd == GET_MOVES){
		return handleGetMovesCommand(game, command);
	}
	else if (command.cmd == SAVE){
		return handleSaveCommand(game, command, print);
	}
	if (print){
		printInvalidCommandErr();
	}
	return SP_GAME_STATUS_ILLEGAL_COMMAND;
}

/**
 * Check if the player choose valid command, if not print the fit error massage
   and check the new command, when the player choose a valid one, executes it.
 * The player turn ends when he chooses a valid move , or when he chooses to quit or reset the game.
 *
 * @param game - the source game
 *
 * @return -
 * SP_GAME_STATUS_QUIT ,if choose to quit,
 * SP_GAME_STATUS_RESTART ,if player choose to restart
 * SP_GAME_STATUS_SUCCESS ,otherwise
 */
SP_GAME_STATUS playerTurn(SPChessGame *game){
	CCommand command;
	SP_GAME_STATUS status;
	int player = game->currentPlayer;
	spChessGamePrintBoardToFile(stdout,game);
	do {
		printMoveRequest(parseColor(game->currentPlayer));
		command = getCommandFromUser();
		status = handlePlayerCommand(game, command ,true);
	} while (player == game->currentPlayer && status!=SP_GAME_STATUS_QUIT && status!=SP_GAME_STATUS_RESTART);
	return status;
}

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
SP_GAME_STATUS computerTurn(SPChessGame *game, bool print){

	SPMove move;
	move = spMinimaxSuggestMove(game, game->difficulty);
	if (move.start.row == -1){
		return SP_GAME_STATUS_MALLOC_ERR;
	}

	spChessGameSetMove(game, move);
	if(print)
		printComputerMove(spChessGameGetPieceName(spChessGameGetPieceAt(game,move.dest.row, move.dest.col)), move.start, move.dest);

	return SP_GAME_STATUS_SUCCESS;
}

/** Prints a message when the game is over
 *
 * @param winner - the winner of the game (SP_CHESS_GAME_BLACK - if black player wins
 * 										   SP_CHESS_GAME_WHITE - if white player wins
 * 										   SP_CHESS_GAME_DRAW - if the game ends in draw)
 *
 * Prints "Checkmate! XYZ player wins the game" where XYZ is "black" or "white" -
 * 		  if the black/ white player wins the game
 * Prints "The game ends in a draw" - otherwise
 */
void printGameOver(int winner){

	if (winner == SP_CHESS_GAME_BLACK || winner == SP_CHESS_GAME_WHITE){
		printf("Checkmate! %s player wins the game\n",parseColor(winner));
	}
	else{
		printf("The game ends in a draw\n");
	}
}

/**
 * Checks who's turn is and play this turn
 *
 * @param game - the source game, no one win and the board game not full
 *
 * @return -
 * SP_GAME_STATUS_QUIT ,if player choose to quit ,
 * SP_GAME_STATUS_RESTART ,if player choose to restart
 * SP_GAME_STATUS_MALLOC_ERR, if malloc has failed
 * SP_GAME_STATUS_SUCCESS ,otherwise
 */
SP_GAME_STATUS play(SPChessGame *game){
	if (game->game_mode == 1){ //1-Player mode
		if(spChessGameGetCurrentPlayer(game) == game->userColor){	//player turn
			return playerTurn(game);
		}
		return computerTurn(game, true);	//computer turn
	}
	//2-Player mode
	return playerTurn(game);
}

/**
 * Enables the user to quit, reset, or save the game
 * in case an allocation memory error occurred during the computer turn
 * @param game - the source game
 *
 * @return -
 * SP_GAME_STATUS_QUIT ,if choose to quit,
 * SP_GAME_STATUS_RESTART ,if player choose to restart
 */
SP_GAME_STATUS getUserCommandAfterError(SPChessGame *game){
	CCommand command;
	SP_GAME_STATUS status;
	do {
		printf("Do you want to quit, reset or save the game?\n");
		command = getCommandFromUser();
		if (command.cmd != QUIT && command.cmd != RESET && command.cmd != SAVE){
			//other commands are disabled after an error  has occurred
			status = SP_GAME_STATUS_ILLEGAL_COMMAND;
		}
		else {
			status = handlePlayerCommand(game, command, true);
		}
	} while (status!=SP_GAME_STATUS_QUIT && status!=SP_GAME_STATUS_RESTART);
	return status;
}

/**
 * Start a new game and play until the game ended(someone win / a draw)
 * If memory error appears or the player choose to quit or restart
 * return SP_GAME_STATUS_MALLOC_ERR, SP_GAME_STATUS_QUIT, SP_GAME_STATUS_RESTART respectively
 *
 * @return SP_GAME_STATUS_MALLOC_ERR, SP_GAME_STATUS_QUIT or SP_GAME_STATUS_RESTART
 */
SP_GAME_STATUS consoleMain(){
	int gameState;
	SP_GAME_STATUS status;

	//init new game
	SPChessGame *game = spChessGameCreate(HISTORY_SIZE);
	if (game == NULL){
		printf("ERROR: failed allocate memory for game\n");
		return SP_GAME_STATUS_MALLOC_ERR;
	}

	printf(" Chess\n");
	printf("-------\n");

	status = getSettingsFromUser(game); //get to settings state
	gameState = spChessGameGetState(game); //get the current game state

	while ((gameState == SP_CHESS_GAME_NOT_OVER || gameState == SP_CHESS_GAME_CHECK) && status != SP_GAME_STATUS_QUIT) {

		if (gameState == SP_CHESS_GAME_CHECK){ //current player in check
			printCurrentPlayerInCheck(game);
		}

		status = play(game); //play the current turn

		if (status == SP_GAME_STATUS_MALLOC_ERR){ //computer turn has failed
			printf("ERROR: failed allocate memory to compute computer move\n");
			status = getUserCommandAfterError(game);
		}

		if (status == SP_GAME_STATUS_RESTART){ //get beck to settings state
			//switch to settings state
			status = getSettingsFromUser(game);
		}

		gameState = spChessGameGetState(game); //get the current game state
	}

	//checkmate/draw
	if (gameState == SP_CHESS_GAME_WHITE || gameState == SP_CHESS_GAME_BLACK || gameState == SP_CHESS_GAME_DRAW){
		printGameOver(gameState);
	}
	//user requested to quit
	else {
		printStatus(status);
	}
	//free resources
	spChessGameDestroy(game);
	return status;
}
