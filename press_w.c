#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>

#define SUCCESS 1
#define FAIL 0
int main(void)
{
	Display *display_thing;
	unsigned int keycode;
	display_thing = XOpenDisplay(NULL);

	keycode = XKeysymToKeycode(display_thing, XK_w);
	XTestFakeKeyEvent(display_thing, keycode, True, 0);
	XTestFakeKeyEvent(display_thing, keycode, False, 0);
	XFlush(display_thing);

	while (SUCCESS){}

	return SUCCESS;
	}