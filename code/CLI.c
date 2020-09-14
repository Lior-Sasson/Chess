#include "CLI.h"

bool playGame() { // returns true if there are no allocation errors
	bool quit = false, allocErr = false;
	Game *game = createGame(1,2,1);
	Command *command;
	COMMAND cmd;
	if (game == NULL) {
		return false;
	}
	command = (Command *) malloc(sizeof(Command));
	if (command == NULL) { // allocation error
		printf("ERROR: playGame has failed.");
		free(game);
		return false;
	}
	printf("Chess\n-------\n");
	if (setGameSettings(game, &allocErr) == false) { // allocation problem
		free(command);
		destroyGame(game);
		if (!allocErr)
			printf("Exiting...\n");
		return false;
	}
	if (game == NULL) {// Quit has been typed, game was destroyed, a massage was printed
		printf("Exiting...\n");
		free(command);
		return false;	
	}
	printf("Starting game...\n");
	if (game-> gameMode == 1 && game-> userColor == 0){ // in case the computer starts, the board still needs to be printed at the beginning.
		printBoard(game);
	}
	
	while (checkState(game) == GAME && quit == false) { // check the output for no winner
		cmd = playRound(game, command);
		if (cmd == INVALID_LINE) { // minimax failed
			free(command);
			destroyGame(game);
			return false;
			
		}		
		if (cmd == QUIT) {
			quit = true;
		}
		else if (cmd == RESET) {
			if (setGameSettings(game, &allocErr) == false) { // quit or error				
				if (allocErr) { // error
					free(command);
					destroyGame(game);
					return false;
				}
				quit = true;
			}
			printf("Restarting...\n");
			if (game-> gameMode == 1 && game-> userColor == 0) // in case the computer starts, the board still needs to be printed at the beginning.
				printBoard(game); 
		}
	}	
	destroyGame(game);
	free(command);
	printf("Exiting...\n");
	return true;
}

COMMAND playRound(Game *game, Command *command) {
	COMMAND cmd;
	if (game-> gameMode == ONE_PLAYER) {
		if (game->userColor == BLACK_PLAYER) {
			if (playComputersTurn(game) == false) { // computer's move
				return INVALID_LINE; // allocation error
			}
			cmd = playPlayersTurn(game, command); // user's move
		}
		else { 
			cmd = playPlayersTurn(game, command);
			if (cmd == RESET || cmd == QUIT || cmd == INVALID_LINE)
				return cmd;
			if (playComputersTurn(game) == false) {
				return INVALID_LINE; // allocation error
			}
		}
	}
	else {
		cmd = playPlayersTurn(game, command);
		if (cmd == RESET || cmd == QUIT)
			return cmd;
		cmd = playPlayersTurn(game, command);
	}
	return cmd;
}

COMMAND playPlayersTurn(Game *game, Command *command) { // returns NULL if there ARE allocation errors
	bool keepLooping = true;
	COMMAND cmd;
	printBoard(game);
	while (keepLooping) {
		printf("Enter your move ");
		if (game->currentPlayer == WHITE_PLAYER) 
			printf("(white player):\n");
		else
			printf("(black player):\n");
		if (setCommand(command) == false) { // allocation error
			free(command);
			return INVALID_LINE;
		}
		switch(command-> cmd) {
		case MOVE:
			if (caseMove(game, command))
				keepLooping = false;
			break;
		case GET_MOVES:
			caseGetMoves(game, command);
			break;
		case SAVE:
			if (saveGame(game, command->pathArg) == true)
				printf("Game saved to: %s\n", command->pathArg);
			else
				printf("File cannot be created or modified\n");
			break;
		case UNDO: // will always loop at least once more
			caseUndo(game);
			break;
		case RESET:
			return RESET;
		case QUIT:
			return QUIT;
		default:
			printf("ERROR: invalid command\n");
			break;
		}
	}
	cmd = command-> cmd;
	return cmd;
}

char* translatePlayer(int player) {
	if (player == 0)
		return "black";
	return "white";
}

bool playComputersTurn(Game *game) {
	int src[2], dest[2];
	if (findNextMove(game, src, dest) == false)
		return false;
	doMove(game,src,dest);
	game -> state = checkState(game);
	return true;
}

void setDefaultGameSettings(Game *game) {
	game-> gameMode = 1;
	game-> difficulty = 2;
	game-> userColor = 1;
}

bool setGameSettings(Game *game, bool *allocErr) { // returns false if a quit command was inserted or an error occured
	bool keepLooping = true;
	Command *command = (Command *) malloc(sizeof(Command));
	if (command == NULL) { // allocation error
		printf("ERROR: setGameSettings has failed."); // allocation problem
		return false;
	}
	setDefaultGameSettings(game);
	printf("Specify game settings or type 'start' to begin a game with the current settings:\n");
	while (keepLooping) {
		if (setCommand(command) == false) { 
			free(command);
			*allocErr = true;
			return false; // allocation error
		}
		switch(command-> cmd) {
		case START:
			keepLooping = false;
			break;
		case GAME_MODE:
			caseGameMode(game, command);
			break;
		case DIFFICULTY:
			caseDifficulty(game, command);
			break;
		case USER_COLOR:
			caseUserColor(game, command);
			break;
		case DEFAULT:
			setDefaultGameSettings(game);
			printf("All settings reset to default\n");
			break;
		case PRINT_SETTINGS:
			printSettings(game);
			break;
		case QUIT:
			free(command);
			return false;
		case LOAD:
			loadGame(game, command->pathArg);
			break;
		default:
			printf ("ERROR: invalid command\n");
			break;
		}
	}
	free(command);
	return true;
}

void caseGameMode(Game *game, Command *command) {
	if (command-> validArgs) {
		game-> gameMode = command-> intArg;
		printf("Game mode is set to %d-player\n", game-> gameMode);
	}
	else
		printf("Wrong game mode\n");
}

void caseDifficulty(Game *game, Command *command) {
	if (game-> gameMode != 1) {
		printf ("ERROR: invalid command\n");
		return;
	}
	if (command-> validArgs) {
		game-> difficulty = command-> intArg;
		printf("Difficulty level is set to ");
		printDifficulty(game-> difficulty);
	}
	else
		printf("Wrong game mode\n");
}

void printSettings(Game *game) {
	printf("SETTINGS:\nGAME_MODE: %d-player\n", game-> gameMode);
	if (game-> gameMode == 1) {
		printf("DIFFICULTY: ");
		printDifficulty(game-> difficulty);
		printf("USER_COLOR: ");
		if (game-> userColor == 0)
			printf("black\n");
		else
			printf("white\n");
	}
}

void printDifficulty (int difficulty) {
	switch (difficulty) {
	case 1:	printf("amateur\n");
			break;
	case 2: printf("easy\n");
			break;
	case 3: printf("moderate\n");
			break;
	case 4: printf("hard\n");
			break;
	case 5: printf("expert\n");
	}
}

void caseUserColor(Game *game, Command *command) {
	if (game-> gameMode != 1) {
		printf ("ERROR: invalid command\n");
		return;
	}
	if (command-> validArgs) {
		game-> userColor = command-> intArg;
		if (command-> intArg == 0)
			printf("User color is set to black\n");
		else
			printf("User color is set to white\n");
	}
	else
		printf("Wrong user color. The value should be 0 or 1\n");
}

bool caseMove(Game *game, Command *command) {
	GAME_MOVE_ERROR error = SUCCSESS;
	//printf("|%d,%d|",command->srcPos[0], command->srcPos[1]);
	error = playMove(game, command->srcPos, command->destPos);
	switch (error) {
	case SUCCSESS:
		return true;
	case INVALID_POSITION:
		printf("Invalid position on the board\n");
		break;
	case EMPTY_POSITION:
		printf("The specified position does not contain your piece\n");
		break;
	case ILLEGAL_MOVE:
		printf("Illegal move\n");
		break;
	case KING_STILL_EXPOSED:
		printf("Illegal move: king is still threatened\n");
		break;
	case KING_EXPOSED:
		printf("Ilegal move: king will be threatened\n");
		break;
	}
	return false;

}

void caseGetMoves(Game *game, Command *command) {
	if (command-> validArgs == false) {
		printf("Invalid position on the board\n");
	}
	else
		if (game-> gameBoard[command->srcPos[0]][command->srcPos[1]] == EMPTY) {
			printf("The specified position does not contain a player piece\n");
		}
		else
			getMoves(game, command->srcPos[0], command->srcPos[1], 0, NULL);	// create this function in Gameeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee
}

void caseUndo(Game *game) { 
	int src[2], dest[2];
	if (undoMove(game, src, dest) == false)  {
		printf("Empty history, no move to undo\n");	
		return;
	}	
	printf("Undo move for %s player: <%d,%c> -> <%d,%c>\n", translatePlayer(game->currentPlayer),src[0],'A'+src[1], dest[0], 'A'+dest[1]);
	
	if (undoMove(game, src, dest) == false) 
		printf("Empty history, no move to undo\n");
	else
		printf("Undo move for %s player: <%d,%c> -> <%d,%c>\n", translatePlayer(game->currentPlayer),src[0],'A'+src[1], dest[0], 'A'+dest[1]);
}


bool setCommand (Command *command) { // returns true if there are no allocation errors
	char cmdStr[MAX_LINE_LENGTH];
	if (fgets(cmdStr, MAX_LINE_LENGTH, stdin) == NULL) { // recieve a command line from the user, check if there's an error
		printf("ERROR: setCommand has failed.");
		return false;
	}
	if (parseLine(command, cmdStr) == false) {
		return false;
	}
	return true;
}

/* not used */
bool shortenStr (char *str) { // returns true if there are no allocation errors
	int i;
	if (str == NULL)
		return false;
	if (strlen(str)+1 == MAX_LINE_LENGTH)
		return true;
	for (i = 0 ; i < MAX_LINE_LENGTH ; i++) {
		if (str[i] == '\0') {
			str = (char *) realloc(str, i+1);
			if (str == NULL) {
				free(str);
				printf("ERROR: shortenStr has failed.");
				return false;
			}
		}
		break;
	}
	return true;
}

void printBoard(Game *game) {
	int row, col;
	for (row = 8 ; row >= 1 ; row--) {
		printf("%d| ", row);
		for (col = 0 ; col < 8 ; col++) {
			printf("%c ", game-> gameBoard[ROWS-row][col]);
		}
		printf("|\n");
	}
	printf("  -----------------\n");
	printf("   A B C D E F G H\n");
}
