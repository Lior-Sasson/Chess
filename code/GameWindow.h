#ifndef GAMEWINDOW_H_
#define GAMEWINDOW_H_

#include "MainWindow.h"

// pieces source: https://www.behance.net/gallery/10018309/Chess-Artwork-Pieces-and-Board-Art-Assets

#define CHESS_BOARD "chess_board.bmp"
#define CHESS_GAME_HEADLINE "chess_game_headline.bmp"
#define GAME_BACKGROUND "game_background.bmp"
#define MAIN_BUTTON_PATH "main_menu_button.bmp"
#define RESTART_BUTTON_PATH "restart_game_button.bmp"
#define SAVE_BUTTON_PATH "save_game_button.bmp"
#define GAME_LOAD_BUTTON_PATH "game_load_game_button.bmp"
#define GAME_QUIT_BUTTON_PATH "game_quit_button.bmp"
#define UNDO_BUTTON_PATH "undo_button.bmp"
#define EMPTY_PATH "emptyTile.bmp"
#define BLACK_KING_PATH "kingB.bmp"
#define WHITE_KING_PATH "kingW.bmp"
#define BLACK_QUEEN_PATH "queenB.bmp"
#define WHITE_QUEEN_PATH "queenW.bmp"
#define BLACK_BISHOP_PATH "bishopB.bmp"
#define WHITE_BISHOP_PATH "bishopW.bmp"
#define BLACK_ROOK_PATH "rookB.bmp"
#define WHITE_ROOK_PATH "rookW.bmp"
#define BLACK_KNIGHT_PATH "knightB.bmp"
#define WHITE_KNIGHT_PATH "knightW.bmp"
#define BLACK_PAWN_PATH "pawnB.bmp"
#define WHITE_PAWN_PATH "pawnW.bmp"
#define GREEN_PATH "greenTile.bmp"
#define RED_PATH "redTile.bmp"
#define YELLOW_PATH "yellowTile.bmp"
#define ORANGE_PATH "orangeTile.bmp"
#define BLACK_WINS "checkmate_black.bmp"
#define WHITE_WINS "checkmate_white.bmp"

#define BOARD_EDGE 600
// square: 2.1x , border = 0.64x
#define SQUARE_EDGE (2.1*(600/17.8))
#define border (0.64*(600/17.8))


#define NUM_OF_TILES 64
// buttons: 3 main, 4 restart, 5 save, 6 load , 7 quit, 8 undo
// 12 piece pics, 1 empty tile, 1 background, 1 board game, 1 headline, 6 buttons, 4 color tiles
#define NUM_OF_GAME_TEXTURES 28
// 1 background, 1 board, 1 headline, 6 buttons
#define NUM_OF_UTILITY 10
/*
 * the squares colors in getMove:
 * green: a standard square
 * red: a threatened square
 * yellow: a capture square
 * orange: a square that is both threatened and a capture square.
 */


/*
 * The main gameWindow function: operates this window, sets all values and allows playing the game.
 * @param game - the given game. (gicen from settings or load)
 * @param gameWindow - the current window.
 * @param rend - the renderer for the current window.
 * @param textures - the window's needed textures.
 * @ret - the next window to go to, or error if an error occured. 
 */
ChessWindow runGameWindow(Game *game, SDL_Window* gameWindow, SDL_Renderer* rend, SDL_Texture* textures[NUM_OF_GAME_TEXTURES]);

bool allowedToMove(Game *game, int *src);

/*
 * This functions handles all events in this window. Allows going to other windows and playing the game.
 * @param game - the given game. (gicen from settings or load)
 * @param gameWindow - the current window.
 * @param rend - the renderer for the current window.
 * @param textures - the window's needed textures.
 * @param tiles - the rectangles that represent the board's checkers.
 * @param utilityRects - the rest of the window's rectangles.
 * @ret - the next window to go to, or error if an error occured. 
 */
ChessWindow gameEventLoop(Game *game, SDL_Window* gameWindow, SDL_Renderer* rend, SDL_Texture** textures, SDL_Rect tiles[ROWS][COLUMNS], SDL_Rect *utilityRects);
/*
 * This function sets the given rect array with the appropriate values.
 * @param utilityRects - some of the window's rectangles: background, board, headline, buttons.
 */
void setUtilityRects(SDL_Rect utilityRects[NUM_OF_UTILITY]);

/*
 * This function copies to the window's renderer the possible moves, in color, for some piece (that was pressed).
 * @param rend - the window's renderer.
 * @param dests - an array of all posibble moves for the current piece that was pressed, and their status.
 * @param textures - the textures of the window.
 * @param tiles - rectangles that are positioned in the board's tile.
 */
void showGetMovesColors(SDL_Renderer* rend, int dests[28][3], SDL_Texture** textures, SDL_Rect tiles[ROWS][COLUMNS]);

/*
 * This function performs a computer's move.
 * @param game - the current game
 * @ret - true if no allocation error occured, false otherwise.
 */
bool playComputersTurnGUI(Game *game);


/*
 * This function recieves the current game state and pops a message if necessary.
 * @param state - the current game state.
 * @param stillCheck - indicates if a message just popped and it's still the same state as was.
 * @return 0 if there are possible moves to perform, 1 otherwise (besides undo).
 */
int announceState(STATE state, int *stillCheck);

/*
 * This function asks the user if they want to save the current game before exiting the window or restarting.
 * @param gameWindow - the current window.
 * @ret 0 for "Yes", 1 for "No" and 2 for "Cancel".
 */
int askToSave(SDL_Window* gameWindow);

/*
 * This function sets the given textures and the appropriate images.
 * @param gameWindow - the current window.
 * @param rend - the renderer for the current window.
 * @param textures - the window's needed textures.
 * @ret - true if the textures were set sucessfully, false otherwise. 
 */
bool setGameTextures(SDL_Window* gameWindow, SDL_Renderer* rend, SDL_Texture** textures);

/*
 * This function sets the renderer according to the game state.
 * @param game - the given game. (gicen from settings or load)
 * @param gameWindow - the current window.
 * @param rend - the renderer for the current window.
 * @param textures - the window's needed textures.
 * @param tiles - the rectangles that represent the board's checkers.
 * @param utilityRects - the rest of the window's rectangles.
 * @param dests - the moves for get moves
 * @param pressedRight - indicates if needs to show moves
 */
void setRend(SDL_Renderer* rend, Game *game, SDL_Texture** textures, SDL_Rect utilityRects[NUM_OF_UTILITY], SDL_Rect tiles[ROWS][COLUMNS], int dests[28][3], int pressedRight);

/*
 * This function updates the rend regarding game board.
 * @param game - the given game. (gicen from settings or load)
 * @param rend - the renderer for the current window.
 * @param textures - the window's needed textures.
 * @param tiles - the rectangles that represent the board's checkers.
 */
void setBoardRend(SDL_Renderer* rend, Game *game, SDL_Texture** textures, SDL_Rect tiles[ROWS][COLUMNS]);

/*
 * This function translate a piece's symbol to it's index in the window's texture array.
 * @param symbol - the piece's symbol to translate.
 * @ret - the appropriate index or -1 if nothing matches.
 */
int symbolToTexIndex(char symbol);

/*
 * This Function set the positions and sizes of the rectangles that present the tiles of the game board.
 * @param tiles - the rectangles that represent the board's checkers.
 * @param boardRect - the ractangle that present the board.
 * @param squareEdge - the edge of a checker.
 * @param bord - the width of the border of the board.
 */
void setGameTiles(SDL_Rect tiles[ROWS][COLUMNS], SDL_Rect boardRect, int squareEdge, int bord);

/*
 * This function recieves a position that is assumed to be on the board and set pos to be
 * the row and column of the position.
 * @param x - the x value of the position.
 * @param y - the y value of the position.
 * @param boardRect - the ractangle that present the board.
 * @param pos - will contain the row and column of the position.
 */
void indexesFromPos(float x, float y, SDL_Rect boardRect, int pos[2]);

/*
 * This function sets the textures that are required for this module.
 * @param gameWindow - the current window.
 * @param rend- the window's renderer.
 * @param - the window's textures.
 * @ret - true if no error occured, false otherwise.
 */
bool setGameTextures(SDL_Window* gameWindow, SDL_Renderer* rend, SDL_Texture** textures);

/*
 * This function sets the textures that are required for the board.
 * @param window - the current window.
 * @param rend- the window's renderer.
 * @param - the textures to set.
 * @ret - true if no error occured, false otherwise.
 */
bool setBoardTex(SDL_Window* window, SDL_Renderer* rend, SDL_Texture** textures);

#endif /*GAMEWINDOW_H_*/
