#ifndef SPMINIMAXNODE_H_
#define SPMINIMAXNODE_H_

#include "SPChessGame.h"

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
SPMove spMiniMax (SPChessGame* src, int depth);

#endif
