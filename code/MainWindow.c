#include "MainWindow.h"
SDL_Window* createWindow() {
	SDL_Window* window = SDL_CreateWindow(
		"Chess- Final Project",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WIDTH,
		HIGHT,
		SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);

	// make sure window was created successfully
	if (window == NULL ) {
		printf("ERROR: unable to create window: %s\n", SDL_GetError());
		return NULL;
	}
	return window;
}

SDL_Renderer* createRenderer(SDL_Window* window) {
	SDL_Renderer* rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	if (rend == NULL) {
		printf("ERROR: unable to create renderer: %s\n", SDL_GetError());
		return NULL;
	}
	return rend;
}

ChessWindow runMainWindow (SDL_Window* mainWindow, SDL_Renderer* rend, SDL_Texture* textures[NUM_OF_MAIN_RECTS]) {
	int i;
	SDL_Event e;
	SDL_Rect rects[NUM_OF_MAIN_RECTS];
	
	setMainRects(rects);
	
	// ensure renderer supports transparency
	SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);

	// clear window to be lavender
	SDL_SetRenderDrawColor(rend, 230, 230, 250, 0);
	SDL_RenderClear(rend);

	for (i = 0 ; i < NUM_OF_MAIN_RECTS ; i++) {
		SDL_RenderCopy(rend, textures[i], NULL, &rects[i]);
	}
	// present changes to user
	//SDL_ShowWindow(mainWindow);
	SDL_RenderPresent(rend);


	while (true) {
		SDL_WaitEvent(&e);
		switch (e.type) {
			case SDL_WINDOWEVENT:
				if (e.window.event == SDL_WINDOWEVENT_CLOSE)
					return QUIT_GAME;
			    if (e.window.event == SDL_WINDOWEVENT_HIDDEN)
			    	SDL_ShowWindow(mainWindow);
				break;
			case SDL_QUIT: // pressed X
				return QUIT_GAME;
			case SDL_KEYDOWN: // pressed esc
				if (e.key.keysym.sym == SDLK_ESCAPE)
					return QUIT_GAME;
				break;
			case SDL_MOUSEBUTTONUP:
				if (isInRect(e, rects[4])) // pressed quit
					return QUIT_GAME;
				if (isInRect(e, rects[2])) // pressed new game
					return SETTINGS_WINDOW;
				if (isInRect(e, rects[3])) // pressed load game
					return LOAD_WINDOW;
				break;
			default:
				break;
		}
	}	
	return QUIT_GAME;
}

void setMainRects(SDL_Rect rects[NUM_OF_MAIN_RECTS]) {
	int i, xPos = (WIDTH-BUTTON_WIDTH)/2.0, headlineWidth = WIDTH/3.0, headlineXPos = (WIDTH-headlineWidth)/2.0;
	rects[0].x = 0;
	rects[0].y = 0;
	rects[0].w = WIDTH;
	rects[0].h = HIGHT;	
	
	rects[1].x = headlineXPos;
	rects[1].y = HIGHT/10.0;
	rects[1].w = headlineWidth;
	rects[1].h = HIGHT/6.0;	
	
	for (i = 2 ; i < 5 ; i++) {
		rects[i].x = xPos;
		rects[i].w = BUTTON_WIDTH;
		rects[i].h = BUTTON_HIGHT;
	}
	
	rects[2].y = HIGHT/3.0;
	rects[3].y = rects[2].y + rects[2].h + BUTTON_HIGHT/2.0;
	rects[4].y = rects[3].y + rects[3].h + BUTTON_HIGHT/2.0;
}

bool setMainTextures(SDL_Window* mainWindow, SDL_Renderer* rend, SDL_Texture* textures[NUM_OF_MAIN_RECTS]) {
	if ((textures[0] = getTex(mainWindow, rend, BACKGROUND_PATH)) == NULL)
		return false;
	if ((textures[1] = getTex(mainWindow, rend, HEADLINE_PATH)) == NULL)
		return false;
	if ((textures[2] = getTex(mainWindow, rend, GAME_BUTTON_PATH)) == NULL)
		return false;
	if ((textures[3] = getTex(mainWindow, rend, LOAD_BUTTON_PATH)) == NULL)
		return false;
	if ((textures[4] = getTex(mainWindow, rend, QUIT_BUTTON_PATH)) == NULL)
		return false;	
	return true;
}
/*
ChessWindow destroyMainWindow(SDL_Window* mainWindow, SDL_Renderer* rend, SDL_Texture* textures[NUM_OF_MAIN_RECTS]) {
	int i;
	for (i = 0 ; i < NUM_OF_MAIN_RECTS ; i++) {
		SDL_DestroyTexture(textures[i]);
	}
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(mainWindow);
	return QUIT_GAME;
}*/

SDL_Texture* getTex(SDL_Window* mainWindow, SDL_Renderer* rend, char* buttonPic) {
	// load an image onto an SDL surface
	SDL_Surface* surf = SDL_LoadBMP(buttonPic);
	if (surf == NULL) {
		printf("ERROR: unable to load image: %s\n", SDL_GetError());
		return NULL;
	}
	// set a specific color (green: 0,255,0) to transparent
	SDL_SetColorKey(surf, SDL_TRUE, SDL_MapRGB(surf->format, 0, 255, 0));

	// create a texture from the surface image
	SDL_Texture* tex = SDL_CreateTextureFromSurface(rend, surf);
	if (tex == NULL) {
		printf("ERROR: unable to create texture: %s\n", SDL_GetError());
		return NULL;
	}
	// surface not needed anymore - free it
	SDL_FreeSurface(surf);
	SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
	return tex;
}

bool isInRect(SDL_Event e, SDL_Rect rect) {
	if ((e.button.x > rect.x) && (e.button.y > rect.y) && (e.button.x < rect.x+rect.w) && (e.button.y < rect.y+ rect.h))
		return true;
	return false;
}

void destroyTextures (SDL_Texture** textures, int numOfTextures) {
	int i;
	for (i = 0 ; i < numOfTextures ; i++) {
		SDL_DestroyTexture(textures[i]);
	}
}

void runHelpMainWindow () {
	SDL_Window* mainWindow;
	SDL_Renderer* rend;
	if ((mainWindow = createWindow()) == NULL)
		return;
	if ((rend = createRenderer(mainWindow)) == NULL)
		return;
	SDL_Texture* mainTex[NUM_OF_MAIN_RECTS];
	if (setMainTextures(mainWindow, rend, mainTex) == false)
		return;
	runMainWindow(mainWindow, rend, mainTex);
	destroyTextures(mainTex, NUM_OF_MAIN_RECTS);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(mainWindow);
	SDL_Quit();

}
/*
int main(int argc, char *argv[]) {
	runHelpMainWindow();
	SDL_Quit();
	return 0;
}*/

