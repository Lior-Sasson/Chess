#ifndef MEMORYWINDOW_H_
#define MEMORYWINDOW_H_

#include "SettingsWindow.h"
#include <unistd.h>

#define SAVE_HEADLINE_PATH "save_game_headline.bmp"
#define LOAD_HEADLINE_PATH "load_game_headline.bmp"
#define SAVE_MEM_PATH "save_game_button.bmp"
#define LOAD_MEM_PATH "load_game_button.bmp"
#define EMPTY_SLOT_PATH "empty_slot.bmp"
#define RIGHT_ARROW_PATH "gold_arrow_right.bmp"
#define LEFT_ARROW_PATH "gold_arrow_left.bmp"
#define MEM_BACK_PATH "mem_back.bmp"

#define BOARD_RECT_INDEX 6 //
#define NUM_OF_MEM_TEXTURES 8
#define NUM_OF_BOARD_TEX 15
#define NUM_OF_MEM_RECTS 7
#define NUM_OF_SLOTS 5
#define PATH_LENGTH 11

ChessWindow runMemoryWindow(Game *game, SDL_Window* memoryWindow, SDL_Renderer* rend, SDL_Renderer* gameRend, SDL_Texture** textures, SDL_Texture** boardTex, ChessWindow back, char saveOrLoad, bool *occupied, char slots[NUM_OF_SLOTS][PATH_LENGTH]);

void setSlots(char slots[NUM_OF_SLOTS][11], bool occupied[NUM_OF_SLOTS]);

void setMemoryRects(SDL_Rect *memoryRects);

bool setMemoryTextures (SDL_Window* memoryWindow, SDL_Renderer* rend, SDL_Texture** textures);

void copyGameBoard(Game *game, SDL_Renderer* rend, SDL_Texture** GameTex, SDL_Rect *rects, SDL_Rect tiles[ROWS][COLUMNS]);

void presentMemTextures(SDL_Renderer* rend, SDL_Texture** textures, SDL_Rect *memoryRects, char saveOrLoad, bool right, bool left);

#endif /* MEMORYWINDOW_H_ */
