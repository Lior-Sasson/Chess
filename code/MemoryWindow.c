#include "MemoryWindow.h"

ChessWindow runMemoryWindow(Game *game, SDL_Window* memoryWindow, SDL_Renderer* rend, SDL_Renderer* gameRend, SDL_Texture** textures, SDL_Texture** boardTex, ChessWindow back, char saveOrLoad, bool *occupied, char slots[NUM_OF_SLOTS][PATH_LENGTH]) {
	SDL_Event e;
	int slot = 0;
	SDL_Rect memoryRects[NUM_OF_MEM_RECTS];
	setMemoryRects(memoryRects);
	Game *cpy = copyGame(game);

	SDL_Rect tiles[ROWS][COLUMNS];
	// copy the pieces
	setGameTiles(tiles, memoryRects[0], (SQUARE_EDGE/1.4), (border/1.4));


	presentMemTextures(rend, textures, memoryRects,saveOrLoad, true, false);
	if (occupied[0]) {
		loadGame(cpy, slots[0]);
		SDL_RenderCopy(rend, boardTex[14], NULL, &memoryRects[0]);

		setBoardRend(rend, cpy, boardTex, tiles);
		SDL_RenderPresent(rend);
	}


	while (true) {
		SDL_WaitEvent(&e);
		switch (e.type) {
			case SDL_WINDOWEVENT:
				if (e.window.event == SDL_WINDOWEVENT_CLOSE) {
					destroyGame(cpy);
					return QUIT_GAME;
				}
				break;
			case SDL_QUIT:
				destroyGame(cpy);
				return QUIT_GAME;
				break;
			case SDL_KEYDOWN:
				if (e.key.keysym.sym == SDLK_ESCAPE) {
					destroyGame(cpy);
					return QUIT_GAME;
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (isInRect(e, memoryRects[3])) { // pressed back
					destroyGame(cpy);
					return back;
				}
				if (isInRect(e, memoryRects[1]) && slot > 0) { // left arrow
					slot--;

					if (slot == 0)
						presentMemTextures(rend, textures, memoryRects,saveOrLoad, true, false);
					else
						presentMemTextures(rend, textures, memoryRects,saveOrLoad, true, true);
					if (occupied[slot]) {
						loadGame(cpy, slots[slot]);
						copyGameBoard(cpy, rend, boardTex, memoryRects, tiles);
					}
					break;
				}
				if (isInRect(e, memoryRects[2]) && slot < NUM_OF_SLOTS-1) { // right arrow
					slot++;

					if (slot == NUM_OF_SLOTS-1)
						presentMemTextures(rend, textures, memoryRects,saveOrLoad, false, true);
					else
						presentMemTextures(rend, textures, memoryRects,saveOrLoad, true, true);
					if (occupied[slot]) {
						loadGame(cpy, slots[slot]);
						copyGameBoard(cpy, rend, boardTex, memoryRects, tiles);
					}
					break;
				}
				if (isInRect(e, memoryRects[5])) { // pressed save/load
					if (saveOrLoad == 's') { // save
						occupied[slot] = true;
						saveGame(game, slots[slot]);
						SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "save message", "Your game was saved.", NULL);
						destroyGame(cpy);
						return back;
					}
					else { // load
						if (!(occupied[slot])) {
							SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "load message", "No game is saved under that slot.", NULL);
							break;
						}
						loadGame(game, slots[slot]);
						destroyGame(cpy);
						return back;
					}
				}
		}
	}
	destroyGame(cpy);
	return QUIT_GAME;
}


void setSlots(char slots[NUM_OF_SLOTS][11], bool occupied[NUM_OF_SLOTS]) {
	int i;
	for (i = 0 ; i < NUM_OF_SLOTS ; i++) {
		slots[i][0] = 'g';
		slots[i][1] = 'a';
		slots[i][2] = 'm';
		slots[i][3] = 'e';
		slots[i][4] = '0' + i/10;
		slots[i][5] = '0' + i%10;
		slots[i][6] = '.';
		slots[i][7] = 't';
		slots[i][8] = 'x';
		slots[i][9] = 't';
		slots[i][10] = '\0';

		if( access( slots[i], R_OK|W_OK ) == -1 ) // file doesn't exist
			occupied[i] = false;
		else
			occupied[i] = true;
	}
}


void setMemoryRects(SDL_Rect *memoryRects) {
	int i;
	int headlineWidth = WIDTH/2.5, boardEdge = BOARD_EDGE/1.4, b = 0.12*(boardEdge/6.17);
	int arrowWidth, mem = WIDTH/6.0;
	// headline rect
	memoryRects[4].x = (WIDTH-headlineWidth)/2.0;
	memoryRects[4].y = HIGHT/15.0;
	memoryRects[4].w = headlineWidth;
	memoryRects[4].h = HIGHT/6.0;
	// empty slot
	memoryRects[0].x = (WIDTH - boardEdge - b)/2.0;
	memoryRects[0].y = memoryRects[4].y + memoryRects[4].h + 20;
	memoryRects[0].w = boardEdge + b;
	memoryRects[0].h = boardEdge + b;
	// board rect
	memoryRects[6].x = memoryRects[0].x + b;
	memoryRects[6].y = memoryRects[0].y + b;
	memoryRects[6].w = boardEdge;
	memoryRects[6].h = boardEdge;
	arrowWidth = memoryRects[0].w / 5.0;
	// arrows
	for (i = 1 ; i < 3 ; i++) {
		memoryRects[i].y = 0.5*(3*memoryRects[6].y + arrowWidth);
		memoryRects[i].w = arrowWidth;
		memoryRects[i].h = arrowWidth/1.2;
	}
	memoryRects[1].x = memoryRects[0].x - arrowWidth - 20;        // left arrow
	memoryRects[2].x = memoryRects[0].x + memoryRects[0].w + 20; // right arrow
	// back rect
	memoryRects[3].x = 30;
	memoryRects[3].y = (0.875)*HIGHT;
	memoryRects[3].w = WIDTH/6.0;
	memoryRects[3].h = HIGHT/10.0;
	// save/load rect
	memoryRects[5].x = memoryRects[0].x + 0.5*memoryRects[0].w - 0.5*mem;
	memoryRects[5].y = memoryRects[0].y + memoryRects[0].h + 20;
	memoryRects[5].w = mem;
	memoryRects[5].h = HIGHT/12.0;

}


bool setMemoryTextures (SDL_Window* memoryWindow, SDL_Renderer* rend, SDL_Texture** textures) {
	if ((textures[0] = getTex(memoryWindow, rend, SAVE_HEADLINE_PATH)) == NULL) {
		destroyTextures(textures,1);
		return false;
	}
	if ((textures[1] = getTex(memoryWindow, rend, LOAD_HEADLINE_PATH)) == NULL) {
		destroyTextures(textures,2);
		return false;
	}
	if ((textures[2] = getTex(memoryWindow, rend, SAVE_MEM_PATH)) == NULL) {
		destroyTextures(textures,3);
		return false;
	}
	if ((textures[3] = getTex(memoryWindow, rend, LOAD_MEM_PATH)) == NULL) {
		destroyTextures(textures,4);
		return false;
	}
	if ((textures[4] = getTex(memoryWindow, rend, EMPTY_SLOT_PATH)) == NULL) {
		destroyTextures(textures,5);
		return false;
	}
	if ((textures[5] = getTex(memoryWindow, rend, LEFT_ARROW_PATH)) == NULL) {
		destroyTextures(textures,6);
		return false;
	}
	if ((textures[6] = getTex(memoryWindow, rend, RIGHT_ARROW_PATH)) == NULL) {
		destroyTextures(textures,7);
		return false;
	}
	if ((textures[7] = getTex(memoryWindow, rend, MEM_BACK_PATH)) == NULL) {
		destroyTextures(textures,8);
		return false;
	}
	return true;
}


void copyGameBoard(Game *game, SDL_Renderer* rend, SDL_Texture** boardTex, SDL_Rect *memoryRects, SDL_Rect tiles[ROWS][COLUMNS]) {
	// copy game board
	SDL_RenderCopy(rend, boardTex[14], NULL, &memoryRects[0]);
	setBoardRend(rend, game, boardTex, tiles);
	SDL_RenderPresent(rend);
}


void presentMemTextures(SDL_Renderer* rend, SDL_Texture** textures, SDL_Rect *memoryRects, char saveOrLoad, bool right, bool left) {
	// ensure renderer supports transparency
	SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);
	// clear
	SDL_SetRenderDrawColor(rend, 42, 42, 42, 0);
	SDL_RenderClear(rend);
	if (saveOrLoad == 's') {
		SDL_RenderCopy(rend, textures[0], NULL, &(memoryRects[4])); // save headline
		SDL_RenderCopy(rend, textures[2], NULL, &(memoryRects[5])); // save button
	}
	else {
		SDL_RenderCopy(rend, textures[1], NULL, &(memoryRects[4])); // load headline
		SDL_RenderCopy(rend, textures[3], NULL, &(memoryRects[5])); // load button
	}
	SDL_RenderCopy(rend, textures[4], NULL, &(memoryRects[0]));
	SDL_RenderCopy(rend, textures[7], NULL, &(memoryRects[3]));
	if (left)
		SDL_RenderCopy(rend, textures[5], NULL, &(memoryRects[1]));
	if (right)
		SDL_RenderCopy(rend, textures[6], NULL, &(memoryRects[2]));
	SDL_RenderPresent(rend);
}




/*
int main(int argc, char *argv[]) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return 1;
	}
	SDL_Window* window = createWindow();
	SDL_Renderer* rend = createRenderer(window);
	Game *game = createGame(1,2,1);
	SDL_Texture* textures[NUM_OF_MEM_TEXTURES];
	bool occupied[NUM_OF_SLOTS];
	char slots[NUM_OF_SLOTS][PATH_LENGTH];
	setMemoryTextures(window, rend, textures);
	setSlots(slots, occupied);
	SDL_ShowWindow(window);
	runMemoryWindow(game, window, rend, textures, NULL, MAIN_WINDOW, 'l', occupied, slots);

	destroyTextures(textures, NUM_OF_MEM_TEXTURES);
	destroyGame(game);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}*/
