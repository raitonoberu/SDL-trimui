#ifndef _SDL_malivideo_h
#define _SDL_malivideo_h

#include "../SDL_egl_c.h"
#include "../SDL_sysvideo.h"

#include "SDL3/SDL_video.h"
#include <EGL/egl.h>

#include <fcntl.h>
#include <linux/fb.h>
#include <linux/vt.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct SDL_DisplayData
{
    EGLNativeWindowType native_display;
} SDL_DisplayData;

typedef struct SDL_WindowData
{
    EGLSurface egl_surface;
} SDL_WindowData;

bool MALI_VideoInit(SDL_VideoDevice *_this);
void MALI_VideoQuit(SDL_VideoDevice *_this);

bool MALI_CreateWindow(SDL_VideoDevice *_this, SDL_Window *window, SDL_PropertiesID create_props);
void MALI_DestroyWindow(SDL_VideoDevice *_this, SDL_Window *window);

void MALI_PumpEvents(SDL_VideoDevice *_this);

void MALI_Free(SDL_VideoDevice *_this);

#endif /* _SDL_malivideo_h */
