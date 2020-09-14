#include "Widget.h"

void destroyWidget(Widget* src)
{
	if (src != NULL)
		src->destroy(src);
}
