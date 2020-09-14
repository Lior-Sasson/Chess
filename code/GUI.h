#ifndef GUI_H_
#define GUI_H_

// later include the last including gui module
#include "MemoryWindow.h"

// main, settings, game, load
#define NUM_OF_WINDOWS 4
#define CHESS_ICON_PATH "chess_icon.bmp"

/*
 * This function uses all other GUI modules to run the program in graphical mode.
 * @ret - true if no error occured, false otherwise.
 */
bool runGame();

/*
 * This function destroys all windows, renders and textures created.
 * @param windows - an array of all the windows.
 * @param renders - an array of all the renderers.
 * @param mainTex - an array of textures that belong to MainWindow
 * @param gameTex - an array of textures that belong to GameWindow
 * @param setTex - an array of textures that belong to SettingsWindow
 * @param memTex - an array of textures that belong to MemoryWindow
 * @param boardTex - an array of textures that belong to making the game board
 */
void destroyGUI(SDL_Window** windows, SDL_Renderer** renders, SDL_Texture** mainTex, SDL_Texture** gameTex, SDL_Texture** setTex, SDL_Texture** memTex, SDL_Texture** boardTex);

/*
 * This function destroy windows and their renderers.
 * @param windows - an array of all the windows.
 * @param renders - an array of all the renderers.
 */
void destroyWindows(SDL_Window** windows, SDL_Renderer** renders);

/*
 * This function destroys the first num windows.
 * @param windows - an array of all the windows.
 * @param num - the number of the windows to destroy.
 */
void destroyJustWindows(SDL_Window** windows, int num);

/*
 * This function creates all windows.
 * @param windows - an array of all the windows
 * @ret - true if no error occured, false otherwise.
 */
bool createWindows(SDL_Window* windows[NUM_OF_WINDOWS]);

/*
 * This function creates all renderers.
 * @param windows - an array of all the renderers
 * @ret - true if no error occured, false otherwise.
 */
bool createRenderers(SDL_Window* windows[NUM_OF_WINDOWS], SDL_Renderer* renders[NUM_OF_WINDOWS]);

#endif /* GUI_H_ */
