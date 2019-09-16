#include <stdio.h>
#include "SPMinimaxNode.h"

/**
 * Given a game state, this function evaluates the best move according to
 * the current player. The function initiates a MiniMax algorithm up to a
 * specified length given by maxDepth. The current game state doesn't change
 * by this function including the history of previous moves.
 *
 * @param currentGame - The current game state
 * @param maxDepth - The maximum depth of the miniMax algorithm
 * @return
 * {{-1,-1},{-1,-1},-1,-1} if either currentGame is NULL or maxDepth <= 0.
 * On success the function returns a valid move
 * which is the best move for the current player.
 */
SPMove spMinimaxSuggestMove(SPChessGame* currentGame, unsigned int maxDepth){
	if (currentGame==NULL || maxDepth<=0){
		return (SPMove){{-1,-1},{-1,-1},-1,-1};
	}
	return spMiniMax(currentGame, maxDepth);
}



