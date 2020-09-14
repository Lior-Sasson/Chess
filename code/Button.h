#ifndef BUTTON_H_
#define BUTTON_H_

#include "Widget.h"
#include <SDL.h>
#include <SDL_video.h>

typedef struct button_t Button;
struct button_t {
	SDL_Renderer* render;
	SDL_Texture* texture;
	SDL_Rect location;
	void (*action)(void);
};

Widget* createButton(
		SDL_Renderer* renderer,
		const char* image,
		SDL_Rect location,
		void (*action)(void));
void destroyButton(Widget* src);
void handleButtonEvent(Widget* src, SDL_Event* event);
void drawButton(Widget*, SDL_Renderer*);

#endif /* BUTTON_H_ */
