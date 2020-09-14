#include "GUI.h"

bool runGame() {
	//int i;
	Game *game = createGame(2,2,1);
	SDL_Window* windows[NUM_OF_WINDOWS]; // main, settings, game, memory
	SDL_Renderer* renders[NUM_OF_WINDOWS];
	SDL_Texture* mainTex[NUM_OF_MAIN_RECTS];
	SDL_Texture* gameTex[NUM_OF_GAME_TEXTURES];
	SDL_Texture* setTex[NUM_OF_SET_TEXTURES];
	SDL_Texture* memTex[NUM_OF_MEM_TEXTURES];
	SDL_Texture* boardTex[NUM_OF_BOARD_TEX];
	ChessWindow nextWindow = MAIN_WINDOW, back;
	bool occupied[NUM_OF_SLOTS];
	char slots[NUM_OF_SLOTS][PATH_LENGTH];

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return 1;
	}

	// create windows and their renderers
	if (createWindows(windows) == false)
		return false;
	if (createRenderers(windows, renders) == false)
		return false;
	
	// create the windows' textures
	if (setMainTextures(windows[0], renders[0], mainTex) == false) {
		destroyWindows(windows, renders);
		return false;
	}
	if (setSettingsTextures(windows[1], renders[1], setTex) == false) {
		destroyTextures(mainTex, NUM_OF_MAIN_RECTS);
		destroyWindows(windows, renders);
		return false;
	}
	if (setGameTextures(windows[2], renders[2], gameTex) == false) {
		destroyTextures(mainTex, NUM_OF_MAIN_RECTS);
		destroyTextures(setTex, NUM_OF_SET_TEXTURES);
		destroyWindows(windows, renders);
		return false;	
	}
	if (setMemoryTextures(windows[3], renders[3], memTex) == false) {
		destroyTextures(mainTex, NUM_OF_MAIN_RECTS);
		destroyTextures(setTex, NUM_OF_SET_TEXTURES);
		destroyTextures(gameTex, NUM_OF_GAME_TEXTURES);
		destroyWindows(windows, renders);
		return false;
	}
	if (setBoardTex(windows[3], renders[3], boardTex) == false) {
		destroyTextures(mainTex, NUM_OF_MAIN_RECTS);
		destroyTextures(setTex, NUM_OF_SET_TEXTURES);
		destroyTextures(gameTex, NUM_OF_GAME_TEXTURES);
		destroyTextures(memTex, NUM_OF_MEM_TEXTURES);
		destroyWindows(windows, renders);
		return false;
	}

	setSlots(slots, occupied);

	while (nextWindow != QUIT_GAME) {
		switch (nextWindow) {
		case MAIN_WINDOW: 
			SDL_ShowWindow(windows[0]);
			nextWindow = runMainWindow(windows[0], renders[0], mainTex);
			game -> state = GAME; // initialize game
			initializeBoard(game);
			game->currentPlayer = 1;
			if (nextWindow == LOAD_WINDOW)
				back = MAIN_WINDOW;
			SDL_HideWindow(windows[0]);
			break;
		case SETTINGS_WINDOW:
			SDL_ShowWindow(windows[1]);
			nextWindow = runSettingsWindow(game, windows[1], renders[1], setTex);
			SDL_HideWindow(windows[1]);
			break;
		case GAME_WINDOW:
			SDL_ShowWindow(windows[2]);
			nextWindow = runGameWindow(game, windows[2], renders[2], gameTex);
			if (nextWindow == LOAD_WINDOW)
				back = GAME_WINDOW;
			SDL_HideWindow(windows[2]);
			break;
		case SAVE_WINDOW:
			SDL_ShowWindow(windows[3]);
			nextWindow = runMemoryWindow(game, windows[3], renders[3], renders[2], memTex, boardTex, GAME_WINDOW, 's', occupied, slots);
			SDL_HideWindow(windows[3]);
			break;
		case LOAD_WINDOW:
			SDL_ShowWindow(windows[3]);
			nextWindow = runMemoryWindow(game, windows[3], renders[3], renders[2], memTex, boardTex, back, 'l', occupied, slots);
			SDL_HideWindow(windows[3]);
			break;
		case ERROR:
			printf("1 %d\n", nextWindow);
			destroyGame(game);
			destroyGUI(windows, renders, mainTex, gameTex, setTex, memTex, boardTex);
			return false;
		default:
			break;
		}
	}
	destroyGame(game);
	destroyGUI(windows, renders, mainTex, gameTex, setTex, memTex, boardTex);

	return true;
}

void destroyGUI(SDL_Window** windows, SDL_Renderer** renders, SDL_Texture** mainTex, SDL_Texture** gameTex, SDL_Texture** setTex, SDL_Texture** memTex, SDL_Texture** boardTex) {
	// destroy textures
	destroyTextures(mainTex, NUM_OF_MAIN_RECTS);
	destroyTextures(gameTex, NUM_OF_GAME_TEXTURES);
	destroyTextures(setTex, NUM_OF_SET_TEXTURES);
	destroyTextures(memTex, NUM_OF_MEM_TEXTURES);
	destroyTextures(boardTex, 13);
	SDL_DestroyTexture(boardTex[14]);
	destroyWindows(windows, renders);
}

void destroyWindows(SDL_Window** windows, SDL_Renderer** renders) {
	int i;
	for(i = 0 ; i < NUM_OF_WINDOWS ; i++) {
		SDL_DestroyRenderer(renders[i]);
		SDL_DestroyWindow(windows[i]);
	}
}

bool createWindows(SDL_Window* windows[NUM_OF_WINDOWS]){
	SDL_Surface* surf = SDL_LoadBMP("chess_icon.bmp");
	// set a specific color (green: 0,255,0) to transparent
	SDL_SetColorKey(surf, SDL_TRUE, SDL_MapRGB(surf->format, 0, 255, 0));
	if ((windows[0] = createWindow()) == NULL)
		return false;
	if ((windows[1] = createWindow()) == NULL) {
		destroyJustWindows(windows, 1);
		return false;
	}
	if ((windows[2] = createWindow()) == NULL) {
		destroyJustWindows(windows, 2);
		return false;
	}
	if ((windows[3] = createWindow()) == NULL) {
		destroyJustWindows(windows, 3);
		return false;
	}
	SDL_SetWindowIcon(windows[0], surf);
	SDL_SetWindowIcon(windows[1], surf);
	SDL_SetWindowIcon(windows[2], surf);
	SDL_SetWindowIcon(windows[3], surf);
	SDL_FreeSurface(surf);
	return true;	

}

bool createRenderers(SDL_Window* windows[NUM_OF_WINDOWS], SDL_Renderer* renders[NUM_OF_WINDOWS]) {
	if ((renders[0] = createRenderer(windows[0])) == NULL) {
		destroyJustWindows(windows, NUM_OF_WINDOWS);
		return false;
	}
	if ((renders[1] = createRenderer(windows[1])) == NULL) {
		SDL_DestroyRenderer(renders[0]);
		destroyJustWindows(windows, NUM_OF_WINDOWS);
		return false;
	}
	if ((renders[2] = createRenderer(windows[2])) == NULL) {
		SDL_DestroyRenderer(renders[0]);
		SDL_DestroyRenderer(renders[1]);
		destroyJustWindows(windows, NUM_OF_WINDOWS);
		return false;	
	}
	if ((renders[3] = createRenderer(windows[3])) == NULL) {
		SDL_DestroyRenderer(renders[0]);
		SDL_DestroyRenderer(renders[1]);
		SDL_DestroyRenderer(renders[2]);
		destroyJustWindows(windows, NUM_OF_WINDOWS);
		return false;
	}
	return true;
}

void destroyJustWindows(SDL_Window** windows, int num) {
	int i;
	for (i = 0 ; i < num ; i++)
		SDL_DestroyWindow(windows[i]);
}

/*
int main(int argc, char *argv[]) {
	runGame();
	return 0;
}*/

