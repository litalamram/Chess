#include "SPChessGame.h"
#include "SPChessGameSettings.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <ctype.h>

/********************Prototypes**************************/
int max(int a, int b);
bool isLeagalIndex(int index);
bool isLegalCoord (SPCoordinate coord);
bool isLegalMoveCoordinates (SPMove move);
char unParseRow (int row);
char unParseColumn (int col);
void initializeBord(SPChessGame* game);
void initPawns (SPChessGame* src);
void initRow(SPChessGame* src, int row, int color);
char createPiece (int color, SPPieceType type);
void applyMove (SPChessGame* src, SPMove move);
bool doesMoveCapturePiece (SPChessGame *src, SPMove move);
bool canPlayerMoveTo(SPChessGame *src, SPCoordinate dest, int playerColor);
SPCoordinate findKingLocation (SPChessGame *src, int playerColor);
bool spChessIsPlayerInCheckMate(SPChessGame *src, int playerColor);
bool spChessIsPlayerInDraw(SPChessGame* src, int playerColor);
bool wouldCauseCheck(SPMove move, int color, SPChessGame *src);
SP_CHESS_GAME_MESSAGE moveStatus (SPChessGame *src, SPMove move);
SP_CHESS_GAME_MESSAGE spChessGameIsValidMoveRook(SPChessGame *src, SPMove move);
SP_CHESS_GAME_MESSAGE spChessGameIsValidMoveBishop(SPChessGame *src, SPMove move);
SP_CHESS_GAME_MESSAGE spChessGameIsValidMoveKing(SPChessGame *src, SPMove move);
SP_CHESS_GAME_MESSAGE spChessGameIsValidMoveKnight(SPChessGame *src, SPMove move);
SP_CHESS_GAME_MESSAGE spChessGameIsValidMovePawn(SPChessGame *src, SPMove move);
SP_CHESS_GAME_MESSAGE spChessGameIsValidMoveQueen(SPChessGame *src, SPMove move);
SP_CHESS_GAME_MESSAGE spChessGameIsValidMoveGeneric(SPChessGame *src, SPMove move);
void setPieceAt (SPChessGame *src, char piece, int row, int col);
int getPieceScore(char piece);
int scoreFunc (void* src);

/*******************Implementation***********************/

/** Returns the maximum value between a and b
 * @param a
 * @param b
 * @return
 * a - if a>b
 *  b - otherwise
 */
int max(int a, int b){
	if (a>b) return a;
	return b;
}

/** Parses and prints the specified coordinate in format "<'8'-coord.row, 'A'+coord.col>"
 * @param coord - the coordinate should be printed
 */
void spChessGamePrintCoord (SPCoordinate coord){
	printf("<%c,%c>",unParseRow(coord.row), unParseColumn(coord.col));
}

/** Parses an array coordinate into a board coordinate
 * @param coord - the coordinate to be parsed
 * @param res - gets the parsed coordinate in format "<'8'-coord.row, 'A'+coord.col>"
 *
 */
void spChessGameParseCoordinate (SPCoordinate coord, char *str){
	sprintf(str, "<%c,%c>",unParseRow(coord.row), unParseColumn(coord.col));
}

/** Returns whether the specified index is between 0 to 7.
 * @param index - the index to be checked
 * @return
 * true - if index>=0 && index<=7
 * false - otherwise
 */
bool isLeagalIndex(int index){
	return index>=0 && index<=7;
}

/** Returns whether the specified coordinate represents a valid board coordinate,
 * i.e, coord.row and coord.col are between 0 to 7.
 * @param coord - the coordinate to be checked
 * @return
 * true - if spChessGameIsLeagalIndex(coord.row)&&spChessGameIsLeagalIndex(coord.col)
 * false - otherwise
 */
bool isLegalCoord (SPCoordinate coord){
	return isLeagalIndex(coord.row) && isLeagalIndex(coord.col);
}

/**Returns whether the specified move has valid start and destination coordinates.
 * @param move - the move to be checked
 * @return
 * true - if spChessGameIsLegalCoord(move.start) && spChessGameIsLegalCoord(move.dest)
 * false - otherwise
 */
bool isLegalMoveCoordinates (SPMove move){
	return isLegalCoord(move.start) && isLegalCoord(move.dest);
}

/** Parses the specified row
 * @param row - the row to be parsed
 * @return
 * '8'-row
 */
int spChessGameParseRow (char row){
	return '8'-row;
}

/** Parses the specified column
 * @param row - the row to be parsed
 * @return
 * col-'A'
 */
int spChessGameParseColumn (char col){
	return col-'A';
}

/** Parses the specified row from array's row index to board index
 * @param row - the row to be parsed
 * @return
 * '8'-row
 */
char unParseRow (int row){
	return '8'-row;
}

/** Parses the specified column from array's column index to board index
 * @param col - the column to be parsed
 * @return
 * col+'A'
 */
char unParseColumn (int col){
	return col+'A';
}

/** Returns a char representing a piece of the specified type and color.
 *
 * @param color - the color the piece should have  (SP_CHESS_GAME_BLACK \ SP_CHESS_GAME_WHITE)
 * @param type - the type of the piece
 *
 * @Return
 * a char representing the piece
 */
char createPiece (int color, SPPieceType type){
	char c;
	switch (type){
	case PAWN:
		c='m';
		break;
	case BISHOP:
		c='b';
		break;
	case ROOK:
		c='r';
		break;
	case KNIGHT:
		c='n';
		break;
	case QUEEN:
		c='q';
		break;
	case KING:
		c='k';
		break;
	case BLANK:
		c=SP_CHESS_GAME_EMPTY_ENTRY;
	}
	if (color == SP_CHESS_GAME_BLACK){
		c = toupper(c); //Upper case
	}
	return c;
}

/** Applies the specified move on the src game:
 * the piece at location move.start is being moved to location move.dest
 *
 * @pre spChessGameisValidMove(src, move) == SP_CHESS_GAME_LEGAL_MOVE
 * @param src - the source game
 * @param move - the move to be applied
 */
void applyMove (SPChessGame* src, SPMove move){
	setPieceAt(src, spChessGameGetPieceAt(src, move.start.row, move.start.col), move.dest.row, move.dest.col);
	setPieceAt(src, SP_CHESS_GAME_EMPTY_ENTRY, move.start.row, move.start.col);
}

/** Initializes game->gameBoard with the initial spots of the black and white pawns
 *  @param game - the source game
 */
void initPawns (SPChessGame* src){
	char piece;
	for (int j=0;j<SP_CHESS_GAME_N_COLUMNS;j++){
		piece = createPiece(SP_CHESS_GAME_BLACK,PAWN);
		setPieceAt(src, piece, 1, j);
		piece = createPiece(SP_CHESS_GAME_WHITE,PAWN);
		setPieceAt(src, piece, 6, j);
	}
}

/** Initializes the specified row of game->gameBoard
 *  with the initial spots of rook, knight, bishop, king and queen of the specified color
 *  @param game - the source game
 *  @param row - the row in the game board to be initialized (0-based)
 *  @param color - the color the piece's should have (SP_CHESS_GAME_BLACK \ SP_CHESS_GAME_WHITE)
 */
void initRow(SPChessGame* src, int row, int color){
	char piece;
	piece = createPiece(color,ROOK);
	setPieceAt(src, piece, row, 0);

	piece = createPiece(color,KNIGHT);
	setPieceAt(src, piece, row, 1);

	piece = createPiece(color,BISHOP);
	setPieceAt(src, piece, row, 2);

	piece = createPiece(color,QUEEN);
	setPieceAt(src, piece, row, 3);

	piece = createPiece(color,KING);
	setPieceAt(src, piece, row, 4);

	piece = createPiece(color,BISHOP);
	setPieceAt(src, piece, row, 5);

	piece = createPiece(color,KNIGHT);
	setPieceAt(src, piece, row, 6);


	piece = createPiece(color,ROOK);
	setPieceAt(src, piece, row, 7);

}

/** Initializes game->gameBoard with the initial spots of the game pieces
 *  @param game - the source game
 */
void initializeBord(SPChessGame* src){
	for (int i=0;i<SP_CHESS_GAME_N_ROWS;i++){
		for (int j=0;j<SP_CHESS_GAME_N_COLUMNS;j++){
			setPieceAt(src, SP_CHESS_GAME_EMPTY_ENTRY, i, j);
		}
	}
	initPawns(src);
	initRow(src,0,SP_CHESS_GAME_BLACK);
	initRow(src,7,SP_CHESS_GAME_WHITE);
}

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
SPChessGame* spChessGameCreate(int historySize){
	if (historySize<=0){
		return NULL;
	}
	SPChessGame* game = (SPChessGame*) malloc(sizeof(SPChessGame));
	if (!game){
		return NULL;
	}
	initializeBord(game);
	game->recentMoves = spArrayListCreate(historySize);
	if (!game->recentMoves){
		free(game);
		return NULL;
	}
	game->currentPlayer = SP_CHESS_GAME_WHITE;
	game->scoreFunc = scoreFunc;
	game->isSaved = 0;
	game->isOver = false;
	spChessGameSettingsSetDefaultsSettings(game);

	return game;
}

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
SPChessGame* spChessGameCopy(SPChessGame* src){
	if (src == NULL){
		return NULL;
	}
	SPChessGame* copy = spChessGameCreate(src->recentMoves->maxSize);
	if (!copy){
		return NULL;
	}
	copy->currentPlayer = src->currentPlayer;
	copy->difficulty = src->difficulty;
	copy->game_mode = src->game_mode;
	copy->isOver = src->isOver;
	copy->isSaved = src->isSaved;
	copy->userColor = src->userColor;

	for (int i=0; i<SP_CHESS_GAME_N_ROWS; i++){
		for (int j=0; j<SP_CHESS_GAME_N_COLUMNS; j++){
			copy->gameBoard[i][j] = src->gameBoard[i][j];
		}
	}

	copy->scoreFunc = src->scoreFunc;
	return copy;

}

/**
 * Frees all memory allocation associated with a given game. If src==NULL
 * the function does nothing.
 *
 * @param src - the source game
 */
void spChessGameDestroy(SPChessGame* src){
	if (!src){
		return;
	}
	spArrayListDestroy(src->recentMoves);
	free(src);
}

/**
 * On success, the function prints the board game to the specified file. If an error occurs, then the
 * function does nothing.
 *
 * @param src - the target game
 * @param f   - the destination file
 * @return
 * SP_CHESS_GAME_INVALID_ARGUMENT - if (src == NULL || f == NULL)
 * SP_CHESS_GAME_IO_EXCEPTION - if an error occurred writing to file
 * SP_CHESS_GAME_SUCCESS - otherwise
 *
 */
SP_CHESS_GAME_MESSAGE spChessGamePrintBoardToFile(FILE *f, SPChessGame* src){
	if (src == NULL || f == NULL){
		return SP_CHESS_GAME_INVALID_ARGUMENT;
	}
	for (int i = 0; i < SP_CHESS_GAME_N_ROWS; i++){
		if (fprintf(f, "%d| ",SP_CHESS_GAME_N_ROWS-i)<0){
			return SP_CHESS_GAME_IO_EXCEPTION;
		}
		for (int j = 0; j < SP_CHESS_GAME_N_COLUMNS; j++){
			if (fprintf(f, "%c ", spChessGameGetPieceAt(src, i, j))<0){
				return SP_CHESS_GAME_IO_EXCEPTION;
			}
		}
		if (fprintf(f, "|\n")<0){
			return SP_CHESS_GAME_IO_EXCEPTION;
		}
	}
	if (fprintf(f, "  -----------------\n")<0){
		return SP_CHESS_GAME_IO_EXCEPTION;
	}
	if (fprintf(f, "   A B C D E F G H\n")<0){
		return SP_CHESS_GAME_IO_EXCEPTION;
	}
	return SP_CHESS_GAME_SUCCESS;
}

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
SP_CHESS_GAME_MESSAGE spChessGameisValidMove(SPChessGame* src, SPMove move){
	//invalid position
	if (!isLegalMoveCoordinates(move)){
		return SP_CHESS_GAME_INVALID_ARGUMENT;
	}

	//empty entry or enemy piece
	if (spChessGameGetPieceAt(src, move.start.row, move.start.col) == SP_CHESS_GAME_EMPTY_ENTRY ||
			src->currentPlayer != spChessGameGetPieceColor(spChessGameGetPieceAt(src, move.start.row, move.start.col))){
		return SP_CHESS_GAME_NOT_YOUR_PIECE;
	}

	//check if the move is llegal
	SP_CHESS_GAME_MESSAGE res = moveStatus(src, move);

	return res;
}

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
SP_CHESS_GAME_MESSAGE spChessGameSetMove(SPChessGame* src, SPMove move){

	if (src == NULL){
		return SP_CHESS_GAME_INVALID_ARGUMENT;
	}
	//add move to recentMoves list
	char capturedPiece = spChessGameGetPieceAt(src, move.dest.row, move.dest.col);
	SPArrayListNode moveNode = spArrayListNewNode(move.start.row, move.start.col, move.dest.row, move.dest.col, capturedPiece);
	if (spArrayListAddLast(src->recentMoves, moveNode) == SP_ARRAY_LIST_FULL){
		spArrayListRemoveFirst(src->recentMoves);
		spArrayListAddLast(src->recentMoves, moveNode) ;
	}
	//apply the move
	applyMove(src, move);
	spChessGameChangePlayer(src);
	return SP_CHESS_GAME_SUCCESS;
}

/** Undoes the specified move on the src game:
 * the piece at location move.dest is being moved to location move.start
 * and capturedPiece is being set at location move.dest
 *
 * @pre spChessGameisValidMove(src, move) == SP_CHESS_GAME_LEGAL_MOVE
 * @param src - the source game
 * @param move - the move to be undo
 */
void undoMove (SPChessGame* src, SPMove move, char capturedPiece){
	SPMove reverseMove = {.start = move.dest, .dest = move.start};
	applyMove(src, reverseMove);
	setPieceAt(src, capturedPiece, move.dest.row, move.dest.col);
}

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
SP_CHESS_GAME_MESSAGE spChessGameUndoPrevMove(SPChessGame* src) {
	if (src == NULL){
		return SP_CHESS_GAME_INVALID_ARGUMENT;
	}
	if (src->recentMoves->actualSize == 0){
		return SP_CHESS_GAME_NO_HISTORY;
	}
	//get last move
	SPArrayListNode lastMove = spArrayListGetLast(src->recentMoves);
	spArrayListRemoveLast(src->recentMoves);
	//undo move
	undoMove(src, (SPMove){{lastMove.sRow, lastMove.sCol},{lastMove.dRow, lastMove.dCol},0,0}, lastMove.capturedpiece);
	spChessGameChangePlayer(src);

	return SP_CHESS_GAME_SUCCESS;
}

/** Returns whether the specified move captures an enemy piece
 * @param src - the game source
 * @param move - the move to be checked
 * @return
 * true - if the board contains a piece at move.dest with different color
 * 		  than the piece at location move.start
 * false - otherwise
 */
bool doesMoveCapturePiece (SPChessGame *src, SPMove move){
	if (src == NULL)
		return false;
	return (spChessGameGetPieceAt(src, move.dest.row, move.dest.col)) != SP_CHESS_GAME_EMPTY_ENTRY
			&& spChessGameGetPieceColor(spChessGameGetPieceAt(src, move.dest.row, move.dest.col)) != spChessGameGetPieceColor(spChessGameGetPieceAt(src, move.start.row, move.start.col));
}

/**
 * Checks if the specified move is legal according to the chess rules.
 *
 * @param src - The source game
 * @param move - The specified move
 * @return
 * SP_CHESS_GAME_INVALID_ARGUMENT - if either one of the locations move.start, move.dest is invalid
 * SP_CHESS_GAME_NOT_YOUR_PIECE  - if position move.start does not contain a piece
 * SP_CHESS_GAME_INVALID_MOVE - if the move is illegal for the piece in the position move.start
 * SP_CHESS_GAME_KING_WILL_BE_THRETHEND - if the move causes the player's king to be threatened
 * SP_CHESS_GAME_LEGAL_MOVE - otherwise.
 */
SP_CHESS_GAME_MESSAGE moveStatus (SPChessGame *src, SPMove move){

	//invalid position
	if (!isLegalMoveCoordinates(move)){
		return SP_CHESS_GAME_INVALID_ARGUMENT;
	}
	SP_CHESS_GAME_MESSAGE res;
	char piece = spChessGameGetPieceAt(src, move.start.row, move.start.col);
	switch (spChessGameGetPieceType(piece)){
	case ROOK:
		res = spChessGameIsValidMoveRook(src, move);
		break;
	case BISHOP:
		res = spChessGameIsValidMoveBishop(src, move);
		break;
	case KNIGHT:
		res = spChessGameIsValidMoveKnight(src, move);
		break;
	case QUEEN:
		res = spChessGameIsValidMoveQueen(src, move);
		break;
	case KING:
		res = spChessGameIsValidMoveKing(src, move);
		break;
	case PAWN:
		res = spChessGameIsValidMovePawn(src, move);
		break;
	case BLANK:
		res = SP_CHESS_GAME_NOT_YOUR_PIECE;
		break;
	}

	return res;
}

/** Returns whether the specified player can move to
 * the specified coordinate with one of his pieces.
 *
 * @param src - the game source
 * @param dest - the destination coordinate
 * @param player - the player color (SP_CHESS_GAME_BLACK \ SP_CHESS_GAME_WHITE)
 *
 *	@return
 *	true - if there is a legal move for the player that ends in dest
 *	false - otherwise
 */
bool canPlayerMoveTo(SPChessGame *src, SPCoordinate dest, int playerColor){
	char piece;
	SPMove move;
	move.dest = dest;
	for (int i=0;i<8;i++){
		for (int j=0;j<8;j++){
			move.start.row = i; move.start.col = j;
			piece = spChessGameGetPieceAt(src, i, j);
			if (piece!='_' && spChessGameGetPieceColor(piece) == playerColor
					&& moveStatus(src, move) ==  SP_CHESS_GAME_LEGAL_MOVE){
				return true;
			}
		}
	}
	return false;
}

/** Returns the location of the specified player's king on the board
 *
 * @param src - the game source
 * @param playerColor - the color of the king to be searched (SP_CHESS_GAME_BLACK \ SP_CHESS_GAME_WHITE)
 *
 * @return
 * The location of the king with the specified color on the board
 * or {-1,-1} if the king was not found
 */
SPCoordinate findKingLocation (SPChessGame *src, int playerColor){
	char piece;
	SPCoordinate res = {.row=-1, .col=-1};
	for (int i=0;i<8;i++){
		for (int j=0;j<8;j++){
			piece = spChessGameGetPieceAt(src, i, j);
			if (spChessGameGetPieceColor(piece) == playerColor && spChessGameGetPieceType(piece) == KING){
				res.row = i;
				res.col = j;
				return res;
			}
		}
	}
	return res;
}

/** Returns if the king at specified position is threatened by a queen, rook or bishop
 *
 * @param src - the src game
 * @param coordinate - location of a king in the board
 *
 * @pre - the piece at specified location is a king
 *
 * @return
 * true, if the king at the specified position is threatened by a queen, rook or bishop
 * false, otherwise
 *
 * */
bool isKingThrethenedByQueenRookBishop (SPChessGame *src, SPCoordinate coordinate){
	int playerColor = spChessGameGetPieceColor(spChessGameGetPieceAt(src, coordinate.row, coordinate.col));
	int enemyColor = (playerColor == SP_CHESS_GAME_WHITE) ? SP_CHESS_GAME_BLACK :  SP_CHESS_GAME_WHITE;

	char piece;
	char queenEnemy = createPiece(enemyColor, QUEEN);
	char bishopEnemy = createPiece(enemyColor, BISHOP);
	char rookEnemy = createPiece(enemyColor, ROOK);

	//search for queen or rook in up direction
	for (int i = 1; i <= coordinate.row; i++){
		piece = spChessGameGetPieceAt(src, coordinate.row-i, coordinate.col);
		if (piece == queenEnemy || piece == rookEnemy){
			return true;
		}
		if (piece != SP_CHESS_GAME_EMPTY_ENTRY){
			break;
		}
	}

	//search for queen or rook in down direction
	for (int i = 1; i <= 7 - coordinate.row; i++){
		piece = spChessGameGetPieceAt(src, coordinate.row+i, coordinate.col);
		if (piece == queenEnemy || piece == rookEnemy){
			return true;
		}
		if (piece != SP_CHESS_GAME_EMPTY_ENTRY){
			break;
		}
	}

	//search for queen or rook in right direction
	for (int i = 1; i <= 7 - coordinate.col; i++){
		piece = spChessGameGetPieceAt(src, coordinate.row, coordinate.col+i);
		if (piece == queenEnemy || piece == rookEnemy){
			return true;
		}
		if (piece != SP_CHESS_GAME_EMPTY_ENTRY){
			break;
		}
	}

	//search for queen or rook in left direction
	for (int i = 1; i <= coordinate.col; i++){
		piece = spChessGameGetPieceAt(src, coordinate.row, coordinate.col-i);
		if (piece == queenEnemy || piece == rookEnemy){
			return true;
		}
		if (piece != SP_CHESS_GAME_EMPTY_ENTRY){
			break;
		}
	}

	//search for queen or bishop in left up direction
	for (int i=1;i<8;i++){
		if (!isLegalCoord((SPCoordinate){coordinate.row-i, coordinate.col-i})){
			break;
		}
		piece = spChessGameGetPieceAt(src, coordinate.row-i, coordinate.col-i);
		if (piece == queenEnemy || piece == bishopEnemy){
			return true;
		}
		if (piece != SP_CHESS_GAME_EMPTY_ENTRY){
			break;
		}
	}


	//search for queen or bishop in right up direction
	for (int i=1;i<8;i++){
		if (!isLegalCoord((SPCoordinate){coordinate.row-i, coordinate.col+i})){
			break;
		}
		piece = spChessGameGetPieceAt(src, coordinate.row-i, coordinate.col+i);
		if (piece == queenEnemy || piece == bishopEnemy){
			return true;
		}
		if (piece != SP_CHESS_GAME_EMPTY_ENTRY){
			break;
		}
	}

	//search for queen or bishop in left down direction
	for (int i=1;i<8;i++){
		if (!isLegalCoord((SPCoordinate){coordinate.row+i, coordinate.col-i})){
			break;
		}
		piece = spChessGameGetPieceAt(src, coordinate.row+i, coordinate.col-i);
		if (piece == queenEnemy || piece == bishopEnemy){
			return true;
		}
		if (piece != SP_CHESS_GAME_EMPTY_ENTRY){
			break;
		}
	}

	//search for queen or bishop in right down direction
	for (int i=1;i<8;i++){
		if (!isLegalCoord((SPCoordinate){coordinate.row+i, coordinate.col+i})){
			break;
		}
		piece = spChessGameGetPieceAt(src, coordinate.row+i, coordinate.col+i);
		if (piece == queenEnemy || piece == bishopEnemy){
			return true;
		}
		if (piece != SP_CHESS_GAME_EMPTY_ENTRY){
			break;
		}
	}

	return false;
}

/** Returns if the king at specified position is threatened by a knight
 *
 * @param src - the src game
 * @param coordinate - location of a king in the board
 *
 * @pre - the piece at specified location is a king
 *
 * @return
 * true, if the king at the specified position is threatened by a knight
 * false, otherwise
 *
 * */
bool isKingThreathenedByKnight (SPChessGame *src, SPCoordinate coordinate){
	int playerColor = spChessGameGetPieceColor(spChessGameGetPieceAt(src, coordinate.row, coordinate.col));
	int enemyColor = (playerColor == SP_CHESS_GAME_WHITE) ? SP_CHESS_GAME_BLACK :  SP_CHESS_GAME_WHITE;

	char piece;
	char knightEnemy = createPiece(enemyColor, KNIGHT);

	int row = coordinate.row, col = coordinate.col;

	//Possible coordinates for knight
	SPCoordinate coordinates[8] = {{.row = row-1, .col = col-2}, {.row = row+1, .col = col-2},
			{.row = row-2, .col = col-1},{.row = row+2, .col = col-1},
			{.row = row-2, .col = col+1}, {.row = row+2, .col = col+1} ,
			{.row = row-1, .col = col-2}, {.row = row+1, .col = col-2}};

	SPCoordinate current;
	for (int i=0; i<8; i++){
		current = coordinates[i];
		if (isLegalCoord(current)){
			piece = spChessGameGetPieceAt(src, current.row, current.col);
			if (piece == knightEnemy){
				return true;
			}
		}

	}
	return false;

}

/** Returns if the king at specified position is threatened by a king
 *
 * @param src - the src game
 * @param coordinate - location of a king in the board
 *
 * @pre - the piece at specified location is a king
 *
 * @return
 * true, if the king at the specified position is threatened by a king
 * false, otherwise
 *
 * */
bool isKingThreathenedByKing (SPChessGame *src, SPCoordinate coordinate){
	int playerColor = spChessGameGetPieceColor(spChessGameGetPieceAt(src, coordinate.row, coordinate.col));
	int enemyColor = (playerColor == SP_CHESS_GAME_WHITE) ? SP_CHESS_GAME_BLACK :  SP_CHESS_GAME_WHITE;

	char piece;
	char kingEnemy = createPiece(enemyColor, KING);

	int row = coordinate.row, col = coordinate.col;

	//Possible coordinates for king
	SPCoordinate coordinates[8] = {{.row = row-1, .col = col-1}, {.row = row, .col = col-1},{.row = row+1, .col = col-1},
			{.row = row-1, .col = col},{.row = row+1, .col = col},
			{.row = row-1, .col = col+1}, {.row = row, .col = col+1} ,{.row = row+1, .col = col+1}};


	SPCoordinate current;
	for (int i=0; i<8; i++){
		current = coordinates[i];
		if (isLegalCoord(current)){
			piece = spChessGameGetPieceAt(src, current.row, current.col);
			if (piece == kingEnemy){
				return true;
			}
		}

	}
	return false;

}

/** Returns if the king at specified position is threatened by a pawn
 *
 * @param src - the src game
 * @param coordinate - location of a king in the board
 *
 * @pre - the piece at specified location is a king
 *
 * @return
 * true, if the king at the specified position is threatened by a pawn
 * false, otherwise
 *
 * */
bool isKingThreathenedByPawn (SPChessGame *src, SPCoordinate coordinate){
	int playerColor = spChessGameGetPieceColor(spChessGameGetPieceAt(src, coordinate.row, coordinate.col));
	int enemyColor = (playerColor == SP_CHESS_GAME_WHITE) ? SP_CHESS_GAME_BLACK :  SP_CHESS_GAME_WHITE;

	char piece;
	char pawnEnemy = createPiece(enemyColor, PAWN);

	int row = coordinate.row, col = coordinate.col;

	//Possible coordinates for pawn
	SPCoordinate coordinates[2];

	if (enemyColor == SP_CHESS_GAME_WHITE){
		coordinates[0] = (SPCoordinate){.row = row+1, .col = col+1};
		coordinates[1] = (SPCoordinate){.row = row+1, .col = col-1};
	}
	else {
		coordinates[0] = (SPCoordinate){.row = row-1, .col = col+1};
		coordinates[1] = (SPCoordinate){.row = row-1, .col = col-1};
	}

	SPCoordinate current;
	for (int i=0; i<2; i++){
		current = coordinates[i];
		if (isLegalCoord(current)){
			piece = spChessGameGetPieceAt(src, current.row, current.col);
			if (piece == pawnEnemy){
				return true;
			}
		}

	}
	return false;

}

/** Returns if the king at specified position is threatened by an opponent piece
 *
 * @param src - the src game
 * @param coordinate - location of a king in the board
 *
 * @pre - the piece at specified location is a king
 *
 * @return
 * true, if the king at the specified position is threatened
 * false, otherwise
 *
 * */
bool isKingThrethened(SPChessGame *src, SPCoordinate coordinate){

	if (isKingThrethenedByQueenRookBishop(src, coordinate)){
		return true;
	}

	if (isKingThreathenedByKnight(src, coordinate)){
		return true;
	}

	if (isKingThreathenedByKing(src, coordinate)){
		return true;
	}

	if (isKingThreathenedByPawn(src, coordinate)){
		return true;
	}

	return false;
}

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
bool spChessGameIsPlayerInCheck(SPChessGame *src, int playerColor){
	if (!src || (playerColor != SP_CHESS_GAME_BLACK && playerColor != SP_CHESS_GAME_WHITE)){
		return false;
	}

	//find king pos
	SPCoordinate kingPos = findKingLocation(src, playerColor);
	if (kingPos.row == -1){
		return true;
	}
	return isKingThrethened(src, kingPos);
}

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
bool spChessGameDoesPlayerHaveValidMoves(SPChessGame *src, int playerColor){
	char piece;
	int size;
	SPMove moves[SP_CHESS_GAME_MAX_NUMBER_OF_MOVES];
	for (int i=0;i<8;i++){
		for (int j=0;j<8;j++){
			piece = spChessGameGetPieceAt(src, i, j);
			if (spChessGameGetPieceColor(piece) == playerColor){
				spChessGameGetMoves(src,(SPCoordinate){.row=i,.col=j},moves,&size,false);
				if (size != 0){
					return true;
				}
			}
		}
	}
	return false;
}

/** Returns whether the specified player is in CheckMate
 * (if his king is threatened by an enemy piece and he doesn't have any valid move)
 *
 * @param src - the fame source
 * @param playerColor - the color of the player (SP_CHESS_GAME_WHITE \ SP_CHESS_GAME_BLACK)
 *
 * @return
 * true - if the specified player is in CheckMate
 * false - otherwise
 */
bool spChessIsPlayerInCheckMate(SPChessGame *src, int playerColor){
	if (!spChessGameIsPlayerInCheck(src, playerColor)){
		return false;
	}
	return !spChessGameDoesPlayerHaveValidMoves(src,playerColor);
}

/** Returns whether the specified player is in draw
 * (if he desn't have any legal moves but his king is not threatened)
 *
 * @param src - the fame source
 * @param playerColor - the color of the player (SP_CHESS_GAME_WHITE \ SP_CHESS_GAME_BLACK)
 *
 * @return
 * true - if the specified player is in draw
 * false - otherwise
 */
bool spChessIsPlayerInDraw(SPChessGame* src, int playerColor){
	if (spChessGameIsPlayerInCheck(src, playerColor)){
		return false;
	}

	return !spChessGameDoesPlayerHaveValidMoves(src,playerColor);
}

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
int spChessGameGetState(SPChessGame *src){
	if (src == NULL){
		return -1;
	}
	bool inCheck = spChessGameIsPlayerInCheck(src, src->currentPlayer);
	bool hasMoves = spChessGameDoesPlayerHaveValidMoves(src, src->currentPlayer);
	if (inCheck && !hasMoves){ //current player in checkmate
		return 1-src->currentPlayer;	 //enemy wins
	}
	if (!inCheck && !hasMoves){ //draw
		return SP_CHESS_GAME_DRAW;
	}
	if (inCheck){ //current player is in check
		return SP_CHESS_GAME_CHECK;
	}

	//game not over
	return SP_CHESS_GAME_NOT_OVER;
}

/** Returns whether the specified move would cause the specified player to be in Check
 *
 * @param src - the game source
 * @param move - the move
 * @param color - the player's color
 *
 * @return
 * true - if the move would cause the player to be in check
 * false - otherwise
 */
bool wouldCauseCheck(SPMove move, int color, SPChessGame *src){
	char pieceRemoved = spChessGameGetPieceAt(src, move.dest.row, move.dest.col);
	applyMove(src, move);
	bool inCheck = spChessGameIsPlayerInCheck(src, color);
	//undo move
	undoMove(src, move, pieceRemoved);
	return inCheck;
}

/**
 * Checks if the specified move is a legal move for a rook.
 *
 * @pre the piece at location move.start is a rook
 *
 * @param src - The source game
 * @param move - The specified move
 *
 * @return
 * SP_CHESS_GAME_INVALID_ARGUMENT - if either one of the locations move.start, move.dest is invalid
 * SP_CHESS_GAME_INVALID_MOVE - if the move is illegal for a rook
 * SP_CHESS_GAME_KING_WILL_BE_THRETHEND - if the move causes the player's king to be threatened
 * SP_CHESS_GAME_LEGAL_MOVE - otherwise.
 */
SP_CHESS_GAME_MESSAGE spChessGameIsValidMoveRook(SPChessGame *src, SPMove move){

	int absDeltaRow, absDeltaCol;
	int startRow = move.start.row, startCol = move.start.col;
	int destRow = move.dest.row, destCol = move.dest.col;
	absDeltaRow = abs(destRow - startRow);
	absDeltaCol = abs(destCol - startCol);

	if (absDeltaRow != 0 && absDeltaCol != 0) { //not horizontal or vertical
		return SP_CHESS_GAME_INVALID_MOVE;
	}

	return spChessGameIsValidMoveGeneric(src, move);
}

/**
 * Checks if the specified move is a legal move for a bishop.
 *
 * @pre the piece at location move.start is a bishop
 *
 * @param src - The source game
 * @param move - The specified move
 *
 * @return
 * SP_CHESS_GAME_INVALID_ARGUMENT - if either one of the locations move.start, move.dest is invalid
 * SP_CHESS_GAME_INVALID_MOVE - if the move is illegal for a bishop
 * SP_CHESS_GAME_KING_WILL_BE_THRETHEND - if the move causes the player's king to be threatened
 * SP_CHESS_GAME_LEGAL_MOVE - otherwise.
 */
SP_CHESS_GAME_MESSAGE spChessGameIsValidMoveBishop(SPChessGame *src, SPMove move) {
	int startRow = move.start.row, startCol = move.start.col;
	int destRow = move.dest.row, destCol = move.dest.col;
	int absDeltaRow = abs(destRow - startRow);
	int absDeltaCol = abs(destCol - startCol);

	if (absDeltaRow != absDeltaCol) {
		return SP_CHESS_GAME_INVALID_MOVE;
	}
	return spChessGameIsValidMoveGeneric(src, move);
}

/**
 * Checks if the specified move is a legal move for a king.
 *
 * @pre the piece at location move.start is a king
 *
 * @param src - The source game
 * @param move - The specified move
 *
 * @return
 * SP_CHESS_GAME_INVALID_ARGUMENT - if either one of the locations move.start, move.dest is invalid
 * SP_CHESS_GAME_INVALID_MOVE - if the move is illegal for a king
 * SP_CHESS_GAME_KING_WILL_BE_THRETHEND - if the move causes the player's king to be threatened
 * SP_CHESS_GAME_LEGAL_MOVE - otherwise.
 */
SP_CHESS_GAME_MESSAGE spChessGameIsValidMoveKing(SPChessGame *src, SPMove move) {
	int startRow = move.start.row, startCol = move.start.col;
	int destRow = move.dest.row, destCol = move.dest.col;
	int absDeltaRow = abs(destRow - startRow);
	int absDeltaCol = abs(destCol - startCol);
	if (absDeltaRow > 1 || absDeltaCol > 1) {
		return SP_CHESS_GAME_INVALID_MOVE;
	}
	return spChessGameIsValidMoveGeneric(src, move);
}

/**
 * Checks if the specified move is a legal move for a knight.
 *
 * @pre the piece at location move.start is a knight
 *
 * @param src - The source game
 * @param move - The specified move
 *
 * @return
 * SP_CHESS_GAME_INVALID_ARGUMENT - if either one of the locations move.start, move.dest is invalid
 * SP_CHESS_GAME_INVALID_MOVE - if the move is illegal for a knight
 * SP_CHESS_GAME_KING_WILL_BE_THRETHEND - if the move causes the player's king to be threatened
 * SP_CHESS_GAME_LEGAL_MOVE - otherwise.
 */
SP_CHESS_GAME_MESSAGE spChessGameIsValidMoveKnight(SPChessGame *src, SPMove move) {
	int startRow = move.start.row, startCol = move.start.col;
	int destRow = move.dest.row, destCol = move.dest.col;
	int absDeltaRow = abs(destRow - startRow);
	int absDeltaCol = abs(destCol - startCol);

	if ((absDeltaRow != 2 || absDeltaCol != 1) && (absDeltaRow != 1 || absDeltaCol != 2)) {
		return SP_CHESS_GAME_INVALID_MOVE;
	}
	return spChessGameIsValidMoveGeneric(src, move);
}

/**
 * Checks if the specified move is a legal move for a pawn.
 *
 * @pre the piece at location move.start is a pawn
 *
 * @param src - The source game
 * @param move - The specified move
 *
 * @return
 * SP_CHESS_GAME_INVALID_ARGUMENT - if either one of the locations move.start, move.dest is invalid
 * SP_CHESS_GAME_INVALID_MOVE - if the move is illegal for a pawn
 * SP_CHESS_GAME_KING_WILL_BE_THRETHEND - if the move causes the player's king to be threatened
 * SP_CHESS_GAME_LEGAL_MOVE - otherwise.
 */
SP_CHESS_GAME_MESSAGE spChessGameIsValidMovePawn(SPChessGame *src, SPMove move) {
	int startRow = move.start.row, startCol = move.start.col;
	int destRow = move.dest.row, destCol = move.dest.col;
	int deltaRow = (destRow - startRow);
	int deltaCol = (destCol - startCol);
	char piece = spChessGameGetPieceAt(src, startRow, startCol);

	if (spChessGameGetPieceColor(piece) == SP_CHESS_GAME_BLACK){
		if ((deltaRow != 1 || deltaCol != 0) &&
				(deltaRow != 2 || deltaCol != 0) &&
				(deltaRow != 1 || (deltaCol != 1 && deltaCol != -1))){
			return SP_CHESS_GAME_INVALID_MOVE;
		}

		if (startRow != 1 && deltaRow == 2){
			return SP_CHESS_GAME_INVALID_MOVE;
		}

		if ((deltaRow == 1 && (deltaCol == 1 || deltaCol == -1)) && !doesMoveCapturePiece(src, move)){
			return SP_CHESS_GAME_INVALID_MOVE;
		}
	}
	else {
		if ((deltaRow != -1 || deltaCol != 0) &&
				(deltaRow != -2 || deltaCol != 0) &&
				(deltaRow != -1 || (deltaCol != 1 && deltaCol != -1))){
			return SP_CHESS_GAME_INVALID_MOVE;
		}

		if (startRow != 6 && deltaRow == -2){
			return SP_CHESS_GAME_INVALID_MOVE;
		}

		if ((deltaRow == -1 && (deltaCol == 1 || deltaCol == -1)) && !doesMoveCapturePiece(src, move)){
			return SP_CHESS_GAME_INVALID_MOVE;
		}

	}

	return spChessGameIsValidMoveGeneric(src, move);
}

/**
 * Checks if the specified move is a legal move for a queen.
 *
 * @pre the piece at location move.start is a queen
 *
 * @param src - The source game
 * @param move - The specified move
 *
 * @return
 * SP_CHESS_GAME_INVALID_ARGUMENT - if either one of the locations move.start, move.dest is invalid
 * SP_CHESS_GAME_INVALID_MOVE - if the move is illegal for a queen
 * SP_CHESS_GAME_KING_WILL_BE_THRETHEND - if the move causes the player's king to be threatened
 * SP_CHESS_GAME_LEGAL_MOVE - otherwise.
 */
SP_CHESS_GAME_MESSAGE spChessGameIsValidMoveQueen(SPChessGame *src, SPMove move) {
	int startRow = move.start.row, startCol = move.start.col;
	int destRow = move.dest.row, destCol = move.dest.col;
	int absDeltaRow = abs(destRow - startRow);
	int absDeltaCol = abs(destCol - startCol);

	if (absDeltaRow != 0 && absDeltaCol != 0 &&  absDeltaRow != absDeltaCol) { //not horizontal,vertical,diagonal
		return SP_CHESS_GAME_INVALID_MOVE;
	}
	return spChessGameIsValidMoveGeneric(src, move);

}

/**
 * Checks if the specified move is a legal move in general.
 * (i.e, checks if the move has valid coordinates,
 * if it doesn't stay in place,
 * if it doesn't skip other pieces (if the piece at move.start is not knight),
 * if doesn't capture the same player's piece,
 * and if it doesn't cause the player to be in check )
 *
 * @pre the piece at location move.start is a rook
 *
 * @param src - The source game
 * @param move - The specified move
 *
 * @return
 * SP_CHESS_GAME_INVALID_ARGUMENT - if either one of the locations move.start, move.dest is invalid
 * SP_CHESS_GAME_INVALID_MOVE - if the move is illegal
 * SP_CHESS_GAME_KING_WILL_BE_THRETHEND - if the move causes the player's king to be threatened
 * SP_CHESS_GAME_LEGAL_MOVE - otherwise.
 */
SP_CHESS_GAME_MESSAGE spChessGameIsValidMoveGeneric(SPChessGame *src, SPMove move) {
	if (src == NULL || !isLegalMoveCoordinates(move)) { //invalid location on board
		return SP_CHESS_GAME_INVALID_ARGUMENT;
	}

	int startRow = move.start.row, startCol = move.start.col;
	int destRow = move.dest.row, destCol = move.dest.col;
	int absDeltaRow = abs(destRow - startRow);
	int absDeltaCol = abs(destCol - startCol);

	char piece = spChessGameGetPieceAt(src, startRow, startCol);

	int rowStep = 0;
	int colStep = 0;

	if (absDeltaRow == 0 && absDeltaCol == 0) { //can't stay in place
		return SP_CHESS_GAME_INVALID_MOVE;
	}

	if (absDeltaRow != 0) {
		rowStep = absDeltaRow / (destRow - startRow); //rowStep = +1 or -1
	}
	if (absDeltaCol != 0) {
		colStep = absDeltaCol / (destCol - startCol); //colStep = +1 or -1
	}

	if (spChessGameGetPieceType(piece) != KNIGHT){
		//can't skip other pieces
		int m = max(absDeltaRow, absDeltaCol);
		for (int i = 1; i < m; i++) {
			if (spChessGameGetPieceAt(src, startRow + rowStep * i, startCol + colStep * i) != SP_CHESS_GAME_EMPTY_ENTRY) { //there is a piece in the way
				return SP_CHESS_GAME_INVALID_MOVE;
			}
		}
	}

	//check if capturing is legal
	char destPiece = spChessGameGetPieceAt(src, destRow, destCol);
	if (destPiece != SP_CHESS_GAME_EMPTY_ENTRY)
	{
		if (spChessGameGetPieceColor(destPiece) == spChessGameGetPieceColor(piece)) { //can't capture piece of the same color
			return SP_CHESS_GAME_INVALID_MOVE;
		}
		if (spChessGameGetPieceType(piece) == PAWN && (absDeltaRow !=1 || absDeltaCol != 1)){ //pawn can capture only diagonally
			return SP_CHESS_GAME_INVALID_MOVE;
		}
	}

	//check if the move causes the player to be in check
	if (spChessGameGetPieceType(destPiece)!=KING){
		if (wouldCauseCheck(move, spChessGameGetPieceColor(piece), src)){
			return SP_CHESS_GAME_KING_WILL_BE_THRETHEND;
		}
	}
	return SP_CHESS_GAME_LEGAL_MOVE;
}

SP_CHESS_GAME_MESSAGE spChessGameGetMoves(SPChessGame* src, SPCoordinate coord, SPMove *moves, int *size, bool flags) {
	int curr=0;

	if (!isLegalCoord(coord)){
		return SP_CHESS_GAME_INVALID_ARGUMENT;
	}
	char piece = spChessGameGetPieceAt(src, coord.row, coord.col);
	if (piece == SP_CHESS_GAME_EMPTY_ENTRY){
		return SP_CHESS_GAME_POSITION_NOT_CONTAIN_PIECE;
	}
	char capturedPiece;
	SPMove move;
	move.start = coord;


	for (int j=0;j<8;j++){
		for (int i=7;i>=0;i--){
			move.dest.row = i; move.dest.col = j;
			if (moveStatus(src,move)==SP_CHESS_GAME_LEGAL_MOVE){
				if (flags){
					//check if the move captures enemy piece
					move.doesCapturePiece = doesMoveCapturePiece(src, move);
					//check if the move is threatened by the opponent
					capturedPiece = spChessGameGetPieceAt(src, i, j);
					applyMove(src, move);
					move.isThreatened = canPlayerMoveTo(src, move.dest, 1-spChessGameGetPieceColor(piece));
					//undo move
					undoMove(src, move, capturedPiece);
				}
				//add move to the array
				moves[curr++] = move;
			}
		}
	}
	*size = curr;
	return SP_CHESS_GAME_SUCCESS;

}

/** Returns the piece at the specified location on the board
 *
 * @pre src!=NULL, row,col are legal
 * @param src - the game dource
 * @param row, col - the row,column on the board
 * @return
 * The piece at location *row,col)
 */
char spChessGameGetPieceAt (SPChessGame *src, int row, int col){
	return src->gameBoard[row][col];
}

/** Sets the specified position to contain the specified piece
 *
 * @param src - the game source
 * @param piece - the piece to be set on the board
 * @param row,col - the location to set the piece on the board
 */
void setPieceAt (SPChessGame *src, char piece, int row, int col){
	src->gameBoard[row][col] = piece;
}

/** Returns the color of the given piece
 *
 * @param piece
 * @return
 * SP_CHESS_GAME_WHITE - if the piece's color is white
 * SP_CHESS_GAME_BLACK - if the piece's color is black
 * 2 - if piece == SP_CHESS_GAME_EMPTY_ENTRY
 */
int spChessGameGetPieceColor (char piece){
	if (piece==SP_CHESS_GAME_EMPTY_ENTRY){
		return 2;
	}
	return islower(piece) ? SP_CHESS_GAME_WHITE : SP_CHESS_GAME_BLACK;
}

/** Returns a string representation of the given piece
 * @param piece
 * @return
 * The name of the piece
 */
char* spChessGameGetPieceName (char piece){
	SPPieceType pType = spChessGameGetPieceType(piece);
	switch (pType){
	case PAWN:
		return "pawn";
	case BISHOP:
		return "bishop";
	case ROOK:
		return "rook";
	case KNIGHT:
		return "knight";
	case QUEEN:
		return "queen";
	case KING:
		return "king";
	case BLANK:
		return "";
	}
	return "";
}

/** Returns the type of the specified piece
 * @param piece
 * @return the piec's type
 */
SPPieceType spChessGameGetPieceType (char piece){
	piece = tolower(piece);
	switch (piece){
	case 'm':
		return PAWN;
	case 'b':
		return BISHOP;
	case 'r':
		return ROOK;
	case 'n':
		return KNIGHT;
	case 'q':
		return QUEEN;
	case 'k':
		return KING;
	}
	return BLANK;
}

/**
 * Returns the current player of the specified game.
 * @param src - the source game
 * @return
 * SP_CHESS_GAME_BLACK - if it's black player's turn
 * SP_CHESS_GAME_WHITE - if it's white's turn
 * SP_CHESS_GAME_EMPTY_ENTRY     - otherwise
 */
int spChessGameGetCurrentPlayer(SPChessGame* src) {
	if (src == NULL){
		return SP_CHESS_GAME_EMPTY_ENTRY;
	}
	return src->currentPlayer;
}

/** Changes the current player
 *
 * @param src - the game source
 */
void spChessGameChangePlayer (SPChessGame* src){
	src->currentPlayer = 1-src->currentPlayer;
}

/** Returns a score for the specified piece
 *
 * @param piece
 * @return a score for the specified piece
 */
int getPieceScore(char piece){
	SPPieceType pType = spChessGameGetPieceType(piece);
	switch (pType){
	case PAWN:
		return 1;
	case KNIGHT:
		return 3;
	case BISHOP:
		return 3;
	case ROOK:
		return 5;
	case QUEEN:
		return 9;
	case KING:
		return 100;
	case BLANK:
		return 0;
	}
	return 0;
}

/** Computes the score of a specified game state
 *  the black player's pieces has positive scores
 *  and the white player's pieces has negative scores
 * 	The score is calculated according to the pieces on the board
 *
 * 	@param src - the source game
 *
 * 	@return
 *  The score
 */
int scoreFunc (void* src){
	SPChessGame *game = (SPChessGame*)src;
	int score = 0, pieceScore;

	for (int i=0;i<8;i++){
		for (int j=0;j<8;j++){
			pieceScore = getPieceScore(spChessGameGetPieceAt(game,i,j));
			if (spChessGameGetPieceColor(spChessGameGetPieceAt(game,i,j))==SP_CHESS_GAME_BLACK){
				score+=pieceScore;
			}
			else {
				score-=pieceScore;
			}
		}
	}
	return score;
}

/** Deletes moves history
 *
 * @param src - the src game
 * */
void spChessGameCleanHistory (SPChessGame* src){
	spArrayListClear(src->recentMoves); //delete moves history
}

/** Restarts the game with the current game settings
 *
 * @param src - the game src
 * */
void spChessGameRestartGame (SPChessGame* src){
	spChessGameCleanHistory(src); //delete moves history
	// save current settings
	int mode = src->game_mode;
	int userColor = src->userColor;
	int diff = src->difficulty;
	//init board
	initializeBord(src);
	//set game settings
	src->currentPlayer = SP_CHESS_GAME_WHITE; //the game start with the white player;
	src->difficulty = diff;
	src->game_mode = mode;
	src->userColor = userColor;
	src->isSaved = 0;
}
