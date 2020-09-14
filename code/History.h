#ifndef HISTORY_H_
#define HISTORY_H_

#include <stdbool.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_SIZE 6
#define EMPTY_CHAR '_' 

/**
 * Struct Move defines a move of a player from a source position
 * to a destination position.
 */
typedef struct chess_move {
	int src[2];  // source position
	int dest[2]; // destination position
	char lastKill; // if a piece was killed in the move, its' symbol, otherwise EMPTY_CHAR
} Move;

/**
 * Struct History defines an array of moves with a dynamic
 * size (from 0 to MAX_SIZE) described by actualSize.
 */
typedef struct chess_history {
	Move **moves; // an array of pointers to Move struct
	int actualSize;
} History;

/**
 * A type used for errors
 */
typedef enum chess_history_list_message {
	SUCCESS,
	INVALID_ARGUMENT,
	NULL_HISTORY,
	EMPTY
} HISTORY_LIST_MASSAGE;

/**
 * Creates an empty move with default positions- (0,0).
 * @return
 * NULL if an allocation error has occured, otherwise an instant of a Move.
 */
Move* createEmptyMove();

/**
 * Sets a given move with given parameters.
 * @param move - the given move.
 * @param src - the source position.
 * @param dest - the destination position.
 */
void setMove(Move* move, int* src, int* dest, char lastKill);

/**
 * Creates a move with initials positions.
 * @return
 * NULL if an allocation error has occured, otherwise an instant of a Move.
 */
Move* createMove(int* src, int* dest, char lastKill);

/**
 * Creates an empty history with moves array with MAX_SIZE.
 * @return
 * NULL if an allocation error has occured, otherwise an instant of a History.
 */
History* createHistory();

/**
 * Frees all allocated memory in the instant, and all moves until size.
 * @param history - an instant of History.
 */
void destroyHistorySize(History *history, int size);

/**
 * Frees all allocated memory in the given instant.
 * @param history - an instant of History
 */
void destroyHistory(History *history);

/**
 * Copies the data from one move to another.
 * @param copyTo - the instant that is being changed.
 * @param copyFrom - the instant that is being copied from.
 */
void copyMove(Move *copyTo, Move *copyFrom);

/**
 * Creates an instant of History that is identical to the one given.
 * @param history- the given History instant
 * @return
 * NULL if an allocation error has occured, otherwise an identical copy of history
 */
History* copyHistory(History *history);

/**
 * Checks if the given history is empty.
 * @param history - the given history
 * @return
 * true if there are no availabe moves, false otherwise.
 */
bool isEmpty(History *history);

/**
 * Checks if the given history is full.
 * @param history - the given history
 * @return
 * true if there are exactly 6 availabe moves, false otherwise.
 */
bool isFull(History *history);

/**
 * Inserts a given element's data at the end of the moves array, unless it's full and frees
 * the allocated element.
 * @param history - the given history
 * @param move - the move to add
 * @return
 * NULL_HISTORY - if history is null
 * FULL - if the move array is full
 * SUCCESS otherwise
 */
HISTORY_LIST_MASSAGE insertLast(History *history, Move *move);

/**
 * Creates a Move with the specified src and dest and inserts it to the
 * end of the moves array, unless it's full.
 * @param history - the given history
 * @param src - the source position of the move
 * @param dest- the destination position of the move
 * @return
 * NULL_HISTORY - if history is null
 * FULL - if the move array is full
 * SUCCESS otherwise
 */
HISTORY_LIST_MASSAGE insertLastManual(History *history, int* src, int* dest, char lastKill);

/**
 * Removes a move in the specified index from the moves array that belong to history.
 * @param history - the given history
 * @param index - the specified index
 * @return
 * NULL_HISTORY - if history is null
 * INVALID_ARGUMENT - if index < 0 or index > MAX_SIZE
 * EMPTY - if there are no available moves in history
 * SUCCESS otherwise
 */
HISTORY_LIST_MASSAGE removeAt(History *history, int index);

/**
 * Removes the first element of the moves array in history.
 * @param history - the given history
 * @return
 * NULL_HISTORY - if history is null
 * EMPTY - if there are no available moves in history
 * SUCCESS otherwise
 */
HISTORY_LIST_MASSAGE removeFirst(History *history);

/**
 * Removes the last element of the moves array in history.
 * @param history - the given history
 * @return
 * NULL_HISTORY - if history is null
 * EMPTY - if there are no available moves in history
 * SUCCESS otherwise
 */
HISTORY_LIST_MASSAGE removeLast(History *history);

/**
 * Update the given src & dest arrays to be the last element's src & dest, assuming actualSize >= 1.
 * @param history - the given history
 * @param src - the source to update
 * @param dest - the destination to update
 */
void getTop(History *history, int *src, int *dest);

/**
 * Returns the last kill that occured, or EMPTY_CHAR.
 * @param history - the current history
 * @return - 
 * the last element's lastKill
 */
char getLastKill (History *history);

/**
 * Prints the moves array of history
 * @param history - the given history
 */
void printHistory(History *history);
#endif /* HISTORY_H_ */
