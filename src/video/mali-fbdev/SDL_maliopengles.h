#ifndef _SDL_maliopengles_h
#define _SDL_maliopengles_h

#if defined(SDL_VIDEO_DRIVER_MALI) && defined(SDL_VIDEO_OPENGL_EGL)

#include "../SDL_egl_c.h"
#include "../SDL_sysvideo.h"

#define MALI_GLES_GetAttribute    SDL_EGL_GetAttribute
#define MALI_GLES_GetProcAddress  SDL_EGL_GetProcAddressInternal
#define MALI_GLES_UnloadLibrary   SDL_EGL_UnloadLibrary
#define MALI_GLES_SetSwapInterval SDL_EGL_SetSwapInterval
#define MALI_GLES_GetSwapInterval SDL_EGL_GetSwapInterval
#define MALI_GLES_DestroyContext  SDL_EGL_DestroyContext

extern bool MALI_GLES_LoadLibrary(SDL_VideoDevice *_this, const char *path);
extern SDL_GLContext MALI_GLES_CreateContext(SDL_VideoDevice *_this, SDL_Window *window);
extern bool MALI_GLES_SwapWindow(SDL_VideoDevice *_this, SDL_Window *window);
extern bool MALI_GLES_MakeCurrent(SDL_VideoDevice *_this, SDL_Window *window, SDL_GLContext context);

#endif /* SDL_VIDEO_DRIVER_MALI && SDL_VIDEO_OPENGL_EGL */

#endif /* _SDL_maliopengles_h */
