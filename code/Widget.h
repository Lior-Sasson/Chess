#ifndef WIDGET_H_
#define WIDGET_H_

#include <SDL.h>
#include <SDL_video.h>

typedef struct widget_t Widget;
struct widget_t {
	void (*draw)(Widget*, SDL_Renderer*);
	void (*handleEvent)(Widget*, SDL_Event*);
	void (*destroy)(Widget*);
	void* data;
};

// useful function for NULL-safe destroy
void destroyWidget(Widget* src);

#endif /* WIDGET_H_ */
