#ifndef MINIMAX_H_
#define MINIMAX_H_
#include "Game.h"
#include <ctype.h>


#define INT_MAX 2147483647
#define INT_MIN -2147483648

/**
 * Translate a piece symbol to its game value.
 * @param game - the current game.
 * @param piece - the given piece.
 * @return
 * the piece's value, or 0 if received EMPTY_CHAR.
 */
int pieceSymbolToValue (Game *game, char piece);

/**
 * Given the current game instant and previous score (before the last move),
 * the function calculates the new score.
 * @param game - the current game.
 * @param oldScore - the previous score.
 * @return -
 * the new score that describes the current game state.
 */
int calcScore(Game *game, int oldScore);

/**
 * Given the current game, the function calculates the game state's score.
 * @param game - the current game.
 * @return
 * the score that describes the current game state.
 */
int calcInitialScore(Game *game);

/**
 * This function is a shell function that updates src and dest to be
 * the chosen move, found by the minimax algorithm.
 * @param game - the current game.
 * @param src[2] - the source position of the chosen move.
 * @param dest[2] - the destination position of the chosen move.
 * @return -
 * true, if no allocation error occurred, false otherwise.
 */
bool findNextMove(Game *game, int *src, int *dest);

/**
 * Calculates recursively the next move according to the minimax
 * algorithm with pruning and updates src and dest accordingly.
 * @param game - the current game.
 * @param alpha - the minimum value for a score in a game state, as
 * 				  explained in the pruning presentation.
 * @param beta - the maximum value for a score in a game state, as
 * 				 explained in the pruning presentation.
 * @param depth - the current depth of the recursion.
 * @param src[2] - the source position of the chosen move.
 * @param dest[2] - the destination position of the chosen move.
 * @param score - the current score.
 * @return
 * the score value.
 */
int findNextMoveRec(Game *game, int alpha, int beta, int depth, int *src, int *dest, int score);

#endif /* MINIMAX_H_ */
