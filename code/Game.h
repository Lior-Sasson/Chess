#ifndef GAME_H_
#define GAME_H_

#include <stdbool.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "History.h"

#define ROWS 8
#define COLUMNS 8
#define BLACK_PLAYER 0
#define WHITE_PLAYER 1
#define ONE_PLAYER 1
#define TWO_PLAYER 2
#define MAX_MOVES_PER_PIECE 28
#define EMPTY_CHAR '_'


/**
 * A type used for describing the current game state.
 */
typedef enum chess_state{
	BLACK_LOSE,
	WHITE_LOSE,
	DRAW,
	BLACK_CHECKED,
	WHITE_CHECKED,
	GAME, // The default value - if it's none of the above
}STATE;


/**
 * Struct Game defines the current state of the game,
 * including history and the board.
 */
typedef struct chess_game{
	History* history;
	char gameBoard[8][8];
	int gameMode;
	STATE state;
	int currentPlayer;
	
	// only for one player:
	int userColor;
	int difficulty;	

	int kings[2][2];

} Game;

/**
 * A type used for move errors.
 */
typedef enum chess_move_error{
	SUCCSESS, // CHANGE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	INVALID_POSITION,
	EMPTY_POSITION,
	ILLEGAL_MOVE,
	KING_STILL_EXPOSED,
	KING_EXPOSED,
	NO_HISTORY,
} GAME_MOVE_ERROR;

/*
 * Check to whom a piece belong(to which player)
 * @param piece - a char represent the chess piece
 * @return
 * BLACK_PLAYER, WHITE_PLAYER accordingly, otherwise -1 (if it's '_')
 */
int checkPlayer(char piece);

/**
 * Allocate memory for a Game struct(including memory
 * for history).
 * @return
 * NULL if an allocation error has occured, otherwise
 * an instant of an undefined Game.
 */
Game* allocateGame();



/**
 * Create and define a Game instant.
 * @param mode - game mode either ONE_PLAYER or TWO_PLAYER.
 * @param difficulty - only for ONE_PLAYER mode, specify
 * 					the difficulty(between 1 and 5)
 * @param color - only for ONE_PLAYER mode, specify user's color(BLACK_PLAYER or WHITE_PLAYER)
 * @return
 * NULL if an allocation error has occurred, otherwise
 * an instant of a Game.
 */
Game* createGame(int mode,  int difficulty, int color);

/**
 * Initialize the game board(to the starting state of chess) of a given game instance.
 * @param game - a pointer to the game instance.
 */
void initializeBoard(Game *game);

/**
 * Destroy a Game instant.
 * @param game - and instant of Game.
 */
void destroyGame(Game* game);

/**
 * Create a copy of a Game.
 * @param game - the original instant of Game.
 * @return
 * NULL if an allocation error has occurred, otherwise
 * a copy of game.
 */
Game* copyGame(Game*game);

/**
 * Check if the specified position is available, meaning
 * empty or only with an enemy unit (belongs to the other player).
 * @param game - an instance of Game.
 * @param row - the row component of the specified position.
 * @param col - the column component of the specified position.
 * @param player - the player we are checking for.
 * @return
 * true if it's available, otherwise false.
 */
bool available(Game* game, int row, int col, int player);

/**
 * Check if a bishop can perform the specified movement.
 * @param game - an instant of Game.
 * @param src - the source position.
 * @param dest - the destination position.
 * @return
 * true if it's a legal movement of a bishop, otherwise false.
 */
bool checkBishop(Game* game,  int src[2], int dest[2]);

/**
 * Check if a king can perform the specified movement.
 * @param game - an instant of Game.
 * @param src - the source position.
 * @param dest - the destination position.
 * @return
 * true if it's a legal movement of a king, otherwise false.
 */
bool checkKing(Game* game,  int src[2], int dest[2]);

/**
 * Check if a pawn can perform the specified movement.
 * @param game - an instant of Game.
 * @param src - the source position.
 * @param dest - the destination position.
 * @return
 * true if it's a legal movement of a pawn, otherwise false.
 */
bool checkPawn(Game* game,  int src[2], int dest[2]);

/**
 * Check if a knight can perform the specified movement.
 * @param game - an instant of Game.
 * @param src - the source position.
 * @param dest - the destination position.
 * @return
 * true if it's a legal movement of a knight, otherwise false.
 */
bool checkKnight(Game* game,  int src[2], int dest[2]);

/**
 * Check if a rook can perform the specified movement.
 * @param game - an instant of Game.
 * @param src - the source position.
 * @param dest - the destination position.
 * @return
 * true if it's a legal movement of a rook, otherwise false.
 */
bool checkRook(Game* game,  int src[2], int dest[2]);

/**
 * Check if a queen can perform the specified movement.
 * @param game - an instant of Game.
 * @param src - the source position.
 * @param dest - the destination position.
 * @return
 * true if it's a legal movement of a queen, otherwise false.
 */
bool checkQueen(Game* game,  int src[2], int dest[2]);

/**
 * Check if the a piece of a player is threatened.
 * @param game - an instant of Game.
 * @param row - the row position of the piece.
 * @param col - the column position of the piece.
 * @return
 * fitting enum of type STATE(BLACK_CHECKED, WHITE_WIN, etc)
 */
bool checkThreat(Game* game, int row, int col);

/*
 * Check if the move is valid and save it's type(if the piece will be threatened and/or it will capture an enemy piece).
 * @param game - an instant of Game.
 * @param src - the source position.
 * @param dest - the destination position.
 * @param moveType - pointer to an integer which will be the type of move,after the function(if threatened == false).
 * @param checkingState - boolean to check if we are only looking for a possible move(checkingState==true)
 * 						 or it's properties as well(checkingState==false).
 * @return
 * true if the move is completed and is legal, false otherwise.
 */
bool move(Game* game, int src[2], int dest[2], int* moveType, int stateNum);

/*
 * Print the destination of a move and it's type to the console.(<x,y>^ for example)
 * @param dest - the designation position
 * @param moveType - indicate the move type(0 = regular, 1 = if the move is threatened by an opponent,
 * 2 = if the move captures a piece, 3 = 2 and 1 at the same time)
 */
void printMoveType(int dest[2], int moveType);

/*
 * Check for bishop or rook(differanting using a boolean) all it's possible moves and if they
 * are legal or not(print if they are legal & threatened==false), if threatened==true just return true and
 * finish.
 * @param game - an instant of Game.
 * @param row - the row position of the bishop/rook.
 * @param col - the column position of the bishop/rook.
 * @param stateNum - int to verify the current usage of the function:
 * 						0 - we are printing all the possible moves for the piece & it's properties as well.
 * 						1 - we are only looking for one possible move(to see if it's a check or checkmate for example).
 * 						2 - we are saving to dests[][] all the possible moves of the piece(without printing).
 * @param bishopNotRook - true if it's bishop, false otherwise(if it's rook).
 * @param dests[28][3] - the array of all the possible destinations for the piece(only relevent if stateNum == 2) and their type.
 * @return
 * true, if there is a legal move and, otherwise return false if there are no legal moves.
 */
bool bishopRookMoves(Game* game, int row, int col, int stateNum, bool bishopNotRook, int dests[MAX_MOVES_PER_PIECE][3]);

/*
 * Check for king all it's possible moves and if they are legal or not
 * (print if they are legal & threatened==false), if threatened==true just return true and finish.
 * @param game - an instant of Game.
 * @param row - the row position of the bishop/rook.
 * @param col - the column position of the bishop/rook.
 * @param stateNum - int to verify the current usage of the function:
 * 						0 - we are printing all the possible moves for the piece & it's properties as well.
 * 						1 - we are only looking for one possible move(to see if it's a check or checkmate for example).
 * 						2 - we are saving to dests[][] all the possible moves of the piece(without printing).
 * @param dests[28][3] - the array of all the possible destinations for the piece(only relevent if stateNum == 2) and their type.
 * @return
 * true, if there is a legal move, otherwise return false (if there are no legal moves).
 */
bool kingMoves(Game* game, int row, int col, int stateNum, int dests[MAX_MOVES_PER_PIECE][3]);

/*
 * Check for queen all it's possible moves and if they are legal or not
 * (print if they are legal & threatened==false), if threatened==true just return true and finish.
 * @param game - an instant of Game.
 * @param row - the row position of the bishop/rook.
 * @param col - the column position of the bishop/rook.
 * @param stateNum - int to verify the current usage of the function:
 * 						0 - we are printing all the possible moves for the piece & it's properties as well.
 * 						1 - we are only looking for one possible move(to see if it's a check or checkmate for example).
 * 						2 - we are saving to dests[][] all the possible moves of the piece(without printing).
 * @param dests[28][3] - the array of all the possible destinations for the piece(only relevent if stateNum == 2) and their type.
 * @return
 * true, if there is a legal move, otherwise return false (if there are no legal moves).
 */
bool queenMoves(Game* game, int row, int col, int stateNum, int dests[MAX_MOVES_PER_PIECE][3]);

/*
 * Check for pawn all it's possible moves and if they are legal or not
 * (print if they are legal & threatened==false), if threatened==true just return true and finish.
 * @param game - an instant of Game.
 * @param row - the row position of the bishop/rook.
 * @param col - the column position of the bishop/rook.
 * @param stateNum - int to verify the current usage of the function:
 * 						0 - we are printing all the possible moves for the piece & it's properties as well.
 * 						1 - we are only looking for one possible move(to see if it's a check or checkmate for example).
 * 						2 - we are saving to dests[][] all the possible moves of the piece(without printing).
 * @param dests[28][3] - the array of all the possible destinations for the piece(only relevent if stateNum == 2) and their type.
 * @return
 * true, if there is a legal move, otherwise return false (if there are no legal moves).
 */
bool pawnMoves(Game* game, int row, int col, int stateNum, int dests[MAX_MOVES_PER_PIECE][3]);

/*
 * Check for knight all it's possible moves and if they are legal or not
 * (print if they are legal & threatened==false), if threatened==true just return true and finish.
 * @param game - an instant of Game.
 * @param row - the row position of the bishop/rook.
 * @param col - the column position of the bishop/rook.
 * @param stateNum - int to verify the current usage of the function:
 * 						0 - we are printing all the possible moves for the piece & it's properties as well.
 * 						1 - we are only looking for one possible move(to see if it's a check or checkmate for example).
 * 						2 - we are saving to dests[][] all the possible moves of the piece(without printing).
 * @param dests[28][3] - the array of all the possible destinations for the piece(only relevent if stateNum == 2) and their type.
 * @return
 * true, if there is a legal move, otherwise return false (if there are no legal moves).
 */
bool knightMoves(Game* game, int row, int col, int stateNum, int dests[MAX_MOVES_PER_PIECE][3]);

/*
 * Will either print all possible moves for the piece in location <row,col>(if threatened == false)
 * or will return true the moment there is at least one legal move(if threatened == true).
 * @param game - an instant of Game.
 * @param row - the row position of the piece.
 * @param col - the column position of the piece.
 * @param stateNum - int to verify the current usage of the function:
 * 						0 - we are printing all the possible moves for the piece & it's properties as well.
 * 						1 - we are only looking for one possible move(to see if it's a check or checkmate for example).
 * 						2 - we are saving to dests[][] all the possible moves of the piece(without printing).
 * @param dests[28][3] - the array of all the possible destinations for the piece(only relevent if stateNum == 2) and their type.
 * @return
 * true, if there is a legal move, otherwise return false (if there are no legal moves).
 */
bool getMoves(Game* game, int row, int col, int stateNum, int dests[MAX_MOVES_PER_PIECE][3]);

/**
 * Check what is the current state of an instant of Game.
 * @param game - an instant of Game.
 * @return
 * GAME if there is no special state(tie,win,check),
 * otherwise return a fitting STATE enum describing
 * the current state.
 */
STATE checkState(Game* game);

/**
 * Perform a move without checking it and updates the Game instant(but not the STATE).
 * @param game - the original instant of Game.
 * @param src - the source position.
 * @param dest - the destination position.
 */
void doMove(Game* game, int src[2], int dest[2]);

/**
 * Check if a certain move is legal and print fitting message if not.
 * @param game - an instant of Game.
 * @param src - the source position.
 * @param dest - the destination position.
 * @return
 * SUCCSESS if the move is legal, otherwise
 * a fitting GAME_MOVE_ERROR.
 */
GAME_MOVE_ERROR checkMove(Game* game, int src[2], int dest[2]);

/*
 * Simply check if the king still(if it was befor) in check or if the move will get him
 * into a check(the king of the player who do the move).
 * @param game - an instant of Game.
 * @param src - the source position.
 * @param dest - the destination position.
 *
 * @return
 * SUCCSESS, if the king is not threatened, otherwise KING_STILL_EXPOSED or
	KING_EXPOSED according to the situation.
 */
GAME_MOVE_ERROR checkKingThreatened(Game* game,  int src[2], int dest[2]);

/**
 * Play a move and updates the Game instant.
 * @param game - the original instmove is legalant of Game.
 * @param src - the source position.
 * @param dest - the destination position.
 * @return
 * SUCCSESS if the move is legal, otherwise
 * a fitting GAME_MOVE_ERROR.
 */
GAME_MOVE_ERROR playMove(Game* game, int src[2], int dest[2]);

/**
 * Undo the last played move and updates the Game instant.
 * @param game - the original instant of Game.
 * @param src - the source position.
 * @param dest - the destination position.
 * @return
 * true if the there is history and the undo worked, otherwise
 * false.
 */
bool undoMove(Game* game, int* src, int* dest);

void resetMatrix(int destinations[MAX_MOVES_PER_PIECE][3]);

/**
 * Save an instant of a Game to file.
 * @param game - the original instant of Game.
 * @param location - the full or relative path of the file.
 * @return
 * false if the the file cannot be opened or modified,
 * otherwise(if saved) true.
 * 
 */
bool saveGame(Game* game, char* location);

/**
 * Load an instant of a Game from file.
 * @param game - the game instant the function will load to(from the state in the save file).
 * @param location - the full or relative path of the file.
 * @return
 * false if the the file does not exist or cannot be opened,
 * otherwise true.
 */
bool loadGame(Game* game, char* location);



#endif /*GAME_H_*/
