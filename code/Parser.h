#ifndef PARSER_H_
#define PARSER_H_

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define INT_MAX 2147483647
#define INT_MIN -2147483648
#define MAX_LINE_LENGTH 2000 // specify the maximum line length

/*
 * Enum COMMAND defines all the possible commands the user may write.
 * For our convinience, they are numbered
 */
typedef enum {
	INVALID_LINE,   // (1)
	START,      	// (2)
	GAME_MODE,		// (3)
	DIFFICULTY,		// (4)
	USER_COLOR,		// (5)
	LOAD,			// (6)
	DEFAULT,		// (7)
	PRINT_SETTINGS, // (8)
	QUIT,			// (9)
	MOVE,			// (10)
	GET_MOVES,		// (11)
	SAVE,			// (12)
	UNDO,			// (13)
	RESET,			// (14)
} COMMAND;

/**
 * Command is a struct the holds the relevant information of a given command.
 */
typedef struct chess_command {
	COMMAND cmd;	   		       // the fitting command
	bool validArgs;     		   // is set to true if the line contains valid arguments
	int intArg;		    		   // argument for commands (3), (4), (5), and also (6) and (12)- actual length of path
	int srcPos[2];				   // argument for (11), and for (10)- the move's source position
	int destPos[2];				   // second argument of (10)- the move's destination position
	char pathArg[MAX_LINE_LENGTH]; // argument for (6), (12)- the file's full or relative path
} Command;

/**
 *	Checks if a specified string represents a valid integer.
 *	@param str - the specified string.
 * 	@return
 * 	true if the string represents a valid integer, and false otherwise.
 */
bool isInt(const char* str);

/**
 *	Checks if a specified string repesents one of the valid commands (COMMAND).
 *	@param str - the specified string.
 *	@return
 *	A command type:
 *		if the string repersent a valid command then it will return the fitting command.
 *		else, it will return the command SP_INVALID_LINE.
 */
COMMAND stringToType(const char* str);

/**
 *	Parses a specified command line. If the line has valid arguments, validArgs is
 *	set to true and the arguments are saved in command. If the line is known by structure
 *	but the parameters aren't valid (not in range), validArgs is set to false while cmd is still
 *	fitting. If the line isn't known, cmd is set to INVALID_LINE. specification for valid arguments:
 *
 *	(3) game_mode - has an integer argument: 1 or 2
 *	(4) difficulty - has an integer argument between 1 and 5
 *	(5) user_color - has an integer argument: 0 or 1
 *	(6), (12) load, save - has an integer argument: the length of the path, and pathArg- the path.
 *	(10) move - has 2 arguments- 2 positions on the board. the letter is between 'A' and 'H', the number between 1-8
 *	(11) get_moves - has one argument - a position on the board.
 *
 *	@param str - the specified string.
 *	@param *command - allocated and empty Command to fill
 *	@return
 *	true if the Parsing executed without an allocation error, false otherwise
 */
bool parseLine(Command *command, const char* str);

/**
 * Handles cases (3), (4), (5) and changes command fittingly.
 * @param command - allocated Command
 * @param secondArg - the second word in the command line- should hold the parameter
 */
void parseLineIntArg (Command *command, const char* secondArg);

/**
 * Checks if the integer argument is in range according to the current command in
 * cases (3), (4), (5).
 * @param cmd - the type of command
 * @param intArg - the integer argument
 */
bool validIntArg (COMMAND cmd, int intArg);

/**
 * Handles case (10) and changes command fittingly.
 * @param command - allocated Command
 * @param words - an array of the first 4 words in the command line (we require all
 * four and ignore the rest if exist)
 * @param allocError - set to false, specifies if an allocation error occurs in the
 * function.
 */
void parseLineMove(Command* command, const char* words[], bool *allocError);

/**
 * Handles case (11) and changes command fittingly.
 * @param command - allocated Command
 * @param pos - the second word in the command line which should hold a posotion
 * on the board.
 * @param allocError - set to false, specifies if an allocation error occurs in the
 * function.
 */
void parseLineGetMoves(Command* command, const char* pos, bool *allocError);

/**
 * Checks if the given position which is represented by two numbers (the letter
 * is altered to a fitting number, 'A' to 0 and so on until 7).
 * @param num - number of row in the board
 * @param letter - number of column in the board
 * @return
 * true if num and letter are between 0 and 7, false otherwise
 */
bool isValidPosition (int num, int letter);

/**
 * Checks if the given string represent a position and changes a given position
 * (an array of 2 integers) accordingly.
 * @param posStr - the given string
 * @param pos - the position to complete
 * @param allocError - set to false, specifies if an allocation error occurs in the
 * function.
 * @return
 * true if posStr's structure is: <X,Y> wheras X is an integer, and Y is a letter/
 */
bool isPosition(char *posStr, int *pos, bool *allocError);

/**
 * Translate a Command type to a string
 * @param cmd - the given command type
 * @return
 * a string that represents the given command type
 */
char* typeToString(COMMAND cmd);

#endif /* PARSER_H_ */
