#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "SPMinimaxNode.h"

/** Updates the values of parentScore, alpha, beta, and bestMove.
 *
 * If the node is a max node of the miniMax tree,
 * it's score is set to the maximum between it's score his child's score,
 * and alpha is set to the maximum between alpha and the parentScore.
 * Otherwise, ParentScore is set to the minimum between him his child's score,
 * and beta is set to the minimum between beta and the parentScore.
 * The bestMove is updated accordingly.
 *
 * @param parentScore - pointer to the parent score
 * @param childScore - the child's score at the minimax tree
 * @param a - pointer to alpha value
 * @param b - pointer to  beta value
 * @param isMax - is the current node is max\min node
 * @param currMove - the current move has been done in the minimax tree
 * @param bestMove - pointer to the best move
 * */
void update(int *parentScore, int childScore, int *a, int *b, bool isMax, SPMove currMove, SPMove *bestMove){
	if((isMax && *parentScore < childScore) || (!isMax && childScore < *parentScore)){
		*parentScore = childScore;
		*bestMove = currMove;
	}
	if (isMax){
		*a = (*a > *parentScore) ? *a : *parentScore;
	}
	else {
		*b = (*b < *parentScore) ? *b : *parentScore;
	}
}

/**
 * Given a game state, this function evaluates the best move according to
 * the current player. The function initiates a MiniMax algorithm using alpha beta puirning up to a
 * specified length given by depth.
 *
 * @param src - The game source
 * @param depth - The maximum depth of the miniMax algorithm
 * @param isMax - Is the current player is maximizing or minimizing the score
 * @param a - Alpha value (initially INT_MIN)
 * @param b - Beta value  (initially INT_MAX)
 * @param bestMove - pointer to get the bestMove for the player
 *
 * @return
 * The score of the node represented by the current game state in the minimax tree
 */
int spMiniMaxAlphaBeta(SPChessGame* src, unsigned int depth, bool isMax, int a, int b, SPMove *bestMove){
	if (!spChessGameDoesPlayerHaveValidMoves(src, src->currentPlayer)){ //no legal moves
		if (spChessGameIsPlayerInCheck(src, src->currentPlayer)){ //current player in checkmate
			if (src->currentPlayer == SP_CHESS_GAME_WHITE) {
				return SP_CHESS_GAME_MAX_SCORE; //black wins
			}
			else {
				return SP_CHESS_GAME_MIN_SCORE; //white wins

			}
		}
		else {//draw
			return SP_CHESS_GAME_DRAW_SCORE;
		}
	}
	if (depth == 0){	//the node is a leaf
		return src->scoreFunc(src);
	}


	int size=0, parentScore, childScore;
	SPMove currBestMove = {{INT_MIN,INT_MAX},{INT_MIN,INT_MAX},0,0};
	SPMove moves[SP_CHESS_GAME_MAX_NUMBER_OF_MOVES];
	SPCoordinate coord;
	char pieceRemoved;
	parentScore = (isMax) ? INT_MIN : INT_MAX;

	for (int j=0;j<8;j++){
		for (int i=7;i>=0;i--){
			coord.row = i; coord.col = j;
			if (spChessGameGetPieceColor(spChessGameGetPieceAt(src,i,j))==src->currentPlayer){
				spChessGameGetMoves(src, coord, moves,&size, false);
				for (int k=0;k<size;k++){
					//set move
					pieceRemoved = spChessGameGetPieceAt(src, moves[k].dest.row, moves[k].dest.col);
					spChessGameSetMove(src,moves[k]);
					//compute child score
					childScore = spMiniMaxAlphaBeta(src, depth-1, !isMax, a, b, bestMove);
					//undo move
					undoMove(src, moves[k], pieceRemoved);
					spChessGameChangePlayer(src);
					//update the values of parentScore, a, b, currBestMove
					update(&parentScore, childScore, &a, &b, isMax, moves[k], &currBestMove);
					if (b <= a){ //cut-off
						*bestMove = currBestMove;
						return parentScore;
					}
				}
			}
		}
	}
	*bestMove = currBestMove;
	return parentScore;
}

/**
 * Given a game state, this function evaluates the best move according to
 * the current player. The function initiates a MiniMax algorithm using alpha beta puirning up to a
 * specified length given by maxDepth. The current game state doesn't change
 * by this function including the history of previous moves.
 *
 * @param currentGame - The current game state
 * @param maxDepth - The maximum depth of the miniMax algorithm
 * @return
 * {{-1,-1},{-1,-1},0,0} if a memory allocation error has occurred.
 * On success the function returns a valid move
 * which is the best move for the current player.
 */
SPMove spMiniMax (SPChessGame* src, int depth){
	SPChessGame *copy = spChessGameCopy(src);
	SPMove bestMove = {{-1,-1},{-1,-1},0,0};
	if (!copy){
		return bestMove;
	}
	bool isMax = (src->currentPlayer == SP_CHESS_GAME_BLACK);
	spMiniMaxAlphaBeta(copy, depth, isMax, INT_MIN, INT_MAX, &bestMove);
	spChessGameDestroy(copy);
	return bestMove;
}

