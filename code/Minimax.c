#include "Minimax.h"

int pieceSymbolToValue (Game *game, char piece) {
	int sign = 1;
	if (game-> userColor == 0) // user is black -> computer is white
		sign = -1;
	switch(piece) {
	case 'M':
		return 1*sign;
	case 'N':
		return 3*sign;
	case 'B':
		return 3*sign;
	case 'R':
		return 5*sign;
	case 'Q':
		return 9*sign;
	case 'K':
		return 100*sign;
	case 'm':
		return -1*sign;
	case 'n':
		return -3*sign;
	case 'b':
		return -3*sign;
	case 'r':
		return -5*sign;
	case 'q':
		return -9*sign;
	case 'k':
		return -100*sign;
	}
	return 0; // If piece == '_' (EMPTY_CHAR)
}

int calcScore(Game *game, int oldScore) {
	switch (game->state) {
	case DRAW:
		return 0;
	case BLACK_LOSE:
		if (game-> userColor == 0)
			return 1000; // Computer's color is white and the black player(user) lost.      COMPUTER WIN
		return -1000; // Computer's color is black and the black player(computer) lost.     COMPUTER LOSE
	case WHITE_LOSE:
		if (game-> userColor == 0)
			return -1000; // Computer's color is white and the white player(computer) lost. COMPUTER LOSE
		return 1000; // Computer's color is black and the white player(user) lost.          COMPUTER WIN
	default:
		/**
		 * while we updates according to the old score, sometimes the computer reach a situation where the
		 * old score isn't accurate (if he checks it after he checked a win/lose situation).
		 * Therefore there is a need to recalculate the score sometimes.
		 */
		if(oldScore == -1000 || oldScore == 1000)
			return calcInitialScore(game);
		// Updating the score according to the last piece removed from the board(if any).
		return oldScore - pieceSymbolToValue(game, getLastKill(game->history));
	}
}

int calcInitialScore(Game *game) {
	int score = 0, row, col;
	for (row = 0 ; row < ROWS ; row++) {
		for (col = 0 ; col < COLUMNS ; col++) {
			score += pieceSymbolToValue(game, game->gameBoard[row][col]);
		}
	}
	return score;
}

bool findNextMove(Game *game, int *src, int *dest) {
	Game *gameCpy = copyGame(game);
	if (gameCpy == NULL) {
		printf("ERROR: Minimax has failed.\n");
		return false;
	}
	findNextMoveRec(gameCpy, INT_MIN, INT_MAX, 0, src, dest, calcInitialScore(game));
	destroyGame(gameCpy);
	return true;
}

int findNextMoveRec(Game *game, int alpha, int beta, int depth, int *src, int *dest, int score) {
	int row = 0, col = 0, move = 0, doMoveSrc[2], tmpScore;
	int destinations[MAX_MOVES_PER_PIECE][3];
	int scoreLimit;
	if (depth == game->difficulty)  // Breakpoint condition
		return score;
	if(depth %2 == 0)
		scoreLimit = INT_MIN;
	else
		scoreLimit = INT_MAX;
	for (col = 0 ; col < COLUMNS ; col++) {
		for (row = ROWS-1 ; row >= 0 ; row--) {
			if (checkPlayer(game->gameBoard[row][col]) == game->currentPlayer ) {
			// we reach a current player's piece
				resetMatrix(destinations); // reset all values to (-1)
				if (getMoves(game, row, col, 2, destinations) == false){ // find the piece's leagal moves
					continue; // no legal moves
				}
				doMoveSrc[0] = row;
				doMoveSrc[1] = col;
				// go through all the possible moves
				for (move = 0 ; move < MAX_MOVES_PER_PIECE ; move++) {
					if(destinations[move][0] == -1)
						break;
					doMove(game, doMoveSrc, destinations[move]);
					game -> state = checkState(game);
					// recursive call with the new fitting score and next depth level
					tmpScore = findNextMoveRec(game, alpha, beta, depth+1, src, dest, calcScore(game, score));
					if (depth % 2 == 0){
						if(tmpScore > scoreLimit) // max
							scoreLimit = tmpScore;
						if(scoreLimit > alpha){
							alpha = scoreLimit;
							if(depth == 0) {
								src[0] = row;
								src[1] = col;
								dest[0] = destinations[move][0];
								dest[1] = destinations[move][1];
							}
						}							
					}
					else{ // if(depth % 2 != 0)
						if(tmpScore < scoreLimit) // min
							scoreLimit = tmpScore;
						if(scoreLimit < beta)
							beta = scoreLimit;
					}
					undoMove(game, doMoveSrc, destinations[move]);
					if (alpha >= beta) // pruning: if alpha >= beta, we skip the calculation
						return scoreLimit;
				}
			}
		}
	}
	return scoreLimit;
}
