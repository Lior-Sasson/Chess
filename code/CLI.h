#ifndef CLI_H_
#define CLI_H_
#include "Parser.h"
#include "Minimax.h"
#include "Game.h"

/**
 * Performs a game, asks the user/s for commands and plays.
 * @return
 * false if an allocation error occured, true otherwise.
 */
bool playGame(); 

/**
 * Plays a single round, i.e. a user's turn and a computer's turn or 2 users' turns.
 * @param game - the current game.
 * @return 
 * The command type the last user entered, or the first's if it's RESET | QUIT. it handles all other commands.
 */
COMMAND playRound(Game *game, Command *command); 

/**
 * Plays the computer's turn with IA implemented by the minimax algorithem.
 * @param - the current game.
 */
bool playComputersTurn(Game *game); // X

/**
 * Plays the user's turn, asks for commands and operates them. Handles all commands but
 * RESET & QUIT- returns their type and "playGame" handles them.
 * @param game - the current game.
 * @param command - an allocated command with irrelevant info. sets it and uses it. 
 * @return - 
 * The Command's type. Only relevant to "playGame" if it's RESET or QUIT.
 */
COMMAND playPlayersTurn(Game *game, Command *command); 

/**
 * Set setting to default values.
 * @param game - the current game.
 */
void setDefaultGameSettings(Game *game); 

/**
 * Set setting to user's choice. keep waiting for commands until recieving START.
 * Treats RESET, UNDO, SAVE, GET_MOVES, MOVE as INVALID_COMMAND. 
 * @param game - the current game.
 * @return 
 * false if an allocation error occured, true otherwise.
 */
bool setGameSettings(Game *game, bool *allocErr); 

/**
 * A help function for setGameSettings: checks the GAME_MODE command's validity and
 * updates the settings accoringly.
 * @param game - the current game.
 * @param command - the given command.
 */
void caseGameMode(Game *game, Command *command); 

/**
 * A help function for setGameSettings: checks the DIFFICULTY command's validity and
 * updates the settings accoringly.
 * @param game - the current game.
 * @param command - the given command.
 */
void caseDifficulty(Game *game, Command *command); 

/**
 * Translate from integer level of diffculty to literal description and
 * prints it.
 * @param difficulty - integer level of diffculty.
 */
void printDifficulty (int difficulty);

/**
 * A help function for setGameSettings: checks the USER_COLOR command's validity and
 * updates the settings accoringly.
 * @param game - the current game.
 * @param command - the given command.
 */
void caseUserColor(Game *game, Command *command); 

/**
 * Checks if the move is allowed, if so performs it and returns true,
 * otherwise prints an appropriate massage and returns false.
 * @param game - the current game.
 * @param command - the given command.
 * @return - 
 * true if the move was exacuted, false otherwise.
 */
bool caseMove(Game *game, Command *command);

/**
 * Checks if the position is legal, and if not it prints the possible
 * moves for the piece in the position.
 * @param game - the current game.
 * @param command - the given command. 
 */
void caseGetMoves(Game *game, Command *command);

/**
 * Checks for the UNDO validity and if so, performs it.
 * @param game - the current game.
 */
void caseUndo(Game *game);

/**
 * Recieves a string command from the user and set the given command
 * accordingly.
 * @param command - an allocated command to set.
 * @return - 
 * false if an allocation error occured, true otherwise. 
 */
bool setCommand (Command *command); 

/**
 * Recieves a string of unkown actual size (the maximal size is MAX_LINE_LENGTH)
 * and reallocates it with the actualSize ('\0' as the last character).
 * @param str - the given string.
 * @return - 
 * false if an allocation error occured, true otherwise. 
 */
bool shortenStr (char *str); 

/**
 * Given the current game, it Prints the settings as instructed.
 * @param game - the current game.
 */
void printSettings(Game *game); 

/**
 * Given the current game, it prints its current board game.
 * @param game - the current game.
 */
void printBoard(Game *game); 

#endif /* CLI_H_ */
