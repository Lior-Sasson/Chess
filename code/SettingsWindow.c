#include "SettingsWindow.h"

ChessWindow runSettingsWindow(Game *game, SDL_Window* window, SDL_Renderer* rend, SDL_Texture* textures[NUM_OF_SET_TEXTURES]) {
	int step = 1;
	SDL_Event e;
	SDL_Rect rects[NUM_OF_SET_RECTS];
	setSettingsRects(rects);
	/*
	 * headline		 0
	 * game mode	 1
	 * player1		 2	13
	 * player2 		 3	14
	 * difficulty 	 4
	 * amateur		 5	15
	 * easy			 6	16
	 * moderate		 7	17
	 * hard 		 8	18
	 * expert		 9	19
	 * color		 10
	 * black 		 11	20
	 * white		 12	21
	 *
	 * back 		 22
	 * next 		 23
	 * start game    24
	 */

	game->gameMode = 1;
	game->difficulty = 2;
	game->userColor = 1;
	//SDL_ShowWindow(window);
	showStep1(rend, game, textures, rects, true);

	while (true) {
		SDL_WaitEvent(&e);
		switch (e.type) {
			case SDL_WINDOWEVENT:
				if (e.window.event == SDL_WINDOWEVENT_CLOSE)
					return QUIT_GAME;
				if (e.window.event == SDL_WINDOWEVENT_HIDDEN)
					SDL_ShowWindow(window);
				break;
			case SDL_QUIT: // pressed x
				return QUIT_GAME;
				break;
			case SDL_KEYDOWN:
				if (e.key.keysym.sym == SDLK_ESCAPE) // pressed esc
					return QUIT_GAME;
				break;
			case SDL_MOUSEBUTTONUP:
				if (step == 1) { // game mode step
					showStep1(rend, game, textures, rects, game->gameMode == 1);
					if (isInRect(e, rects[2])) { // 1-player
						game->gameMode = 1;
						showStep1(rend, game, textures, rects, true);
						break;
					}
					else if (isInRect(e, rects[3])) { // 2-player
						game->gameMode = 2;
						showStep1(rend, game, textures, rects, false);
						break;
					}
					if (isInRect(e, rects[11])) { // back
						return MAIN_WINDOW;
					}
					if (game->gameMode == 1 && isInRect(e, rects[12])) { // next
						step = 2;
						showStep2(rend, game, textures, rects);
						break;
					}
					if (isInRect(e, rects[13])) { // new game
						return GAME_WINDOW;
					}
					break;
				}
				if (game->gameMode == 1) {
					if (step == 2) { // difficulty step
						showStep2(rend, game, textures, rects);
						if (isInRect(e, rects[5])) { // amateur
							game->difficulty = 1;
							showStep2(rend, game, textures, rects);
							break;
						}
						if (isInRect(e, rects[6])) { // easy
							game->difficulty = 2;
							showStep2(rend, game, textures, rects);
							break;
						}
						if (isInRect(e, rects[7])) { // moderate
							game->difficulty = 3;
							showStep2(rend, game, textures, rects);
							break;
						}
						if (isInRect(e, rects[8])) { // hard
							game->difficulty = 4;
							showStep2(rend, game, textures, rects);
							break;
						}
						if (isInRect(e, rects[9])) { // expert
							game->difficulty = 5;
							showStep2(rend, game, textures, rects);
							break;
						}
						if (isInRect(e, rects[11])) { // back
							step = 1;
							showStep1(rend, game, textures, rects, true);
							break;
						}
						if (isInRect(e, rects[12])) { // next
							step = 3;
							showStep3(rend, game, textures, rects);
							break;
						}
						if (isInRect(e, rects[13])) { // new game
							return GAME_WINDOW;
						}
						break;
					}
					if (step == 3) { // color step
						showStep3(rend, game, textures, rects);
						if (isInRect(e, rects[2])) { // black
							game->userColor = 0;
							showStep3(rend, game, textures, rects);
							break;
						}
						else if (isInRect(e, rects[3])) { // white
							game->userColor = 1;
							showStep3(rend, game, textures, rects);
							break;
						}
						if (isInRect(e, rects[11])) { // back
							step = 2;
							showStep2(rend, game, textures, rects);
							break;
						}
						if (isInRect(e, rects[13])) { // new game
							return GAME_WINDOW;
						}
						break;
					}
				}
				break;
		}
	}
	return QUIT_GAME;
}

void showUtility(SDL_Renderer* rend, SDL_Texture **textures, SDL_Rect *rects, bool next) {
	SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND); // ensure renderer supports transparency
	SDL_SetRenderDrawColor(rend, 0, 0, 0, 0);
	SDL_RenderClear(rend);
	SDL_RenderCopy(rend, textures[0], NULL, &(rects[0])); // headline
	SDL_RenderCopy(rend, textures[22], NULL, &(rects[11])); // back
	if (next)
		SDL_RenderCopy(rend, textures[23], NULL, &(rects[12])); // next
	SDL_RenderCopy(rend, textures[24], NULL, &(rects[13])); // start game
}

void showStep1(SDL_Renderer* rend, Game *game, SDL_Texture **textures, SDL_Rect *rects, bool next) { // game mode
	showUtility(rend, textures, rects, next);
	SDL_RenderCopy(rend, textures[1], NULL, &(rects[1])); // game mode
	if (game->gameMode == 1) {
		SDL_RenderCopy(rend, textures[13], NULL, &(rects[2])); // pressed 1-player
		SDL_RenderCopy(rend, textures[3], NULL, &(rects[3])); // player 2
	}
	else {
		SDL_RenderCopy(rend, textures[2], NULL, &(rects[2])); // player1
		SDL_RenderCopy(rend, textures[14], NULL, &(rects[3])); // pressed 2-player
	}
	SDL_RenderPresent(rend);
}

void showStep2(SDL_Renderer* rend, Game *game, SDL_Texture **textures, SDL_Rect *rects) { // difficulty
	int i;
	showUtility(rend, textures, rects, true);
	for (i = 4 ; i <= 9 ; i++) {
		SDL_RenderCopy(rend, textures[i], NULL, &(rects[i]));
	}
	SDL_RenderCopy(rend, textures[game->difficulty + 4 + 10], NULL, &(rects[game->difficulty + 4])); // the pressed one
	SDL_RenderPresent(rend);
}

void showStep3(SDL_Renderer* rend, Game *game, SDL_Texture **textures, SDL_Rect *rects) { // color
	showUtility(rend, textures, rects, false);
	SDL_RenderCopy(rend, textures[10], NULL, &(rects[10])); // color

	if (game->userColor == 0) { // black
		SDL_RenderCopy(rend, textures[20], NULL, &(rects[2])); // pressed black
		SDL_RenderCopy(rend, textures[12], NULL, &(rects[3])); // white
	}
	else {
		SDL_RenderCopy(rend, textures[11], NULL, &(rects[2])); // black
		SDL_RenderCopy(rend, textures[21], NULL, &(rects[3])); // pressed white
	}
	SDL_RenderPresent(rend);
}


bool setSettingsTextures(SDL_Window* window, SDL_Renderer* rend, SDL_Texture** textures) {
	if ((textures[0] = getTex(window, rend, SETTINGS_HEADLINE)) == NULL) {
		destroyTextures(textures,1);
		return false;
	}
	if ((textures[1] = getTex(window, rend, GAME_MODE_PATH)) == NULL) {
		destroyTextures(textures,2);
		return false;
		}
	if ((textures[2] = getTex(window, rend, PLAYER1_PATH)) == NULL) {
		destroyTextures(textures,3);
		return false;
	}
	if ((textures[3] = getTex(window, rend, PLAYER2_PATH)) == NULL) {
		destroyTextures(textures,4);
		return false;
	}
	if ((textures[4] = getTex(window, rend, LEVEL_PATH)) == NULL) {
		destroyTextures(textures,5);
		return false;
	}
	if ((textures[5] = getTex(window, rend, AMATEUR_PATH)) == NULL) { // difficulty1 (5-9)
		destroyTextures(textures,6);
		return false;
	}
	if ((textures[6] = getTex(window, rend, EASY_PATH)) == NULL) {
		destroyTextures(textures,7);
		return false;
	}
	if ((textures[7] = getTex(window, rend, MODERATE_PATH)) == NULL) {
		destroyTextures(textures,8);
		return false;
	}
	if ((textures[8] = getTex(window, rend, HARD_PATH)) == NULL) {
		destroyTextures(textures,9);
		return false;
	}
	if ((textures[9] = getTex(window, rend, EXPERT_PATH)) == NULL) {
		destroyTextures(textures,10);
		return false;
	}
	if ((textures[10] = getTex(window, rend, COLOR_PATH)) == NULL) {
		destroyTextures(textures,11);
		return false;
	}
	if ((textures[11] = getTex(window, rend, BLACK_PATH)) == NULL) {
		destroyTextures(textures,12);
		return false;
	}
	if ((textures[12] = getTex(window, rend, WHITE_PATH)) == NULL) { // ends not pressed
		destroyTextures(textures,13);
		return false;
	}
	if ((textures[13] = getTex(window, rend, PLAYER1_PATH2)) == NULL) {
		destroyTextures(textures,14);
		return false;
	}
	if ((textures[14] = getTex(window, rend, PLAYER2_PATH2)) == NULL) {
		destroyTextures(textures,15);
		return false;
	}
	if ((textures[15] = getTex(window, rend, AMATEUR_PATH2)) == NULL) { // difficulty1 (15-19)
		destroyTextures(textures,16);
		return false;
	}
	if ((textures[16] = getTex(window, rend, EASY_PATH2)) == NULL) {
		destroyTextures(textures,17);
		return false;
	}
	if ((textures[17] = getTex(window, rend, MODERATE_PATH2)) == NULL) {
		destroyTextures(textures,18);
		return false;
	}
	if ((textures[18] = getTex(window, rend, HARD_PATH2)) == NULL) {
		destroyTextures(textures,19);
		return false;
	}
	if ((textures[19] = getTex(window, rend, EXPERT_PATH2)) == NULL) {
		destroyTextures(textures,20);
		return false;
	}
	if ((textures[20] = getTex(window, rend, BLACK_PATH2)) == NULL) {
		destroyTextures(textures,21);
		return false;
	}
	if ((textures[21] = getTex(window, rend, WHITE_PATH2)) == NULL) {
		destroyTextures(textures,22);
		return false;
	}
	if ((textures[22] = getTex(window, rend, SET_BACK)) == NULL) { // back 22
		destroyTextures(textures,23);
		return false;
	}
	if ((textures[23] = getTex(window, rend, SET_NEXT)) == NULL) { // next 23
		destroyTextures(textures,24);
		return false;
	}
	if ((textures[24] = getTex(window, rend, SET_START)) == NULL) { // start 24
		destroyTextures(textures,25);
		return false;
	}

	return true;
}

void setSettingsRects(SDL_Rect *rects) {
	int i, headlineWidth = WIDTH/1.4;
	// headline
	rects[0].x = (WIDTH-headlineWidth)/2.0;
	rects[0].y = HIGHT/25.0;
	rects[0].w = headlineWidth;
	rects[0].h = HIGHT/6.0;
	// game mode
	rects[1].x = (WIDTH-390)/2.0;
	rects[1].y = rects[0].y + 150;
	rects[1].w = 390;
	rects[1].h = 80;
	// 1-player/ black
	rects[2].x = (WIDTH-230)/2.0;
	rects[2].y = rects[1].y + 120;
	rects[2].w = 230;
	rects[2].h = 70;
	// 2-player/ white
	rects[3].x = (WIDTH-230)/2.0;
	rects[3].y = rects[2].y + 100;
	rects[3].w = 230;
	rects[3].h = 70;
	// difficulty level
	rects[4].x = (WIDTH-500)/2.0;
	rects[4].y = rects[0].y + 150;
	rects[4].w = 500;
	rects[4].h = 80;
	// 5,6,7,8,9
	for (i = 0 ; i < 5 ; i++) {
		rects[i+5].w = 250;
		rects[i+5].h = 70;
	}
	rects[5].y = rects[4].y + rects[4].h + 70;
	rects[6].y = rects[5].y;
	rects[7].y = rects[5].y;
	rects[8].y = rects[5].y + rects[5].h + 80;
	rects[9].y = rects[8].y;
	rects[5].x = 50;
	rects[6].x = rects[5].x + rects[5].w + 50;
	rects[7].x = rects[6].x + rects[6].w + 50;
	rects[8].x = 200;
	rects[9].x = rects[8].x + rects[8].w + 50;
	// color
	rects[10].x = (WIDTH-250)/2.0;
	rects[10].y = rects[0].y + 150;
	rects[10].w = 250;
	rects[10].h = 80;
	// back button
	rects[11].x = 30;
	rects[11].y = (0.875)*HIGHT;
	rects[11].w = WIDTH/6.0;
	rects[11].h = HIGHT/10.0;
	// next button
	rects[12].x = 800;
	rects[12].y = (0.875)*HIGHT;
	rects[12].w = WIDTH/6.0;
	rects[12].h = HIGHT/10.0;
	// start game button
	rects[13].x = 365;
	rects[13].y = (0.875)*HIGHT;
	rects[13].w = WIDTH/4.0;
	rects[13].h = HIGHT/10.0;
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
	SDL_Texture* textures[NUM_OF_SET_TEXTURES];
	printf("1\n");
	if (setSettingsTextures(window, rend, textures) == false) return 0;
	printf("2\n");
	runSettingsWindow(game, window, rend, textures);
	printf("3\n");
	destroyGame(game);
	destroyTextures(textures, NUM_OF_SET_TEXTURES);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}*/
