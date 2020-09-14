#include "Parser.h"

bool isInt(const char* str){
	int index = 0;
	long long value = 0;
	if (str == NULL)
		return false;
	if (str[0] == '\0' || strcmp(str, "") == 0) // checks if str is empty or null character
		return false;
	if(str[0] == '-') { // checks if the string represent a negative number
		index++;
		if (str[1] == '\0')
			return false;
	}
	while(str[index] != '\0'){
		if (str[index] < '0' || str[index] > '9')
			return false;
		value = 10 * value + (str[index] - '0'); // calculates the numerical value of str
		if((str[0] == '-' && -value < INT_MIN)||(str[0] != '-' && value > INT_MAX)) // checks if the current value is an integer
			return false;
		index++;
	}
	return true;
}

COMMAND stringToType(const char* str) {
	if (strcmp(str,"start") == 0)
		return START;
	if (strcmp(str,"game_mode") == 0)
		return GAME_MODE;
	if (strcmp(str,"difficulty") == 0)
		return DIFFICULTY;
	if (strcmp(str,"user_color") == 0)
		return USER_COLOR;
	if (strcmp(str,"load") == 0)
		return LOAD;
	if (strcmp(str,"default") == 0)
		return DEFAULT;
	if (strcmp(str,"print_settings") == 0)
		return PRINT_SETTINGS;
	if (strcmp(str,"quit") == 0)
		return QUIT;
	if (strcmp(str,"move") == 0)
		return MOVE;
	if (strcmp(str,"get_moves") == 0)
		return GET_MOVES;
	if (strcmp(str,"save") == 0)
		return SAVE;
	if (strcmp(str,"undo") == 0)
		return UNDO;
	if (strcmp(str,"reset") == 0)
		return RESET;
	return INVALID_LINE;
}

bool parseLine(Command *command, const char* str) { // returns true if there is no allocation error
	COMMAND cmd;
	char *tmpStr = (char *) calloc (strlen (str) + 1, sizeof(char));
	const char *words[4];
	bool noAllocError = true;
	
	if(tmpStr == NULL){ // checks if an allocation error has occurred
		printf("ERROR: parseLine has failed");
		return false;
	}
	strcpy(tmpStr, str); // copy the const string to a non-const string for string manipulation
	command-> validArgs = true;
	words[0] = strtok(tmpStr, " \t\r\n"); // divide the string to seperate words, recieve the first
	if (words[0] == NULL) {
		command-> cmd = INVALID_LINE;
		free(tmpStr);
		return true;
	}
	command-> cmd = stringToType(words[0]); // recive the command (or INVALID_LINE)
	cmd = command-> cmd;
	words[1] = strtok(NULL, " \t\r\n");
	words[2] = strtok(NULL, " \t\r\n");
	words[3] = strtok(NULL, " \t\r\n"); // 4 relevant words in a command at the most. we ignore the rest

	// we need to handle only: (3),(4),(5),(6),(10),(11),(12)
	if (cmd == GAME_MODE || cmd == DIFFICULTY || cmd == USER_COLOR) // (3), (4), (5)
		parseLineIntArg(command, words[1]);
	if (cmd == MOVE) 												// (10)
		parseLineMove(command, words, &noAllocError);
	if (cmd == GET_MOVES)											// (11)
		parseLineGetMoves(command, words[1], &noAllocError);
	if (cmd == LOAD || cmd == SAVE) {								// (6),(12)			
		strcpy(command-> pathArg, words[1]);		
		command-> intArg = strlen(words[1]);
	}
	free(tmpStr); // free the allocated memory
	
	if (!noAllocError)
		printf("ERROR: parseLine has failed");
	return noAllocError;
}

void parseLineIntArg (Command *command, const char* secondArg) { // (3), (4), (5)
	int intArg;
	if (secondArg == NULL) {
		command-> cmd = INVALID_LINE;
		return;
	}
	if (isInt(secondArg)) {
		intArg = atoi(secondArg);
		if (validIntArg(command-> cmd, intArg)) {
			command-> validArgs = true;
			command-> intArg = intArg;
		}
		else
			command-> validArgs = false;
	}
	else
		command-> cmd = INVALID_LINE;
}

bool validIntArg (COMMAND cmd, int intArg) {
	if ((cmd == GAME_MODE) && (intArg == 1 || intArg == 2))
		return true;
	if ((cmd == DIFFICULTY) && (intArg >= 1 && intArg <= 5))
		return true;
	if ((cmd == USER_COLOR) && (intArg == 0 || intArg == 1))
		return true;
	return false;
}

void parseLineMove(Command* command, const char* words[], bool *noAllocError) { // (10)
	char *srcStr;
	char *destStr;
	if (words[1] == NULL || words[2] == NULL || words[3] == NULL || strcmp(words[2], "to") != 0) {
		command-> cmd = INVALID_LINE;
		return;
	}
	srcStr = (char *) calloc (strlen (words[1]) + 1, sizeof(char));
	if (srcStr == NULL) { // checking for allocation error
		*noAllocError = false;
		return;
	}
	destStr = (char *) calloc (strlen (words[3]) + 1, sizeof(char));
	if (destStr == NULL) { // checking for allocation error
		free(srcStr);
		*noAllocError = false;
		return;
	}
	strcpy(srcStr, words[1]);
	strcpy(destStr, words[3]);
	if (isPosition(srcStr, command-> srcPos, noAllocError) == false || isPosition(destStr, command->  destPos, noAllocError) == false) {
		command-> cmd = INVALID_LINE;	
		free(srcStr);
		free(destStr);
		return;
	}
	free(srcStr);
	free(destStr);
	if (!isValidPosition(command-> srcPos[0], command-> srcPos[1]) || !isValidPosition(command->  destPos[0], command->  destPos[1])) {
		command-> validArgs = false;
	}
}

void parseLineGetMoves(Command* command, const char* pos, bool *noAllocError) { // (11)
	char *tmp;
	if (pos == NULL) {
		command-> cmd = INVALID_LINE;
		return;
	}
	tmp = (char *) calloc (strlen (pos) + 1, sizeof(char));
	if (tmp == NULL) {
		*noAllocError = false;
		return;
	}
	strcpy(tmp, pos);
	if (isPosition(tmp, command-> srcPos, noAllocError) == false) {
		command-> cmd = INVALID_LINE;	
	}
	if (isValidPosition(command-> srcPos[0], command-> srcPos[1]) == false) {
		command-> validArgs = false;
	}
	free(tmp);
}

bool isPosition(char *posStr, int *pos, bool *noAllocError) { // checks if posStr represents a position
	int i = 0;
	char *num = (char *) calloc(strlen(posStr) + 1, sizeof(char)), c = '.';
	if (num == NULL) {
		*noAllocError = false;
		return false;
	}
	if (posStr[0] != '<') {
		free(num);
		return false;
	}
	while (posStr[i+1] != ',' && i < MAX_LINE_LENGTH - 4 && c != '\0') {
		c = posStr[i+1];
		num[i] = c;
		i++;
	}
	if ((num = (char *) realloc(num ,i+1)) == NULL) {
		*noAllocError = false;
		free(num);
		return false;
	}
	if (i >= MAX_LINE_LENGTH - 4 || c == '\0' || !isInt(num)) {
		free(num);
		return false;
	}
	if (posStr[i+2] < 'A' || posStr[i+2] > 'Z') {
		free(num);
		return false;
	}
	pos[0] = 8 - atoi(num); // ROWS = 8
	pos[1] = posStr[i+2] - 'A';
	free(num);
	return true;
}

bool isValidPosition (int num, int letter) { // checks if position in range
	if (num < 0 || num > 7 || letter < 0 || letter > 7)
		return false;
	return true;
}

char* typeToString(COMMAND cmd) {
	if (cmd == START)
		return "start";
	if (cmd == GAME_MODE)
		return "game_mode";
	if (cmd == DIFFICULTY)
		return "difficulty";
	if (cmd == USER_COLOR)
		return "user_color";
	if (cmd == LOAD)
		return "load";
	if (cmd == DEFAULT)
		return "default";
	if (cmd == PRINT_SETTINGS)
		return "print_settings";
	if (cmd == QUIT)
		return "quit";
	if (cmd == MOVE)
		return "move";
	if (cmd == GET_MOVES)
		return "get_moves";
	if (cmd == UNDO)
		return "undo";
	if (cmd == RESET)
		return "reset";
	if (cmd == INVALID_LINE)
		return "invalid";
	return ".";
}


