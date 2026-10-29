#include "types.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <X11/Xlib.h>

bool quited = false;

void on_delete(Display * display, Window window)
{
    XDestroyWindow(display, window);
    quited = true;
}


extern int main(int argc, char *argv[]) {
    Display * display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Failed to initialize display.\n");
        return EXIT_FAILURE;
    }

    Window parent = DefaultRootWindow(display);
    if (parent == None) {
        fprintf(stderr, "No root window found.\n");
        XCloseDisplay(display);
        return EXIT_FAILURE;
    }

    const Int32 x = 0;
    const Int32 y = 0;
    const UInt32 width = 800;
    const UInt32 height = 600;
    const UInt32 border_width = 0;
    const UInt64 border_color = 0x00000000;
    const UInt64 background_color = 0xffffffff;

    Window window = XCreateSimpleWindow(
        display, parent,
        x, y, width, height,
        border_width, border_color,
        background_color
    );
    if (window == None) {
        fprintf(stderr, "Failed to create window.\n");
        XCloseDisplay(display);
        return EXIT_FAILURE;
    }
    XSelectInput(display, window, KeyPressMask | KeyReleaseMask);

    XMapWindow(display, window);
    
    Atom wm_delete_window = XInternAtom(display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(display, window, &wm_delete_window, 1);

    while (!quited) {
        XEvent event;
        XNextEvent(display, &event);

       if (event.type == KeyRelease && XEventsQueued(display, QueuedAfterReading)){
            XEvent nev;
            XPeekEvent(display, &nev);
                
            if (nev.type == KeyPress && nev.xkey.time == event.xkey.time &&
                nev.xkey.keycode == event.xkey.keycode)
            {
                continue;
            }
        }
        
        switch(event.type) {
          case ClientMessage: {
            if(event.xclient.data.l[0] == wm_delete_window) {
                on_delete(event.xclient.display, event.xclient.window);
            }
          } break;

          case KeyPress: {
            unsigned int keycode = event.xkey.keycode;
            KeySym keysym = XLookupKeysym(&event.xkey, 0);
            char *key_name = XKeysymToString(keysym);
            printf("Pressed: Keycode=%d, Name=%s\n", keycode, key_name ? key_name : "Unknown");
          } break;
          
          case KeyRelease: {
            unsigned int keycode = event.xkey.keycode;
            KeySym keysym = XLookupKeysym(&event.xkey, 0);
            char *key_name = XKeysymToString(keysym);
            printf("Released: Keycode=%d, Name=%s\n", keycode, key_name ? key_name : "Unknown");
          } break;
        }
    }

    XCloseDisplay(display);
    printf("Done.\n");
    return 0;
}