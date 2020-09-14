#include "History.h"

Move* createEmptyMove() {
	Move *move = malloc(sizeof(Move));
	if (move == NULL)
		return NULL;
	move -> src[0] = -1;
	move -> src[1] = -1;
	move -> dest[0] = -1;
	move -> dest[1] = -1;
	move -> lastKill = '_';
	return move;
}
void setMove(Move* move, int* src, int* dest, char lastKill) {
	move -> src[0] = src[0];
	move -> src[1] = src[1];
	move -> dest[0] = dest[0];
	move -> dest[1] = dest[1];
	move -> lastKill = lastKill;
}

Move* createMove(int* src, int* dest, char lastKill) {
	Move *move;
	if ((move = createEmptyMove()) == NULL)
		return NULL;
	setMove(move, src, dest, lastKill);
	return move;
}
History* createHistory() {
	int i;
	History *history = malloc(sizeof(History));
	if (history == NULL) {
		printf("ERROR: createHistory has failed\n");
		return NULL;
	}
	if ((history -> moves = calloc(MAX_SIZE, sizeof(Move))) == NULL) {
		free(history);
		printf("ERROR: createHistory has failed\n");
		return NULL;
	}
	for (i = 0 ; i < MAX_SIZE ; i++) {
		if ((history -> moves[i] = createEmptyMove()) == NULL) {
			destroyHistorySize(history, i);
			return NULL;
		}
	}
	history -> actualSize = 0;
	return history;
}

void destroyHistorySize(History *history, int size) {
	int i;
	if (history == NULL)
		return;
	for (i = 0 ; i < size ; i++) {
		free(history -> moves[i]);
	}
	free(history -> moves);
	free(history);
}

void destroyHistory (History *history) {
	destroyHistorySize(history, MAX_SIZE);
}

void copyMove(Move *copyTo, Move *copyFrom) {
	copyTo -> src[0] = copyFrom -> src[0];
	copyTo -> src[1] = copyFrom -> src[1];
	copyTo -> dest[0] = copyFrom -> dest[0];
	copyTo -> dest[1] = copyFrom -> dest[1];
	copyTo -> lastKill = copyFrom -> lastKill;
}

History* copyHistory(History *history) {
	int i;
	History *cpy = createHistory();
	if (cpy == NULL)
		return NULL;
	cpy -> actualSize = history -> actualSize;
	for (i = 0 ; i < history -> actualSize ; i++) {
		copyMove(cpy -> moves[i], history -> moves[i]);
		if (cpy -> moves[i] == NULL)
			return NULL;
	}
	return cpy;
}

bool isEmpty(History *history) {
	if (history -> actualSize == 0)
		return true;
	return false;
}

bool isFull(History *history) {
	if (history -> actualSize == MAX_SIZE)
		return true;
	return false;
}

HISTORY_LIST_MASSAGE insertLast(History *history, Move *move) {
	if (history == NULL)
		return NULL_HISTORY;
	if (isFull(history))
		removeFirst(history);
	copyMove(history -> moves[history -> actualSize] , move);
	history -> actualSize ++;
	free(move); // free the temporary element
	return SUCCESS;
}

HISTORY_LIST_MASSAGE insertLastManual(History *history, int* src, int* dest, char lastKill) {
	if (history == NULL)
		return NULL_HISTORY;
	if (isFull(history)) {
		removeFirst(history);
	}
	setMove(history -> moves[history -> actualSize], src, dest, lastKill);
	history -> actualSize ++;	
	return SUCCESS;
}

HISTORY_LIST_MASSAGE removeAt(History *history, int index) {
	if ((index < 0) || (index >= history -> actualSize))
		return INVALID_ARGUMENT;
	if (history == NULL)
		return NULL_HISTORY;
	if (isEmpty(history))
		return EMPTY;
	for (; index < (history -> actualSize) - 1 ; index++) {
		copyMove(history -> moves[index] , history -> moves[index+1]);
	}
	//free(history -> moves[history -> actualSize - 1]);
	history -> actualSize --;
	return SUCCESS;
}

HISTORY_LIST_MASSAGE removeFirst(History *history) {
	return removeAt(history, 0);
}

HISTORY_LIST_MASSAGE removeLast(History *history) {
	return removeAt(history, history -> actualSize - 1);
}

void getTop(History *history, int *src, int *dest) {
	src[0] = history-> moves[history->actualSize -1]-> src[0];
	src[1] = history-> moves[history->actualSize -1]->src[1];
	dest[0] = history-> moves[history->actualSize -1]->dest[0];
	dest[1] = history-> moves[history->actualSize -1]->dest[1];
}

char getLastKill (History *history) {
	return history-> moves[history->actualSize -1]-> lastKill; 
}

void printHistory(History *history) {
	int i;
	for (i = 0 ; i < history -> actualSize ; i++) {
		printf("%d: (%d,%d) -> (%d,%d) ;", i,history->moves[i]->src[0],history->moves[i]->src[1],history->moves[i]->dest[0],history->moves[i]->dest[1]);
	}
	printf("\n");
}
