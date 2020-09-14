#include "GameWindow.h"

ChessWindow runGameWindow(Game *game, SDL_Window* gameWindow, SDL_Renderer* rend, SDL_Texture** textures) {
	SDL_Rect utilityRects[NUM_OF_UTILITY];
	SDL_Rect tiles[ROWS][COLUMNS];

	setUtilityRects(utilityRects);
	setGameTiles(tiles, utilityRects[1], SQUARE_EDGE, border);
	utilityRects[9].x = tiles[2][1].x - 0.5*SQUARE_EDGE;
	utilityRects[9].y = tiles[2][1].y;
	utilityRects[9].w = 7*SQUARE_EDGE;
	utilityRects[9].h = 2.5*SQUARE_EDGE;

	// ensure renderer supports transparency
	SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);
	//SDL_ShowWindow(gameWindow);
	setRend(rend, game, textures, utilityRects, tiles, NULL, 0);

	return gameEventLoop(game, gameWindow, rend, textures, tiles, utilityRects);

}

ChessWindow gameEventLoop(Game *game, SDL_Window* gameWindow, SDL_Renderer* rend, SDL_Texture** textures, SDL_Rect tiles[ROWS][COLUMNS], SDL_Rect *utilityRects) {
	SDL_Event e;
	int save = 0; // indicates if moves were made after save
	int gameOver = 0; // indicates if there was a winner/draw
	int movePiece = 0; // indicates if allowed to drag
	int firstMouseDown = 1;
	int xDif, yDif, src[2], dest[2];
	int stillCheck = 0, show = 0, pressedRight = 0;
	float x, y;
	int dests[28][3];

	game->state = checkState(game);
	setRend(rend, game, textures, utilityRects, tiles, dests, pressedRight);

	if (game->gameMode == 1 && game->userColor == 0) { // computer starts
		SDL_Delay(2000);
		if (playComputersTurnGUI(game)) {
			setRend(rend, game, textures, utilityRects, tiles, NULL, 0);
		}
		else
			return ERROR; // allocation error
		save = 1; // moves were played without saving
	}

	while (true) {
		SDL_WaitEvent(&e);
		gameOver = announceState(game->state, &stillCheck);
		if (game->state == BLACK_LOSE) {
			SDL_RenderCopy(rend, textures[27], NULL, &(utilityRects[9]));
			SDL_RenderPresent(rend);
		}
		else if (game->state == WHITE_LOSE) {
			SDL_RenderCopy(rend, textures[26], NULL, &(utilityRects[9]));
			SDL_RenderPresent(rend);
		}


		switch (e.type) {
			case SDL_WINDOWEVENT:
					if (e.window.event == SDL_WINDOWEVENT_CLOSE) {
						if (save == 0) { // no moves were made
							return QUIT_GAME;
						}
						// moves were made
						save = askToSave(gameWindow);
						if (!save || save == -1) {
							return QUIT_GAME;
						}
						if (save == 1) {
							return SAVE_WINDOW;
						}
						break;
					}
					break;
			case SDL_QUIT: // pressed X
				if (save == 0) { // no moves were made
					return QUIT_GAME;
				}
				// moves were made
				save = askToSave(gameWindow);
				if (!save || save == -1) {
					return QUIT_GAME;
				}
				if (save == 1) {
					return SAVE_WINDOW;
					}
				break;
			case SDL_KEYDOWN:
				if (e.key.keysym.sym == SDLK_ESCAPE) { // pressed esc
					if (save == 0) { // no moves were made
						return QUIT_GAME;
					}
					save = askToSave(gameWindow);
					if (!save || save == -1) {
						return QUIT_GAME;
					}
					if (save == 1) {
						return SAVE_WINDOW;
					}
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (e.button.button == SDL_BUTTON_LEFT && isInRect(e, utilityRects[1]) && (!gameOver) ) {
					if (firstMouseDown) { // fill source
						indexesFromPos(e.button.x, e.button.y, utilityRects[1], src);
						x = tiles[src[0]][src[1]].x;
						y = tiles[src[0]][src[1]].y;
						firstMouseDown = 0;
					}
					if (allowedToMove(game, src)) { // pressed on a piece that's allowed to move
						//SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "check massage", "says th!", NULL);
						xDif = e.button.x - tiles[src[0]][src[1]].x;
						yDif = e.button.y - tiles[src[0]][src[1]].y;
						movePiece = 1;
					}
					else {
						firstMouseDown = 1;
					}
				}
				break;

			case SDL_MOUSEBUTTONUP:
				if (pressedRight) {
					pressedRight = 0;
					setRend(rend, game, textures, utilityRects, tiles, NULL, 0);
				}
				firstMouseDown = 1;
				if (movePiece) {
					movePiece = 0; // no more moving
					if (isInRect(e, utilityRects[1])) {
						indexesFromPos(e.button.x, e.button.y, utilityRects[1], dest);
						if (checkMove(game, src, dest) == SUCCSESS) { // complete move
							doMove(game,src,dest); // play move in the game struct (already know it's legal)
							game -> state = checkState(game); // update state
							gameOver = announceState(game -> state, &stillCheck);
							setRend(rend, game, textures, utilityRects, tiles, NULL, 0);
							tiles[src[0]][src[1]].x = x; // get the source tile back in place :)
							tiles[src[0]][src[1]].y = y;

							if (game->gameMode == 1 && !gameOver) {
								if (playComputersTurnGUI(game)) {
									setRend(rend, game, textures, utilityRects, tiles, NULL, 0); //**********************************
								}
								else
									return ERROR; // allocation error
							}
							save = 1; // moves were played without saving
						}
					}
					tiles[src[0]][src[1]].x = x; // get the source tile back in place :)
					tiles[src[0]][src[1]].y = y;
					setRend(rend, game, textures, utilityRects, tiles, NULL, 0);
					break;

				}

				if ((e.button.button == SDL_BUTTON_RIGHT) && (isInRect(e, utilityRects[1])) && (!gameOver)) { // getMoves

					indexesFromPos(e.button.x, e.button.y, utilityRects[1], src);
					resetMatrix(dests);
					if (getMoves(game, src[0], src[1], 2, dests)) { // there are possible moves
						pressedRight = 1;
						setRend(rend, game, textures, utilityRects, tiles, dests, pressedRight);
					}
					break;
				}

				if (isInRect(e, utilityRects[7])) { // pressed quit button
					if (save == 0) { // no moves were made
						return QUIT_GAME;
					}
					save = askToSave(gameWindow);
					if (!save || save == -1) {
						return QUIT_GAME;
					}
					if (save == 1) {
						return SAVE_WINDOW;
					}
					break;
				}
				if (isInRect(e, utilityRects[3])) { // pressed main button
					if (save == 0) {
						return MAIN_WINDOW;
					}
					save = askToSave(gameWindow);
					if (save == 1) {
						return SAVE_WINDOW;
					}
					if (save == -1) {
						return QUIT_GAME;
					}
					if (!save)
						return MAIN_WINDOW;
					break;
				}
				if (isInRect(e, utilityRects[6])) {// pressed load button
					if (save == 0) {
						return LOAD_WINDOW;
					}
					save = askToSave(gameWindow);
					if (!save)
						return LOAD_WINDOW;
					if (save == -1) {
						return QUIT_GAME;
					}
					if (save == 1) {
						return SAVE_WINDOW;
					}
					break;
				}
				if (isInRect(e, utilityRects[4])) { // pressed restart button

					if (save == 0) {
						gameOver = 0;
						game -> state = GAME;
						initializeBoard(game);
						game->currentPlayer = 1;
						setRend(rend, game, textures, utilityRects, tiles, NULL, 0);
					}
					else {
						save = askToSave(gameWindow);
						if (!save) {
							gameOver = 0;
							game -> state = GAME;
							initializeBoard(game);
							game->currentPlayer = 1;
							setRend(rend, game, textures, utilityRects, tiles, NULL, 0);
						}
						if (save == 1) {
							return SAVE_WINDOW;
						}
					}
					//gameOver = 0;
					break;
				}
				else if (isInRect(e, utilityRects[5]))  // pressed save button
					return SAVE_WINDOW;

				if (isInRect(e, utilityRects[8])) { // pressed undo button
					if (undoMove(game, src, dest)) { // allowed to undo
						gameOver = 0;
						setRend(rend, game, textures, utilityRects, tiles, NULL, 0);
					}
					else {
						SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "undo overflow", "You cannot undo move now.", NULL);
					}
					if (undoMove(game, src, dest))  // allowed to undo
						setRend(rend, game, textures, utilityRects, tiles, NULL, 0);
				}
				break;
			case SDL_MOUSEMOTION:
				if (movePiece) {
					tiles[src[0]][src[1]].x = e.motion.x - xDif;
					tiles[src[0]][src[1]].y = e.motion.y - yDif;
				}
				//setRend(rend, game, textures, utilityRects, tiles, dests, pressedRight); // DELETE AFTER
				if (show == 6) {
					if (!gameOver)
						setRend(rend, game, textures, utilityRects, tiles, dests, pressedRight);
					show = 0;
				}
				else
					show++;
				break;
			default:
				break;
		}
	}
	return QUIT_GAME;
}

bool allowedToMove(Game *game, int *src) {
	if (game->gameMode == 2 &&  checkPlayer(game->gameBoard[src[0]][src[1]]) == game->currentPlayer)
		return true;
	if (game->gameMode == 1 && checkPlayer(game->gameBoard[src[0]][src[1]]) == game->currentPlayer && game->currentPlayer == game->userColor)
		return true;
	return false;
}

void showGetMovesColors(SDL_Renderer* rend, int dests[28][3], SDL_Texture** textures, SDL_Rect tiles[ROWS][COLUMNS]) {
	int i;
	for (i = 0 ; i < 28 ; i++) {
		if (dests[i][0] == -1)
			break;
		if(dests[i][2] == 1) { // threatened
			SDL_RenderCopy(rend, textures[22], NULL, &(tiles[dests[i][0]][dests[i][1]]));
			//SDL_RenderPresent(rend);
		}
		else if(dests[i][2] == 2) { // captures
			SDL_RenderCopy(rend, textures[23], NULL, &(tiles[dests[i][0]][dests[i][1]]));
			//SDL_RenderPresent(rend);
		}
		else if(dests[i][2] == 3) { // both
			SDL_RenderCopy(rend, textures[24], NULL, &(tiles[dests[i][0]][dests[i][1]]));
			//SDL_RenderPresent(rend);
		}
		else { // niether
			SDL_RenderCopy(rend, textures[25], NULL, &(tiles[dests[i][0]][dests[i][1]]));
			//SDL_RenderPresent(rend);
		}
	}
}

bool playComputersTurnGUI(Game *game) {
	int src[2], dest[2];
	if (findNextMove(game, src, dest) == false)
		return false;
	doMove(game,src,dest);
	game -> state = checkState(game);
	return true;
}

int announceState(STATE state, int *stillCheck) {
	if (state == GAME) {
		*stillCheck = 0;
		return 0;
	}
	if (state == BLACK_CHECKED) {
		if (!(*stillCheck))
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "check massage", "Watch out! The black king is threatened!", NULL);
		*stillCheck = 1;
		return 0;
	}
	if (state == WHITE_CHECKED) {
		if (!(*stillCheck))
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "check massage", "Watch out! The white king is threatened!", NULL);
		*stillCheck = 1;
		return 0;
	}
	if (state == DRAW) {
		if (!(*stillCheck))
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "game over", "It's a draw!", NULL);
		*stillCheck = 1;
		return 1;
	}
	if (state == WHITE_LOSE) {
		if (!(*stillCheck))
			//SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "game over", "Checkmate! The black player wins!", NULL);
		*stillCheck = 1;
		return 1;
	}
	if (state == BLACK_LOSE) {
		if (!(*stillCheck))
			//SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "game over", "Checkmate! The white player wins!", NULL);
		*stillCheck = 1;
		return 1;
	}
	return -1;
}

int askToSave(SDL_Window* gameWindow) {
	const SDL_MessageBoxButtonData buttons[] = {
		{ /* .flags, .buttonid, .text */        0, 0, "No" },
		{ SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "Yes" },
		{ SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 2, "Cancel" },
	};
	const SDL_MessageBoxData messageboxdata = {
		SDL_MESSAGEBOX_WARNING, /* .flags */
		gameWindow, /* .window */
		"Exit game", /* .title */
		"Would you like to save your game?", /* .message */
		SDL_arraysize(buttons), /* .numbuttons */
		buttons /* .buttons */
	};
	int buttonid;
	if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
		SDL_Log("error displaying message box");
		return -1;
	}
	return buttonid;
}

void setRend(SDL_Renderer* rend, Game *game, SDL_Texture** textures, SDL_Rect utilityRects[NUM_OF_UTILITY], SDL_Rect tiles[ROWS][COLUMNS], int dests[28][3], int pressedRight) {
	int i;
	// clear window
	SDL_SetRenderDrawColor(rend, 240, 207, 96, 0);
	SDL_RenderClear(rend);
	for (i = 0 ; i  < NUM_OF_UTILITY-1 ; i++) {
		SDL_RenderCopy(rend, textures[i+13], NULL, &(utilityRects[i]));
	}
	 setBoardRend(rend, game, textures, tiles);
	if (pressedRight)
		showGetMovesColors(rend, dests, textures, tiles);
	// present changes to user
	SDL_RenderPresent(rend);
}

void setBoardRend(SDL_Renderer* rend, Game *game, SDL_Texture** textures, SDL_Rect tiles[ROWS][COLUMNS]) {
	int col, row, texIndex;
	for (col = 0 ; col < COLUMNS ; col++) {
		for (row = 0 ; row < ROWS ; row++) {
			texIndex = symbolToTexIndex(game->gameBoard[row][col]);
			SDL_RenderCopy(rend, textures[texIndex], NULL, &(tiles[row][col]));
		}
	}
}

void setUtilityRects(SDL_Rect utilityRects[NUM_OF_UTILITY]) {
	float buttonWidth = WIDTH/3.5, buttonHight = HIGHT/11.0;
	int i;
	// order: background, board, headline, main, restart, save, load, quit, undo.
	utilityRects[0].x = 0;
	utilityRects[0].y = 0;
	utilityRects[0].w = WIDTH;
	utilityRects[0].h = HIGHT;

	utilityRects[1].x = WIDTH*0.375;
	utilityRects[1].y = (HIGHT - BOARD_EDGE)/2.0 - 35;
	utilityRects[1].w = BOARD_EDGE;
	utilityRects[1].h = BOARD_EDGE;

	utilityRects[2].x = WIDTH/125.0;
	utilityRects[2].y = utilityRects[1].y-30;
	utilityRects[2].w = WIDTH/3.0;
	utilityRects[2].h = HIGHT/8.0;

	for (i = 0 ; i < 5 ; i++) {
		utilityRects[i+3].x = WIDTH/45.0;
		utilityRects[i+3].w = buttonWidth;
		utilityRects[i+3].h = buttonHight;
	}

	utilityRects[3].y = utilityRects[2].y + utilityRects[2].h + 30;
	utilityRects[4].y = utilityRects[3].y + buttonHight + 40;
	utilityRects[5].y = utilityRects[4].y + buttonHight + 40;
	utilityRects[6].y = utilityRects[5].y + buttonHight + 40;
	utilityRects[7].y = utilityRects[6].y + buttonHight + 40;

	utilityRects[8].w = WIDTH/7;
	utilityRects[8].h = buttonHight;
	utilityRects[8].x = utilityRects[1].x + ((utilityRects[1].w) - utilityRects[8].w)/2.0;
	utilityRects[8].y = utilityRects[1].y + BOARD_EDGE + 20;

}

int symbolToTexIndex(char symbol) {
	switch(symbol) {
	case '_': return 0;
	case 'K': return 1;
	case 'k': return 2;
	case 'Q': return 3;
	case 'q': return 4;
	case 'B': return 5;
	case 'b': return 6;
	case 'R': return 7;
	case 'r': return 8;
	case 'N': return 9;
	case 'n': return 10;
	case 'M': return 11;
	case 'm': return 12;
	}
	return -1;
}

int whichEmptyTile(int row, int col) { // return 0 for black, 1 for white
	if (((row % 2 == 0) && (col % 2 != 0)) || ((row % 2 != 0) && (col % 2 == 0)))
		return 0; // empty black
	return 1; // empty white
}

void setGameTiles(SDL_Rect tiles[ROWS][COLUMNS], SDL_Rect boardRect, int squareEdge, int bord) {
	int row, col;
	for (row = 0 ; row < ROWS ; row++ ) {
		for (col = 0 ; col < COLUMNS ; col++) {
			tiles[row][col].x = boardRect.x + bord + col*squareEdge;
			tiles[row][col].y = boardRect.y + bord + row*squareEdge;
			tiles[row][col].w = squareEdge;
			tiles[row][col].h = squareEdge;
		}
	}
}

void indexesFromPos(float x, float y, SDL_Rect boardRect, int pos[2]) {
	pos[0] = (y - boardRect.y - border)/SQUARE_EDGE;
	pos[1] = (x - boardRect.x - border)/SQUARE_EDGE;
}

void posFromIndexes(int row, int col, SDL_Rect boardRect, float *x, float *y) {
	*x = boardRect.x + border + col*SQUARE_EDGE;
	*y = boardRect.y + border + row*SQUARE_EDGE;
}

bool setGameTextures(SDL_Window* gameWindow, SDL_Renderer* rend, SDL_Texture** textures) {
	if (setBoardTex(gameWindow, rend, textures) == false) {
		return false;
	}

	if ((textures[13] = getTex(gameWindow, rend, GAME_BACKGROUND)) == NULL) {
		destroyTextures(textures,14);
		return false;
	}
	if ((textures[15] = getTex(gameWindow, rend, CHESS_GAME_HEADLINE)) == NULL) {
		destroyTextures(textures,16);
		return false;
	}
	if ((textures[16] = getTex(gameWindow, rend, MAIN_BUTTON_PATH)) == NULL) { // starting buttons
		destroyTextures(textures,17);
		return false;
	}
	if ((textures[17] = getTex(gameWindow, rend, RESTART_BUTTON_PATH)) == NULL) {
		destroyTextures(textures,18);
		return false;
	}
	if ((textures[18] = getTex(gameWindow, rend, SAVE_BUTTON_PATH)) == NULL) {
		destroyTextures(textures,19);
		return false;
	}
	if ((textures[19] = getTex(gameWindow, rend, GAME_LOAD_BUTTON_PATH)) == NULL) {
		destroyTextures(textures,20);
		return false;
	}
	if ((textures[20] = getTex(gameWindow, rend, GAME_QUIT_BUTTON_PATH)) == NULL) {
		destroyTextures(textures,21);
		return false;
	}
	if ((textures[21] = getTex(gameWindow, rend, UNDO_BUTTON_PATH)) == NULL) {
		destroyTextures(textures,22);
		return false;
	}
	if ((textures[22] = getTex(gameWindow, rend, RED_PATH)) == NULL) { // color tiles
		destroyTextures(textures,23);
		return false;
	}
	if ((textures[23] = getTex(gameWindow, rend, YELLOW_PATH)) == NULL) {
		destroyTextures(textures,24);
		return false;
	}
	if ((textures[24] = getTex(gameWindow, rend, ORANGE_PATH)) == NULL) {
		destroyTextures(textures,25);
		return false;
	}
	if ((textures[25] = getTex(gameWindow, rend, GREEN_PATH)) == NULL) {
		destroyTextures(textures,26);
		return false;
	}
	SDL_SetTextureAlphaMod(textures[22], 150);
	SDL_SetTextureAlphaMod(textures[23], 150);
	SDL_SetTextureAlphaMod(textures[24], 150);
	SDL_SetTextureAlphaMod(textures[25], 150);
	if ((textures[26] = getTex(gameWindow, rend, BLACK_WINS)) == NULL) {
		destroyTextures(textures,27);
		return false;
	}
	if ((textures[27] = getTex(gameWindow, rend, WHITE_WINS)) == NULL) {
		destroyTextures(textures,28);
		return false;
	}

	return true;
}

bool setBoardTex(SDL_Window* window, SDL_Renderer* rend, SDL_Texture** textures) {
	if ((textures[0] = getTex(window, rend, EMPTY_PATH)) == NULL) {
		destroyTextures(textures,1);
		return false;
	}
	if ((textures[1] = getTex(window, rend, BLACK_KING_PATH)) == NULL) {
		destroyTextures(textures,2);
		return false;
	}
	if ((textures[2] = getTex(window, rend, WHITE_KING_PATH)) == NULL) {
		destroyTextures(textures,3);
		return false;
	}
	if ((textures[3] = getTex(window, rend, BLACK_QUEEN_PATH)) == NULL) {
		destroyTextures(textures,4);
		return false;
	}
	if ((textures[4] = getTex(window, rend, WHITE_QUEEN_PATH)) == NULL) {
		destroyTextures(textures,5);
		return false;
	}
	if ((textures[5] = getTex(window, rend, BLACK_BISHOP_PATH)) == NULL) {
		destroyTextures(textures,6);
		return false;
	}
	if ((textures[6] = getTex(window, rend, WHITE_BISHOP_PATH)) == NULL) {
		destroyTextures(textures,7);
		return false;
	}
	if ((textures[7] = getTex(window, rend, BLACK_ROOK_PATH)) == NULL) {
		destroyTextures(textures,8);
		return false;
	}
	if ((textures[8] = getTex(window, rend, WHITE_ROOK_PATH)) == NULL) {
		destroyTextures(textures,9);
		return false;
	}
	if ((textures[9] = getTex(window, rend, BLACK_KNIGHT_PATH)) == NULL) {
		destroyTextures(textures,10);
		return false;
	}
	if ((textures[10] = getTex(window, rend, WHITE_KNIGHT_PATH)) == NULL) {
		destroyTextures(textures,11);
		return false;
	}
	if ((textures[11] = getTex(window, rend, BLACK_PAWN_PATH)) == NULL) {
		destroyTextures(textures,12);
		return false;
	}
	if ((textures[12] = getTex(window, rend, WHITE_PAWN_PATH)) == NULL) {
		destroyTextures(textures,13);
		return false;
	}
	// no 13!!!
	if ((textures[14] = getTex(window, rend, CHESS_BOARD)) == NULL) {
		destroyTextures(textures,13);
		SDL_DestroyTexture(textures[14]);
		return false;
	}
	return true;
}



/*
int main(int argc, char *argv[]) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return 1;
	}
	SDL_Window* window = createWindow();
	SDL_Renderer* rend = createRenderer(window);
	Game *game = createGame(2,2,1);
	SDL_Texture* textures[NUM_OF_GAME_TEXTURES];
	if (setGameTextures(window, rend, textures) == false) {
		return 0;
	}
	SDL_ShowWindow(window);
	runGameWindow(game, window, rend, textures);
	destroyGame(game);
	destroyTextures(textures, NUM_OF_GAME_TEXTURES);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}*/


