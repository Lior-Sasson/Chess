#ifndef SETTINGSWINDOW_H_
#define SETTINGSWINDOW_H_

#include "GameWindow.h"

#define SETTINGS_HEADLINE "settings_headline.bmp"
#define GAME_MODE_PATH "set_game_mode.bmp"
#define PLAYER1_PATH "1-player1.bmp"
#define PLAYER2_PATH "2-player1.bmp"
#define LEVEL_PATH "set_difficulty_level.bmp"
#define AMATEUR_PATH "amateur1.bmp"
#define EASY_PATH "easy1.bmp"
#define MODERATE_PATH "moderate1.bmp"
#define HARD_PATH "hard1.bmp"
#define EXPERT_PATH "expert1.bmp"
#define COLOR_PATH "set_color.bmp"
#define BLACK_PATH "black1.bmp"
#define WHITE_PATH "white1.bmp"

#define PLAYER1_PATH2 "1-player2.bmp"
#define PLAYER2_PATH2 "2-player2.bmp"
#define AMATEUR_PATH2 "amateur2.bmp"
#define EASY_PATH2 "easy2.bmp"
#define MODERATE_PATH2 "moderate2.bmp"
#define HARD_PATH2 "hard2.bmp"
#define EXPERT_PATH2 "expert2.bmp"
#define BLACK_PATH2 "black2.bmp"
#define WHITE_PATH2 "white2.bmp"

#define SET_BACK "mem_back.bmp"
#define SET_NEXT "set_next.bmp"
#define SET_START "set_start.bmp"

#define NUM_OF_SET_TEXTURES 25
#define NUM_OF_SET_RECTS 14



ChessWindow runSettingsWindow(Game *game, SDL_Window* window, SDL_Renderer* rend, SDL_Texture** textures);

void showStep1(SDL_Renderer* rend, Game *game, SDL_Texture **textures, SDL_Rect *rects, bool next);

void showStep2(SDL_Renderer* rend, Game *game, SDL_Texture **textures, SDL_Rect *rects);

void showStep3(SDL_Renderer* rend, Game *game, SDL_Texture **textures, SDL_Rect *rects);

bool setSettingsTextures(SDL_Window* window, SDL_Renderer* rend, SDL_Texture** textures);

void setSettingsRects(SDL_Rect *rects);



#endif /* SETTINGSWINDOW_H_ */
