#include "Game.h"

int checkPlayer(char piece){
	if(piece == EMPTY_CHAR)
		return -1;
	if((piece > 'A') && (piece < 'Z'))
		return BLACK_PLAYER;
	else
		return WHITE_PLAYER;
}

Game* allocateGame(){
	// Allocating memory for the game structure
	Game* game = malloc(sizeof(Game));
	if (game == NULL){
		printf("ERROR: allocateGame has failed");
		return NULL;
	}
	if ((game -> history = createHistory()) == NULL){
		free(game);
		return NULL;
	}
	return game;
}

Game* createGame(int mode,  int difficulty, int color){
	Game* game = allocateGame();
	if(game == NULL){
		return NULL;
	}
	initializeBoard(game);
	// Mode and setting initialization
	game -> gameMode = mode;
	game -> difficulty = difficulty;
	game -> userColor = color;
	game -> state = GAME;
	game -> currentPlayer = WHITE_PLAYER; // White always start
	return game;
}

void initializeBoard(Game *game) {
	int i, j;
	// Initializing the board
	game -> gameBoard[0][0] = 'R';
	game -> gameBoard[0][1] = 'N';
	game -> gameBoard[0][2] = 'B';
	game -> gameBoard[0][4] = 'K';
	game -> gameBoard[0][3] = 'Q';
	game -> gameBoard[0][7] = 'R';
	game -> gameBoard[0][6] = 'N';
	game -> gameBoard[0][5] = 'B';
	for(i = 1; i < ROWS-1; i++){
		for(j = 0; j<COLUMNS; j++){
			if(i == 1)
				game -> gameBoard[i][j] = 'M';
				else if(i == (ROWS-2))
				game -> gameBoard[i][j] = 'm';
			else 
				game -> gameBoard[i][j] = EMPTY_CHAR;
		}
	}
	game -> gameBoard[ROWS-1][0] = 'r';
	game -> gameBoard[ROWS-1][1] = 'n';
	game -> gameBoard[ROWS-1][2] = 'b';
	game -> gameBoard[ROWS-1][4] = 'k';
	game -> gameBoard[ROWS-1][3] = 'q';
	game -> gameBoard[ROWS-1][7] = 'r';
	game -> gameBoard[ROWS-1][6] = 'n';
	game -> gameBoard[ROWS-1][5] = 'b';
	// Saving the location of the kings
	game -> kings[BLACK_PLAYER][0] = 0; // Black king - row 
	game -> kings[BLACK_PLAYER][1] = 4; // Black king - column
	game -> kings[WHITE_PLAYER][0] = ROWS-1; // Black king - row 
	game -> kings[WHITE_PLAYER][1] = 4; // Black king - column

	// initializing history
	game->history->actualSize = 0;
}

void destroyGame(Game* game){
	if (game == NULL)
		return;
	destroyHistory(game -> history);
	free(game);
	game = NULL;
}

Game* copyGame(Game* game){
	int i,j;
	Game* newGame = allocateGame();
	if(newGame == NULL)
		return NULL;
	for(i = 0; i < ROWS; i++){
		for(j = 0; j<COLUMNS; j++){
			newGame -> gameBoard[i][j] = game -> gameBoard[i][j];
		}
	}
	for(i = 0; i <= 1; i++){
		for(j = 0; j <= 1; j++){
			newGame -> kings[i][j] = game -> kings[i][j]; 
		}
	}
	newGame -> gameMode = game -> gameMode;
	newGame -> difficulty = game -> difficulty;
	newGame -> userColor = game -> userColor;
	newGame -> state = game -> state;
	newGame -> currentPlayer = game -> currentPlayer;
	return newGame;
}

bool available(Game* game, int row, int col, int player){
	int piece;
	if(game -> gameBoard[row][col] != EMPTY_CHAR){
		piece = (game -> gameBoard[row][col]) - ('A'+ (player)*('a'-'A'));
		if((piece < 0) || (piece > 25))
			return true; // return true if it does not contain a piece of the user's color (meaning it contains an enemy piece)
		else
			return false;
	}
	return true;
}

bool checkBishop(Game* game,  int src[2], int dest[2]){
	int rows, cols, incRows = 1, incCols = 1;
	rows = (dest[0]) - (src[0]);
	cols = (dest[1]) - (src[1]);
	if(cols < 0)
		incCols = -1;
	if(rows < 0)
		incRows = -1;
	if((rows != cols) && (rows != (-cols)))
		return false;
	for(rows = (src[0] + incRows), cols = (src[1] + incCols); rows != (dest[0]); rows += incRows, cols += incCols){
		if(game -> gameBoard[rows][cols] != EMPTY_CHAR)
			return false;
	}
	// Checks If position <i,j>(final position) does not contain a piece of the user's color
	return available(game,rows,cols,checkPlayer(game->gameBoard[src[0]][src[1]]));
}

bool checkKing(Game* game,  int src[2], int dest[2]){
	int distance = (src[0]) - (dest[0]);
	if((distance > 1) || (distance < -1))
		return false;

	distance = (src[1]) - (dest[1]);
	if((distance > 1) || (distance < -1))
		return false;

	return available(game,(dest[0]),(dest[1]),checkPlayer(game->gameBoard[src[0]][src[1]]));
}

bool checkPawn(Game* game,  int src[2], int dest[2]){
	int rows,cols, inc;
	int player = checkPlayer(game->gameBoard[src[0]][src[1]]);
	inc = 1 - 2* (player); // 1 if player is black and -1 if it's white
	rows = ((src[0]) - (dest[0]))*(-inc); // we fix rows so it will always be positive for the player
	cols = (src[1]) - (dest[1]);
	if((rows < -2) || (rows > 2) || (cols < -1) || (cols > 1))
		return false;
	if(rows == 2){
		if(cols != 0)
			return false;
		// Checks if the pawn is located at its starting position
		if((player == BLACK_PLAYER) && (src[0] != 1))
			return false;
		if((player == WHITE_PLAYER) && (src[0] != (ROWS -2)))
			return false;
		// Checks that the path is not blocked
		if(game -> gameBoard[(src[0] + inc)][src[1]] != EMPTY_CHAR)
			return false;
		if(game -> gameBoard[dest[0]][dest[1]] != EMPTY_CHAR)
			return false;
	}
	else if(rows == 1){
		if((cols == -1) || (cols == 1)){ // Checks if the capturing attempt is legal
			if(game -> gameBoard[dest[0]][dest[1]] == EMPTY_CHAR)
				return false;
			return available(game,(dest[0]),(dest[1]),player);
		}
		if(cols == 0)
			return (game -> gameBoard[dest[0]][dest[1]] == EMPTY_CHAR);
	}
	else{
		return false;
	}
	return true;
}

bool checkKnight(Game* game,  int src[2], int dest[2]){
	int rows,cols;
	rows = (src[0]) - (dest[0]);
	cols = (src[1]) - (dest[1]);
	if((rows < -2) || (rows > 2) || (cols < -2) || (cols > 2))
		return false;
	if((rows == -2) || (rows == 2)){ // Checks vertically movement
		if((cols != -1) && (cols != 1))
			return false;
		else
			return available(game,dest[0],dest[1],checkPlayer(game->gameBoard[src[0]][src[1]]));
	}
	if((cols == -2) || (cols == 2)){ // Checks horizontally movement
		if((rows != -1) && (rows != 1))
			return false;
		else
			return available(game,dest[0],dest[1],checkPlayer(game->gameBoard[src[0]][src[1]]));
	}
	return false;

}

bool checkRook(Game* game,  int src[2], int dest[2]){
	int rows, cols, inc = 1;
	rows = (dest[0]) - (src[0]);
	cols = (dest[1]) - (src[1]);
	if((cols !=0) && (rows != 0)) // The movement is either completely horizontal or vertical
		return false;

	if(rows != 0){ // Checks vertical movement
		if(rows < 0)
			inc = -1;
		for(rows = (src[0] + inc); rows != (dest[0]); rows += inc){
			if(game -> gameBoard[rows][src[1]] != EMPTY_CHAR)
				return false;
		}
	}
	else{ // Checks horizontal movement
		if(cols < 0)
			inc = -1;
		for(cols = (src[1] + inc); cols != (dest[1]); cols += inc){
			if(game -> gameBoard[src[0]][cols] != EMPTY_CHAR)
				return false;
		}
	}
	// Checks If position <i,j>(final position) does not contain a piece of the user's color
	return available(game,dest[0],dest[1],checkPlayer(game->gameBoard[src[0]][src[1]]));
}

bool checkQueen(Game* game,  int src[2], int dest[2]){
	if((checkRook(game,src,dest)) || (checkBishop(game,src,dest)))
		return true;
	else
		return false;
}

bool checkThreat(Game* game, int row, int col){
	int i, j, player = checkPlayer(game->gameBoard[row][col]);
	char piece;
	/*********************************Check the current column for threats*******************************************/
	for(i = row+1; i < ROWS; i++){
		piece = game -> gameBoard[i][col];
		if(piece == EMPTY_CHAR)
			continue;
		// check if the king is defended by one of the player's units or one of his enemy units
		// (for example - a pawn in front of the king will be unable to harm him)
		if((player == checkPlayer(piece)) || ((piece != ('R'+(1-player)*('a'-'A'))) && (piece != ('Q'+(1-player)*('a'-'A')))))
			break;
		if(piece != EMPTY_CHAR){
			if((player == WHITE_PLAYER) && ((piece == 'R') || (piece == 'Q')))
				return true;
			if((player == BLACK_PLAYER) && ((piece == 'r') || (piece == 'q')))
				return true;
		}
	}
	for(i = row-1; i >= 0; i--){
		piece = game -> gameBoard[i][col];
		if(piece == EMPTY_CHAR)
			continue;
		// check if the king is defended by one of the player's units or one of his enemy units
		if((player == checkPlayer(piece)) || ((piece != ('R'+(1-player)*('a'-'A'))) && (piece != ('Q'+(1-player)*('a'-'A')))))
			break;
		if(piece != EMPTY_CHAR){
			if((player == WHITE_PLAYER) && ((piece == 'R') || (piece == 'Q')))
				return true;
			if((player == BLACK_PLAYER) && ((piece == 'r') || (piece == 'q')))
				return true;
		}
	}
	/*********************************Check the current row for threats*******************************************/
	for(i = col+1; i < COLUMNS; i++){
		piece = game -> gameBoard[row][i];
		if(piece == EMPTY_CHAR)
			continue;
		// check if the king is defended by one of the player's units or one of his enemy units
		if((player == checkPlayer(piece)) || ((piece != ('R'+(1-player)*('a'-'A'))) && (piece != ('Q'+(1-player)*('a'-'A')))))
			break;
		if(piece != EMPTY_CHAR){
			if((player == WHITE_PLAYER) && ((piece == 'R') || (piece == 'Q')))
				return true;
			if((player == BLACK_PLAYER) && ((piece == 'r') || (piece == 'q')))
				return true;
		}
	}
	for(i = col-1; i >= 0; i--){
		piece = game -> gameBoard[row][i];
		if(piece == EMPTY_CHAR)
			continue;
		// check if the king is defended by one of the player's units or one of his enemy units
		if((player == checkPlayer(piece)) || ((piece != ('R'+(1-player)*('a'-'A'))) && (piece != ('Q'+(1-player)*('a'-'A')))))
			break;
		if(piece != EMPTY_CHAR){
			if((player == WHITE_PLAYER) && ((piece == 'R') || (piece == 'Q')))
				return true;
			if((player == BLACK_PLAYER) && ((piece == 'r') || (piece == 'q')))
				return true;
		}
	}
	/*********************************Check the diagonals for threats*******************************************/
	for(i = row+1, j = col+1; ((i < ROWS) && (j < COLUMNS)); i++, j++){
		piece = game -> gameBoard[i][j];
		if(piece == EMPTY_CHAR)
			continue;
		// check if the king is defended by one of the player's units or one of his enemy units
		if((player == checkPlayer(piece)) || ((piece != ('B'+(1-player)*('a'-'A'))) && (piece != ('Q'+(1-player)*('a'-'A')))))
			break;
		if(piece != EMPTY_CHAR){
			if((player == WHITE_PLAYER) && ((piece == 'B') || (piece == 'Q')))
				return true;
			if((player == BLACK_PLAYER) && ((piece == 'b') || (piece == 'q')))
				return true;
		}
	}
	for(i = row-1, j = col-1; ((i >= 0) && (j >= 0)); i--, j--){
		piece = game -> gameBoard[i][j];
		if(piece == EMPTY_CHAR)
			continue;
		// check if the king is defended by one of the player's units or one of his enemy units
		if((player == checkPlayer(piece)) || ((piece != ('B'+(1-player)*('a'-'A'))) && (piece != ('Q'+(1-player)*('a'-'A')))))
			break;
		if(piece != EMPTY_CHAR){
			if((player == WHITE_PLAYER) && ((piece == 'B') || (piece == 'Q')))
				return true;
			if((player == BLACK_PLAYER) && ((piece == 'b') || (piece == 'q')))
				return true;
		}
	}
	for(i = row+1, j = col-1; ((i < ROWS) && (j >= 0)); i++, j--){
		piece = game -> gameBoard[i][j];
		if(piece == EMPTY_CHAR)
			continue;
		// check if the king is defended by one of the player's units or one of his enemy units
		if((player == checkPlayer(piece)) || ((piece != ('B'+(1-player)*('a'-'A'))) && (piece != ('Q'+(1-player)*('a'-'A')))))
			break;
		if(piece != EMPTY_CHAR){
			if((player == WHITE_PLAYER) && ((piece == 'B') || (piece == 'Q')))
				return true;
			if((player == BLACK_PLAYER) && ((piece == 'b') || (piece == 'q')))
				return true;
		}
	}
	for(i = row-1, j = col+1; ((i >= 0) && (j < COLUMNS)); i--, j++){
		piece = game -> gameBoard[i][j];
		if(piece == EMPTY_CHAR)
			continue;
		// check if the king is defended by one of the player's units or one of his enemy units
		if((player == checkPlayer(piece)) || ((piece != ('B'+(1-player)*('a'-'A'))) && (piece != ('Q'+(1-player)*('a'-'A')))))
			break;
		if(piece != EMPTY_CHAR){
			if((player == WHITE_PLAYER) && ((piece == 'B') || (piece == 'Q')))
				return true;
			if((player == BLACK_PLAYER) && ((piece == 'b') || (piece == 'q')))
				return true;
		}
	}
	/***************************Checking for pawns in front of the king(and diagonal)*****************************/
	if((row > 0) && (player == WHITE_PLAYER)){
		if(col > 0)
			if(game -> gameBoard[row-1][col-1] == 'M')
				return true;
		if(col < (COLUMNS-1))
			if(game -> gameBoard[row-1][col+1] == 'M')
				return true;
	}
	if((row < (ROWS-1)) && (player == BLACK_PLAYER)){
		if(col > 0)
			if(game -> gameBoard[row+1][col-1] == 'm')
				return true;
		if(col < (COLUMNS-1))
			if(game -> gameBoard[row+1][col+1] == 'm')
				return true;
	}
	/********************************************Checking for Knights*********************************************/
	piece = EMPTY_CHAR;
	if((row+2 )< ROWS){
		if(col < (COLUMNS-1)){
			piece = game -> gameBoard[row+2][col+1];
			if(((player == WHITE_PLAYER) && (piece == 'N')) || ((player == BLACK_PLAYER) && (piece == 'n')))
				return true;
		}
		if(col-1 >= 0){
			piece = game -> gameBoard[row+2][col-1];
			if(((player == WHITE_PLAYER) && (piece == 'N')) || ((player == BLACK_PLAYER) && (piece == 'n')))
				return true;
		}
	}
	if((row-2) >= 0){
		if(col < (COLUMNS-1)){
			piece = game -> gameBoard[row-2][col+1];
			if(((player == WHITE_PLAYER) && (piece == 'N')) || ((player == BLACK_PLAYER) && (piece == 'n')))
				return true;
		}
		if(col-1 >= 0){
			piece = game -> gameBoard[row-2][col-1];
			if(((player == WHITE_PLAYER) && (piece == 'N')) || ((player == BLACK_PLAYER) && (piece == 'n')))
				return true;
		}
	}
	if((col+2) < COLUMNS){
		if(row < (ROWS-1)){
			piece = game -> gameBoard[row+1][col+2];
			if(((player == WHITE_PLAYER) && (piece == 'N')) || ((player == BLACK_PLAYER) && (piece == 'n')))
				return true;
		}
		if(row-1 >= 0){
			piece = game -> gameBoard[row-1][col+2];
			if(((player == WHITE_PLAYER) && (piece == 'N')) || ((player == BLACK_PLAYER) && (piece == 'n')))
				return true;
		}
	}
	if((col-2) >= 0){
		if(row < (ROWS-1)){
			piece = game -> gameBoard[row+1][col-2];
			if(((player == WHITE_PLAYER) && (piece == 'N')) || ((player == BLACK_PLAYER) && (piece == 'n')))
				return true;
		}
		if(row-1 >= 0){
			piece = game -> gameBoard[row-1][col-2];
			if(((player == WHITE_PLAYER) && (piece == 'N')) || ((player == BLACK_PLAYER) && (piece == 'n')))
				return true;
		}
	}
	/********************************************Checking for king next to king****************************************/
	if(game ->kings[WHITE_PLAYER][0] == game ->kings[BLACK_PLAYER][0]){
		if(game ->kings[WHITE_PLAYER][1] == (game ->kings[BLACK_PLAYER][1]-1))
			return true;
		if(game ->kings[WHITE_PLAYER][1] == (game ->kings[BLACK_PLAYER][1]+1))
			return true;
	}
	if(game ->kings[WHITE_PLAYER][1] == game ->kings[BLACK_PLAYER][1]){
		if(game ->kings[WHITE_PLAYER][0] == (game ->kings[BLACK_PLAYER][0]-1))
			return true;
		if(game ->kings[WHITE_PLAYER][0] == (game ->kings[BLACK_PLAYER][0]+1))
			return true;
	}
	return false;
}

bool move(Game* game, int src[2], int dest[2], int* moveType, int stateNum){
	char tmp;
	*moveType = 0;
	if(checkPlayer(game -> gameBoard[src[0]][src[1]]) == checkPlayer(game -> gameBoard[dest[0]][dest[1]]))
		return false;
	if(dest[0] > 7 || dest[0] < 0 || dest[1] > 7 || dest[1] < 0)
		return false;
	GAME_MOVE_ERROR result = checkKingThreatened(game, src, dest);
	if(result == SUCCSESS){
		if(stateNum == 1)
			return true;
		tmp = game -> gameBoard[dest[0]][dest[1]];
		game -> gameBoard[dest[0]][dest[1]] = game -> gameBoard[src[0]][src[1]]; 
		game -> gameBoard[src[0]][src[1]] = EMPTY_CHAR;
		if(checkThreat(game,dest[0],dest[1])){ // Case 2: If the move is threatened by an opponent 
			*moveType += 1;
		}
		game -> gameBoard[src[0]][src[1]] = game -> gameBoard[dest[0]][dest[1]];
		game -> gameBoard[dest[0]][dest[1]] = tmp;
		if(game -> gameBoard[dest[0]][dest[1]] != EMPTY_CHAR){
			*moveType += 2; // Case 3/4 : If the move captures a piece / If the move captures a piece and is threatened
		}
		return true;
	}
	return false;
}

void printMoveType(int dest[2], int moveType){
	printf("<%d,%c>",ROWS-dest[0], ('A'+dest[1]));
	switch(moveType){
		case 1:
			printf("*");
			break;
		case 2:
			printf("^");
			break;
		case 3:
			printf("*^");
			break;
		}
		printf("\n");
}



// Merges two subarrays of arr[].
// First subarray is arr[l..m]
// Second subarray is arr[m+1..r]
void merge(int arr[28][3], int l, int m, int r){
    int i, j, k, w;
    int n1 = m - l + 1;
    int n2 =  r - m;

    /* create temp arrays */
    int L[n1][3], R[n2][3];

    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++){
    	for(w = 0; w < 3; w++)
    		L[i][w] = arr[l + i][w];
    }
    for (j = 0; j < n2; j++)
    	for(w = 0; w < 3; w++)
    		R[j][w] = arr[m + 1+ j][w];

    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2){
        if ((L[i][1] < R[j][1]) || ((L[i][1] == R[j][1]) && (L[i][0] > R[j][0]))){
        	for(w = 0; w < 3; w++)
        		arr[k][w] = L[i][w];
            i++;
        }
        else{
        	for(w = 0; w < 3; w++)
        		arr[k][w] = R[j][w];
            j++;
        }
        k++;
    }

    /* Copy the remaining elements of L[], if there
       are any */
    while (i < n1){
    	for(w = 0; w < 3; w++)
    		arr[k][w] = L[i][w];
        i++;
        k++;
    }

    /* Copy the remaining elements of R[], if there
       are any */
    while (j < n2){
    	for(w = 0; w < 3; w++)
    		arr[k][w] = R[j][w];
        j++;
        k++;
    }
}

/* l is for left index and r is right index of the
   sub-array of arr to be sorted */
void mergeSort(int arr[28][3], int l, int r){
    if (l < r){
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l+(r-l)/2;

        // Sort first and second halves
        mergeSort(arr, l, m);
        mergeSort(arr, m+1, r);

        merge(arr, l, m, r);
    }
}

void sortDests(int dests[28][3]){
	int len = 0;
	while(dests[len][0] != -1)
		len++;
	mergeSort(dests,0,len-1);
}

bool bishopRookMoves(Game* game, int row, int col, int stateNum, bool bishopNotRook, int dests[28][3]){
	bool legal = false, canMove = false;
	int dest[2], src[2] , moveType, i = 1, incRow = 1 ,incCol = 0, moveNum = 0;
	if(bishopNotRook){
		incRow = 1;
		incCol = 1;
	}
	src[0] = row;
	src[1] = col;
	dest[0] = row+incRow;
	dest[1] = col+incCol;
	// the loop is in use basically only for the queen
	// (if there were diagonal moves for example then the array won't be empty when we check horizontal moves)
	while(dests[moveNum][0] != -1)
		moveNum++;
	while(i <= 4){
		legal = false;
		if( (dest[0] < ROWS) && (dest[1] < COLUMNS) && (dest[0] >= 0) && (dest[1] >= 0) ){
			if(available(game,dest[0],dest[1],checkPlayer(game->gameBoard[row][col])))
				legal = move(game,src,dest,&moveType,stateNum);
			if(legal){
				canMove = true;
				if(stateNum == 1)
					return true;
				else{
					if(stateNum == 0)
						printMoveType(dest,moveType);
					// saving all the possible moves for minimax
					else{
						dests[moveNum][0] = dest[0];
						dests[moveNum][1] = dest[1];
						dests[moveNum][2] = moveType;
						moveNum++;
					}
					// updating the next destination to check
					dest[0] += incRow;
					dest[1] += incCol;
					if((moveType == 0) || (moveType == 1))
						continue;
				}
			}
		}
		i++;
		switch(i){
			case 2: 
				if(bishopNotRook)
					incCol = -1; // incRow = 1, incCol = -1 meaning that the diagonal is  col--,row++
				else
					incRow = -1;
				break;
			case 3:
				if(bishopNotRook)
					incRow = -1;
				else{
					incRow = 0;
					incCol = 1;
				}
				break;
			case 4:
				if(bishopNotRook)
					incCol = 1;
				else
					incCol = -1;
				break;
		}
		dest[0] = row+incRow;
		dest[1] = col+incCol;
	}
	sortDests(dests);
	return canMove;
}

bool kingMoves(Game* game, int row, int col, int stateNum, int dests[28][3]){
	int dest[2], src[2], i, j, moveType, moveNum = 0;
	src[0] = row;
	src[1] = col;
	bool legal = false, canMove = false;
	for(i = -1; i <= 1; i++){
		dest[1] = col+i;
		for(j = 1; j >= -1; j--){
			dest[0] = row+j;
			if( (dest[0] < ROWS) && (dest[1] < COLUMNS) && (dest[0] >= 0) && (dest[1] >= 0) ){
				legal = move(game,src,dest,&moveType,stateNum);
				if(legal){
					canMove = true;
					if(stateNum == 1)
						return true;
					else if(stateNum == 0)
						printMoveType(dest,moveType);
					// saving all the possible moves for minimax
					else{
						dests[moveNum][0] = dest[0];
						dests[moveNum][1] = dest[1];
						dests[moveNum][2] = moveType;
						moveNum++;
					}
				}
			}
		}
	}
	return canMove;
}

bool queenMoves(Game* game, int row, int col, int stateNum, int dests[28][3]){
	bool legalRook = false ,legalBishop = false;
	int m = 0, r = 0;
	legalBishop = bishopRookMoves(game,row,col,stateNum,true,dests);
	if(legalBishop && (stateNum == 1))
		return true;
	while(dests[m][0] != -1)
		m++;
	legalRook = bishopRookMoves(game,row,col,stateNum,false,dests);
	if(legalRook && (stateNum == 1))
		return true;
	r = m;
	while(dests[r][0] != -1)
		r++;
	if((m > 0) && (r != m))
		merge(dests,0,m-1,r-1);
	return (legalRook || legalBishop);
}

bool pawnMoves(Game* game, int row, int col, int stateNum, int dests[28][3]){
	bool legal = false, canMove = false;
	int dest[2], src[2], moveType, i = 1, inc = 1, moveNum = 0;
	int player = checkPlayer(game->gameBoard[row][col]);
	if(player == WHITE_PLAYER)
		inc = -1;
	src[0] = row;
	src[1] = col;
	dest[0] = row + inc;
	dest[1] = col - 1;
	while(i <= 4){
		if(checkPawn(game,src,dest)){
			legal = move(game,src,dest,&moveType,stateNum);
			if(legal){
				canMove = true;
				if(stateNum == 1)
					return true;
				else if(stateNum == 0)
					printMoveType(dest,moveType);
				// saving all the possible moves for minimax
				else{
					dests[moveNum][0] = dest[0];
					dests[moveNum][1] = dest[1];
					dests[moveNum][2] = moveType;
					moveNum++;
				}
			}
		}
		i++;
		switch(i){
		case 2:
			dest[1] = col;
			if(player == BLACK_PLAYER)
				dest[0] = row + 2*inc;
			break;
		case 3:
			if(player == BLACK_PLAYER)
				dest[0] = row + inc;
			else
				dest[0] = row + 2*inc;
			break;
		case 4:
			dest[0] = row + inc;
			dest[1] = col + 1;
			break;
		}
	}
	return canMove;
}

bool knightMoves(Game* game, int row, int col, int stateNum, int dests[28][3]){
	bool legal = false, canMove = false;
	int dest[2], src[2], moveType, i = 1, moveNum = 0;
	src[0] = row;
	src[1] = col;
	for(i = 1; i <= 8; i++){
		switch(i){
		case 1:
			dest[1] = col - 2;
			dest[0] = row + 1;
			break;
		case 2:
			dest[0] = row - 1;
			break;
		case 3:
			dest[1] = col - 1;
			dest[0] = row + 2;
			break;
		case 4:
			dest[0] = row - 2;
			break;
		case 5:
			dest[1] = col + 1;
			dest[0] = row + 2;
			break;
		case 6:
			dest[0] = row - 2;
			break;
		case 7:
			dest[1] = col + 2;
			dest[0] = row + 1;
			break;
		case 8:
			dest[0] = row - 1;
			break;
		}
		if( (dest[0] < ROWS) && (dest[1] < COLUMNS) && (dest[0] >= 0) && (dest[1] >= 0) ){
			legal = move(game,src,dest,&moveType,stateNum);
			if(legal){
				canMove = true;
				if(stateNum == 1)
					return true;
				else if(stateNum == 0)
					printMoveType(dest,moveType);
				// saving all the possible moves for minimax
				else{
					dests[moveNum][0] = dest[0];
					dests[moveNum][1] = dest[1];
					dests[moveNum][2] = moveType;
					moveNum++;
				}
			}
		}
	}
	return canMove;

}

bool getMoves(Game* game, int row, int col, int stateNum, int dests[28][3]){
	int player = checkPlayer(game -> gameBoard[row][col]);
	int piece; // The numerical value of the piece(or possibly EMPTY_CHAR spot) using the ascii table
	bool moveExist = false;
	// Checks If either one of the locations is invalid
	if((row < 0) || (col > 7)){
		printf( "Invalid position on the board\n");
		return false;
	}

	// Checks If position <x,y>(initial position) does not contain a piece of the user's color
	piece = (game -> gameBoard[row][col])-('A'+ (player)*('a'-'A'));
	if(player == -1){
		printf("The specified position does not contain a player piece\n");
		return false;
	}
	// Checks If the move is illegal for the piece
	switch(piece){
		case 1:
			moveExist = bishopRookMoves(game,row,col,stateNum,true,dests); // Checking for bishop
			break;
		case 10:
			moveExist = kingMoves(game,row,col,stateNum,dests);
			break;
		case 12:
			moveExist = pawnMoves(game,row,col,stateNum,dests);
			break;
		case 13:
			moveExist = knightMoves(game,row,col,stateNum,dests);
			break;
		case 16:
			moveExist = queenMoves(game,row,col,stateNum,dests);
			break;
		case 17:
			moveExist = bishopRookMoves(game,row,col,stateNum,false,dests); // Checking for rook
			break;
	}
	return moveExist;
}

STATE checkState(Game* game){
	int row, col, empty[28][3];
	resetMatrix(empty);
	bool threatened, hasMoves = false;
	threatened = checkThreat(game, game ->kings[game ->currentPlayer][0], game ->kings[game ->currentPlayer][1]);
	for(row = 0; row < ROWS; row++){
		for(col = 0; col < COLUMNS; col++){
			if(!available(game,row,col,game ->currentPlayer)){ // Meaning if <row,col> is not empty and doesn't have an enemy unit there.
				hasMoves = getMoves(game,row,col,1,empty);
				if(hasMoves)
					break;
			}
		}
		if(hasMoves)
			break;
	}
	if(!threatened){
		if(hasMoves)
			return GAME;
		else
			return DRAW;
	}
	if(hasMoves){ // At this point it's clear that threatened = false
		if(game -> currentPlayer == BLACK_PLAYER)
			return BLACK_CHECKED;
		else
			return WHITE_CHECKED;
	}
	// If we reached here it means there are no legal moves and the king is threatened therefore the current player lost.
	if(game -> currentPlayer == BLACK_PLAYER)
		return BLACK_LOSE;
	else
		return WHITE_LOSE;
}

void doMove(Game* game, int src[2], int dest[2]){
	if(game -> gameBoard[src[0]][src[1]] == 'k'){
		game ->kings[WHITE_PLAYER][0] = dest[0];
		game ->kings[WHITE_PLAYER][1] = dest[1];
	}
	else if(game -> gameBoard[src[0]][src[1]] == 'K'){
		game ->kings[BLACK_PLAYER][0] = dest[0];
		game ->kings[BLACK_PLAYER][1] = dest[1];
	}
	insertLastManual(game -> history,src,dest,game -> gameBoard[dest[0]][dest[1]]);
	game -> gameBoard[dest[0]][dest[1]] = game -> gameBoard[src[0]][src[1]];
	game -> gameBoard[src[0]][src[1]] = EMPTY_CHAR;
	game -> currentPlayer = (1 - game -> currentPlayer);
}

GAME_MOVE_ERROR checkMove(Game* game,  int src[2], int dest[2]){
	int player;
	int piece; // The numerical value of the piece(or possibly EMPTY_CHAR spot) using the ascii table
	bool legal = false; // Boolean value for validating the legitimacy of a move

	// Checks If either one of the locations is invalid
	if((src[0] < 0) || (src[0] > 7) || (dest[0] < 0) || (dest[0] > 7)|| (src[1] < 0) || (src[1] > 7) || (dest[1] < 0) || (dest[1] > 7)){
		return INVALID_POSITION;
	}
	player = checkPlayer(game -> gameBoard[src[0]][src[1]]);
	// Checks If position <x,y>(initial position) does not contain a piece of the user's color
	piece = (game -> gameBoard[src[0]][src[1]])-('A'+ (game -> currentPlayer)*('a'-'A'));
	if((player == -1) || (piece < 0) || (piece > 25)){
		return EMPTY_POSITION;
	}
	// Checks If the move is illegal for the piece
	switch(piece){
		case 1:
			legal = checkBishop(game,src,dest);
			break;
		case 10:
			legal = checkKing(game,src,dest);
			break;
		case 12:
			legal = checkPawn(game,src,dest);
			break;
		case 13:
			legal = checkKnight(game,src,dest);
			break;
		case 16:
			legal = checkQueen(game,src,dest);
			break;
		case 17:
			legal = checkRook(game,src,dest);
			break;
		}
	if(legal == false){
		return ILLEGAL_MOVE;
	}
	// Checks the situation of the king before and after the move
	return checkKingThreatened(game,src,dest);
}

GAME_MOVE_ERROR checkKingThreatened(Game* game,  int src[2], int dest[2]){
	int player = checkPlayer(game -> gameBoard[src[0]][src[1]]);
	GAME_MOVE_ERROR result = SUCCSESS;
	Game* tmpGame = copyGame(game);
	if(tmpGame == NULL){// Allocation error!
		destroyGame(game);
		return SUCCSESS; // Not really
	}
	doMove(tmpGame, src, dest);
	if(checkThreat(tmpGame, tmpGame -> kings[player][0],tmpGame -> kings[player][1])){
		if(game -> currentPlayer == WHITE_PLAYER)
			tmpGame -> state = WHITE_CHECKED;
		else
			tmpGame -> state = BLACK_CHECKED;

		if(game -> currentPlayer == WHITE_PLAYER){
			if(tmpGame -> state == WHITE_CHECKED){
				if(game -> state == WHITE_CHECKED)
					result = KING_STILL_EXPOSED;
				else
					result = KING_EXPOSED;// Checks if the move causes the player's king to be threatened
			}
		}
		else{
			if(tmpGame -> state == BLACK_CHECKED){
				if(game -> state == BLACK_CHECKED)
					result = KING_STILL_EXPOSED;
				else
					result = KING_EXPOSED;// Checks if the move causes the player's king to be threatened
			}
		}
	}
	destroyGame(tmpGame);
	return result;
}

GAME_MOVE_ERROR playMove(Game* game, int src[2], int dest[2]){
	GAME_MOVE_ERROR e = checkMove(game,src,dest);
	if(e == SUCCSESS){
		doMove(game,src,dest);
		game -> state = checkState(game);
	}
	return e;
}

bool undoMove(Game* game, int* src, int* dest){
	if(isEmpty(game -> history))
		return false;
	getTop(game -> history,src,dest);
	// Updating the board
	game -> gameBoard[src[0]][src[1]] = game -> gameBoard[dest[0]][dest[1]];
	game -> gameBoard[dest[0]][dest[1]] = getLastKill(game -> history);
	// Updating the king location
	if(game -> gameBoard[src[0]][src[1]] == 'K'){
		game -> kings[BLACK_PLAYER][0] = src[0];
		game -> kings[BLACK_PLAYER][1] = src[1];
	}
	else if(game -> gameBoard[src[0]][src[1]] == 'k'){
		game -> kings[WHITE_PLAYER][0] = src[0];
		game -> kings[WHITE_PLAYER][1] = src[1];
	}
	// Update history
	removeLast(game -> history);
	// Updade game fields
	game -> currentPlayer = 1 - (game -> currentPlayer);
	game -> state = checkState(game);
	return true;
}

bool saveGame(Game* game, char* location){
	int row, col;
	char* str;
	FILE* saveFile = fopen(location,"w");
	// making sure the file was created/opened properly
	if(saveFile == NULL){
		//printf("File cannot be created or modified\n"); ????!!?!?!??!?!?!?!?!?! what about GUI ?!?
		return false;
	}
	// saving current player
	if(game -> currentPlayer == WHITE_PLAYER)
		fprintf(saveFile,"white\n");
	else
		fprintf(saveFile,"black\n");

	fprintf(saveFile,"SETTINGS:\n");
	fprintf(saveFile,"GAME_MODE: %d-player\n",game->gameMode);
	// saving the single player fields(unneeded if there are two players)
	if(game->gameMode == ONE_PLAYER){
		switch(game->difficulty){
		case 1:
			str = "amateur";
			break;
		case 2:
			str = "easy";
			break;
		case 3:
			str = "moderate";
			break;
		case 4:
			str = "hard";
			break;
		case 5:
			str = "expert";
			break;
		}
		fprintf(saveFile,"DIFFICULTY: %s\n",str);
		if(game -> userColor == WHITE_PLAYER)
				str = "white";
			else
				str = "black";
		fprintf(saveFile,"USER_COLOR: %s\n",str);
	}
	// saving the board
	for (row = 8 ; row >= 1 ; row--) {
		fprintf(saveFile,"%d| ", row);
		for (col = 0 ; col < 8 ; col++) {
			fprintf(saveFile,"%c ", game-> gameBoard[ROWS-row][col]);
		}
		fprintf(saveFile,"|\n");
	}
	fprintf(saveFile,"-----------------\n");
	fprintf(saveFile,"   A B C D E F G H\n");

	fclose(saveFile);
	return true;
	//printf("Game saved to: %s\n",location);????!!?!?!??!?!?!?!?!?! what about GUI ?!?
}

bool loadGame(Game* game, char* location){
	int tmp1, tmp2 = 0, tmp3 = 0;
	char player[5], str[8], piece = EMPTY_CHAR;
	FILE* loadFile = fopen(location,"r");
	if(loadFile == NULL){
		printf("Error: File doesn't exist or cannot be opened\n"); 
		return false;
	}
	fscanf(loadFile,"%s\n",player);
	fscanf(loadFile,"SETTINGS:\n");
	fscanf(loadFile,"GAME_MODE: %d-player\n",&tmp1);
	if(tmp1 == ONE_PLAYER){
		fscanf(loadFile,"DIFFICULTY: %s\n",str);
		if(strcmp(str,"amateur") == 0)
			tmp2 = 1;
		else if(strcmp(str,"easy") == 0)
			tmp2 = 2;
		else if(strcmp(str,"moderate") == 0)
			tmp2 = 3;
		else if(strcmp(str,"hard") == 0)
			tmp2 = 4;
		else if(strcmp(str,"expert") == 0)
			tmp2 = 5;
		fscanf(loadFile,"USER_COLOR: %s\n",str);
		if(strcmp(str,"white") == 0)
			tmp3 = 1;
		else
			tmp3 = 0;
	}
	game -> currentPlayer = strcmp(player,"white") ? BLACK_PLAYER : WHITE_PLAYER;
	game -> gameMode = tmp1;
	// only meaningful if the mode is one player
	game -> difficulty = tmp2;
	game -> userColor = tmp3;
	for(tmp1 = 8; tmp1 >= 1; tmp1--){
		fscanf(loadFile,"%d| ",&tmp3);
		for (tmp2 = 0 ; tmp2 < 8 ; tmp2++) {
			fscanf(loadFile,"%c ", &piece);
			game-> gameBoard[ROWS-tmp1][tmp2] = piece;
			if(piece == 'k'){
				game->kings[WHITE_PLAYER][0] = ROWS-tmp1;
				game->kings[WHITE_PLAYER][1] = tmp2;
			}
			else if(piece == 'K'){
				game->kings[BLACK_PLAYER][0] = ROWS-tmp1;
				game->kings[BLACK_PLAYER][1] = tmp2;
			}
		}
		fscanf(loadFile,"|\n");
	}
	game -> state = checkState(game);
	fclose(loadFile);
	return true;
}

void resetMatrix(int destinations[MAX_MOVES_PER_PIECE][3]) {
	int index = 0;
	for (index = 0 ; index < MAX_MOVES_PER_PIECE ; index++) {
		destinations[index][0] = -1;
		destinations[index][1] = -1;
		destinations[index][2] = -1;
	}
}


