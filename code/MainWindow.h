#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include "Game.h"
#include "Minimax.h"
#include <SDL.h>
#include <SDL_video.h>

#define WIDTH 1000
#define HIGHT 750
#define ROWS 8
#define COLUMNS 8

#define BUTTON_HIGHT HIGHT/10.0
#define BUTTON_WIDTH WIDTH/5.0
#define HEADLINE_PATH "headline.bmp"
#define BACKGROUND_PATH "chess_background.bmp"
#define GAME_BUTTON_PATH "new_game_button.bmp"
#define LOAD_BUTTON_PATH "load_game_button.bmp"
#define QUIT_BUTTON_PATH "quit_button.bmp"

// background, headline, game button, load button, quit button
#define NUM_OF_MAIN_RECTS 5

/* This enum defines the next window to go to, or quit, or error */
typedef enum chess_windows {
	MAIN_WINDOW,
	SETTINGS_WINDOW,
	GAME_WINDOW,
	SAVE_WINDOW,
	LOAD_WINDOW,
	QUIT_GAME,
	ERROR,
} ChessWindow;

/*
 * This functions creates a window for the chess final project. All the windows are
 * set to the same size.
 * @ret - the SDL_Window pointer or NULL if an error occured.
 */
SDL_Window* createWindow();

/*
 * This function creates a renderer for a given window.
 * @ret - the SDL_Renderer pointer or NULL if an error occured.
 */
SDL_Renderer* createRenderer(SDL_Window* window);

/*
 * This function runs the main window.
 * @param mainWindow - the current window.
 * @param rend - the window's renderer.
 * @param textures - the window's textures.
 * @ret - the next window the user wishes to go to, or quit, or error.
 */
ChessWindow runMainWindow (SDL_Window* mainWindow, SDL_Renderer* rend, SDL_Texture** textures);

/**
 * This function set the window's rectangels' positions and sizes.
 * @param rect - the window's rectangels.
 */
void setMainRects(SDL_Rect rects[NUM_OF_MAIN_RECTS]);

/*
 * This function sets the window's needed textures.
 * @param mainWindow - the current window.
 * @param rend - the window's renderer.
 * @param textures - the window's needed textures.
 */
bool setMainTextures(SDL_Window* mainWindow, SDL_Renderer* rend, SDL_Texture* textures[NUM_OF_MAIN_RECTS]);

/*
 * This function creates a texture for a given renderer.
 * @param Window - the current window.
 * @param rend - the window's renderer.
 * @param buttonPic - the image's path on which we want to create the texture for.
 * @ret - an instance of SDL_Texture or NULL if an error occured.
 */
SDL_Texture* getTex(SDL_Window* Window, SDL_Renderer* rend, char* buttonPic);

/*
 * Checks if the mouse clicked in the rectangle's territory.
 * @param e - the current event.
 * @param rect - the given rectangle.
 * @ret - true if the mouse clicked in the rect territory, false otherwise.
 */
bool isInRect(SDL_Event e, SDL_Rect rect);

/*
 * This function frees all memory allocated for the given textures.
 * @param textures - the given textures.
 * @paran numOfTextures - the number of textures to be destroyed.
 */
void destroyTextures (SDL_Texture** textures, int numOfTextures);

#endif /* MAINWINDOW_H_ */
