#ifndef SPCHESSGAME_H_
#define SPCHESSGAME_H_
#include <stdbool.h>
#include <stdio.h>
#include "SPArrayList.h"

/**
 * SPCHESSGame Summary:
 *
 * A container that represents a chess game, a two players 8 by 8
 * board game (rows X columns). The container supports the following functions.
 *
 * spChessGameCreate           - Creates a new game board
 * spChessGameCopy             - Copies a game board
 * spChessGameDestroy          - Frees all memory resources associated with a game
 * spChessGameSetMove          - Sets a move on a game board
 * spChessGameIsValidMove      - Checks if a move is valid
 * spChessGameUndoPrevMove     - Undoes previous move made by the last player
 * spChessGamePrintBoard       - Prints the current board
 * spChessGameGetCurrentPlayer - Returns the current player
 *
 */

//Definitions
#define SP_CHESS_GAME_N_ROWS 8
#define SP_CHESS_GAME_N_COLUMNS 8
#define SP_CHESS_GAME_EMPTY_ENTRY '_'
#define SP_CHESS_GAME_MAX_NUMBER_OF_MOVES 64

#define SP_CHESS_GAME_MAX_SCORE 1000
#define SP_CHESS_GAME_MIN_SCORE -1000
#define SP_CHESS_GAME_DRAW_SCORE 0

#define SP_CHESS_GAME_BLACK 0
#define SP_CHESS_GAME_WHITE 1
#define SP_CHESS_GAME_DRAW 2
#define SP_CHESS_GAME_CHECK 3
#define SP_CHESS_GAME_NOT_OVER -1

/**
 * Type used represent a coordinate in the game board
 */
typedef struct sp_chess_game_array_coordinate_t{
	int row, col;
} SPCoordinate;

/**
 * Type used to represent a move in the game
 */
typedef struct sp_move_t {
	SPCoordinate start, dest;
	bool isThreatened, doesCapturePiece;
} SPMove;

/**
 * Represents type of a piece in the game
 */
typedef enum SPPieceType {
	PAWN,
	BISHOP,
	ROOK,
	KNIGHT,
	QUEEN,
	KING,
	BLANK
} SPPieceType;

/**
 * Type used to represent a game
 */
typedef struct sp_chess_game_t {
	char gameBoard[SP_CHESS_GAME_N_ROWS][SP_CHESS_GAME_N_COLUMNS];
	int currentPlayer ;
	int game_mode ;
	int difficulty ;
	int userColor ;
	SPArrayList* recentMoves;
	int isSaved;
	bool isOver;
	int (*scoreFunc)(void*);
} SPChessGame;


/**
 * Type used for returning error codes from game functions
 */
typedef enum sp_chess_game_message_t {
	SP_CHESS_GAME_KING_STILL_THRETHEND,
	SP_CHESS_GAME_KING_WILL_BE_THRETHEND,
	SP_CHESS_GAME_CAPTURE_AND_THERTHENED,
	SP_CHESS_GAME_LEGAL_MOVE,
	SP_CHESS_GAME_INVALID_MOVE,
	SP_CHESS_GAME_NOT_YOUR_PIECE,
	SP_CHESS_GAME_POSITION_NOT_CONTAIN_PIECE,
	SP_CHESS_GAME_INVALID_ARGUMENT,
	SP_CHESS_GAME_NO_HISTORY,
	SP_CHESS_GAME_SUCCESS,
	SP_CHESS_GAME_IO_EXCEPTION
} SP_CHESS_GAME_MESSAGE;

/** Parses and prints the specified coordinate in format "<'8'-coord.row, 'A'+coord.col>"
 * @param coord - the coordinate should be printed
 */
void spChessGamePrintCoord (SPCoordinate coord);

/** Parses an array coordinate into a board coordinate
 * @param coord - the coordinate to be parsed
 * @param res - gets the parsed coordinate in format "<'8'-coord.row, 'A'+coord.col>"
 *
 */
void spChessGameParseCoordinate (SPCoordinate coord, char *res);

/**
 * Creates a new game with a specified history size. The history size is a
 * parameter which specifies the number of previous moves to store. If the number
 * of moves played so far exceeds this parameter, then first moves stored will
 * be discarded in order for new moves to be stored.
 *
 * @historySize - The total number of moves to undo,
 *                a player can undo at most historySizeMoves turns.
 * @return
 * NULL if either a memory allocation failure occurs or historySize <= 0.
 * Otherwise, a new game instant is returned.
 */
SPChessGame* spChessGameCreate(int historySize);

/**
 *	Creates a copy of a given game.
 *	The new copy has the same status as the src game.
 *
 *	@param src - the source game which will be copied
 *	@return
 *	NULL if either src is NULL or a memory allocation failure occurred.
 *	Otherwise, an new copy of the source game is returned.
 *
 */
SPChessGame* spChessGameCopy(SPChessGame* src);

/**
 * Frees all memory allocation associated with a given game. If src==NULL
 * the function does nothing.
 *
 * @param src - the source game
 */
void spChessGameDestroy(SPChessGame* src);

/**
 * On success, the function prints the board game to the specified file. If an error occurs, then the
 * function does nothing.
 *
 * @param src - the target game
 * @param f   - the destination file
 * @return
 * SP_CHESS_GAME_INVALID_ARGUMENT - if (src == NULL || f == NULL)
 * SP_CHESS_GAME_SUCCESS - otherwise
 *
 */
SP_CHESS_GAME_MESSAGE spChessGamePrintBoardToFile(FILE *f, SPChessGame* src);

/**
 * Checks if the specified move is legal according to the chess rules.
 *
 * @param src - The source game
 * @param move - The specified move
 * @return
 * SP_CHESS_GAME_INVALID_ARGUMENT - if either one of the locations move.start, move.dest is invalid
 * SP_CHESS_GAME_NOT_YOUR_PIECE  - if position move.start does not contain a piece of the current player's color
 * SP_CHESS_GAME_INVALID_MOVE - if the move is illegal for the piece in the position move.start
 * SP_CHESS_GAME_KING_WILL_BE_THRETHEND - if the move causes the player's king to be threatened
 * SP_CHESS_GAME_LEGAL_MOVE - otherwise.
 */
SP_CHESS_GAME_MESSAGE spChessGameisValidMove(SPChessGame* src, SPMove move);

/**
 * Sets the next move in a given game by specifying
 * a move in the form {{start_row, start_col},{dest_row, dest_col}}
 *
 * @pre spChessGameisValidMove(src, move) == SP_CHESS_GAME_LEGAL_MOVE
 * @param src - The target game
 * @param move - The move to be applied. The row and columns indices are 0-based .
 * @return
 * SP_CHESS_GAME_INVALID_ARGUMENT - if src is NULL
 * SP_CHESS_GAME_SUCCESS - otherwise
 */
SP_CHESS_GAME_MESSAGE spChessGameSetMove(SPChessGame* src, SPMove move);

/**
 * Undoes the last move has been made and changes the current
 * player's turn. If the user invoked this command more than historySize times
 * in a row then an error occurs.
 *
 * @param src - The source game
 * @return
 * SP_CHESS_GAME_INVALID_ARGUMENT - if src == NULL
 * SP_CHESS_GAME_NO_HISTORY       - if the user invoked this function more then historySize in a row.
 * SP_CHESS_GAME_SUCCESS          - on success
 */
SP_CHESS_GAME_MESSAGE spChessGameUndoPrevMove(SPChessGame* src) ;

/** Undoes the specified move on the src game:
 * the piece at location move.dest is being moved to location move.start
 * and capturedPiece is being set at location move.dest
 *
 * @pre spChessGameisValidMove(src, move) == SP_CHESS_GAME_LEGAL_MOVE
 * @param src - the source game
 * @param move - the move to be undo
 */
void undoMove (SPChessGame* src, SPMove move, char capturedPiece);

/** Returns whether the specified player is in Check
 * (if his king is threatened by an enemy piece)
 *
 * @param src - the fame source
 * @param playerColor - the color of the player (SP_CHESS_GAME_WHITE \ SP_CHESS_GAME_BLACK)
 *
 * @return
 * true - if the specified player is in Check
 * false - otherwise
 */
bool spChessGameIsPlayerInCheck(SPChessGame *src, int playerColor);

/**
 * Checks if the specified game is over by checkmate or draw, or if the current player is in check.
 * The function returns either SP_CHESS_GAME_BLACK or SP_CHESS_GAME_WHITE in case there's a winner,
 * where the value returned is the color of the winner.
 * If the game is over and there's a DRAW then the value SP_CHESS_GAME_DRAW is returned.
 * If the current player is in check, then SP_CHESS_GAME_CHECK is returned.
 * in any other case SP_CHESS_GAME_NOT_OVER
 * is returned.
 * @param src - the source game
 * @return
 * SP_CHESS_GAME_BLACK - if the black player won (white is in checkmate)
 * SP_CHESS_GAME_WHITE - if the white player won (black is in checkmate)
 * SP_CHESS_GAME_DRAW  - if the current player doesn’t have any legal moves,
 * 						 but the king is not threatened by the opponent
 * SP_CHESS_GAME_CHECK - if the current player is in check
 * SP_CHESS_GAME_NOT_OVER - otherwise
 */
int spChessGameGetState(SPChessGame *src);

/** All the legal moves of the piece at the specified location are inserted to the given array.
 * if flags==true, then for every move in the array the fields isThreatened and doesCapturePiece
 * are set to true\false according to whether the move is threatened by an enemy piece
 * and whether it captures a piece.
 *
 * @pre @moves contains enough space to contain all the available moves
 *
 * @param src - the source game
 * @param coord - the piece location
 * @param *moves - pointer to the resulted array
 * @param *size - pointer to get the size of moves array
 * @param flags - whether to check if the move captures piece and if it is threatened by the enemy
 *
 * @return
 * SP_CHESS_GAME_INVALID_ARGUMENT - if the specified location is not a valid coordinate in the board
 * SP_CHESS_GAME_POSITION_NOT_CONTAIN_PIECE - if the specified location doesn't contain a piece
 * SP_CHESS_GAME_SUCCESS - otherwise
 */
SP_CHESS_GAME_MESSAGE spChessGameGetMoves(SPChessGame* src, SPCoordinate coord, SPMove *moves, int *size, bool flags) ;

/** Returns the piece at the specified location on the board
 *
 * @pre src!=NULL, row,col are legal
 * @param src - the game dource
 * @param row, col - the row,column on the board
 * @return
 * The piece at location *row,col)
 */
char spChessGameGetPieceAt (SPChessGame *src, int row, int col);

/** Returns the color of the given piece
 *
 * @param piece
 * @return
 * SP_CHESS_GAME_WHITE - if the piece's color is white
 * SP_CHESS_GAME_BLACK - if the piece's color is black
 * 2 - if piece == SP_CHESS_GAME_EMPTY_ENTRY
 */
int spChessGameGetPieceColor (char piece);

/** Returns a string representation of the given piece
 * @param piece
 * @return
 * The name of the piece
 */
char* spChessGameGetPieceName (char piece);

/**
 * Returns the current player of the specified game.
 * @param src - the source game
 * @return
 * SP_CHESS_GAME_BLACK - if it's black player's turn
 * SP_CHESS_GAME_WHITE - if it's white's turn
 * SP_CHESS_GAME_EMPTY_ENTRY     - otherwise
 */
int spChessGameGetCurrentPlayer(SPChessGame* src) ;

/** Changes the current player
 *
 * @param src - the game source
 */
void spChessGameChangePlayer (SPChessGame* src);

/** Returns the type of the specified piece
 * @param piece
 * @return the piec's type
 */
SPPieceType spChessGameGetPieceType (char piece);

/** Restarts the game with the current game settings
 *
 * @param src - the game src
 * */
void spChessGameRestartGame (SPChessGame* src);

/** Deletes moves history
 *
 * @param src - the src game
 * */
void spChessGameCleanHistory (SPChessGame* src);

/** Parses the specified row
 * @param row - the row to be parsed
 * @return
 * '8'-row
 */
int spChessGameParseRow (char row);

/** Parses the specified column
 * @param row - the row to be parsed
 * @return
 * col-'A'
 */
int spChessGameParseColumn (char col);

/**Returns if the specified player has any valid moves
 *
 * @param src - the game src
 * @param playerColor - the color of player (SP_CHESS_GAME_BLACK \ SP_CHESS_GAME_WHITE)
 *
 * @return
 * true - if has any valid moves
 * false - otherwise
 *
 * */
bool spChessGameDoesPlayerHaveValidMoves(SPChessGame *src, int playerColor);


#endif
