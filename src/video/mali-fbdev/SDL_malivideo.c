#include "SDL_internal.h"

#if defined(SDL_VIDEO_DRIVER_MALI)

#include <EGL/egl.h>
#include <EGL/eglplatform.h>
#include <fcntl.h>
#include <stdbool.h>

#include "../SDL_egl_c.h"
#include "../SDL_sysvideo.h"
#include "SDL3/SDL_stdinc.h"
#include "SDL3/SDL_video.h"

#ifdef SDL_INPUT_LINUXEV
#include "../../core/linux/SDL_evdev.h"
#endif

#include "SDL_maliopengles.h"
#include "SDL_malivideo.h"

static SDL_VideoDevice *MALI_Create()
{
    SDL_VideoDevice *device;

    device = (SDL_VideoDevice *)SDL_calloc(1, sizeof(SDL_VideoDevice));
    if (!device) {
        SDL_OutOfMemory();
        return NULL;
    }

    device->internal = NULL;
    device->num_displays = 0;

    device->free = MALI_Free;

    device->VideoInit = MALI_VideoInit;
    device->VideoQuit = MALI_VideoQuit;

    device->CreateSDLWindow = MALI_CreateWindow;
    device->DestroyWindow = MALI_DestroyWindow;

    device->GL_LoadLibrary = MALI_GLES_LoadLibrary;
    device->GL_GetProcAddress = MALI_GLES_GetProcAddress;
    device->GL_UnloadLibrary = MALI_GLES_UnloadLibrary;
    device->GL_CreateContext = MALI_GLES_CreateContext;
    device->GL_MakeCurrent = MALI_GLES_MakeCurrent;
    device->GL_SetSwapInterval = MALI_GLES_SetSwapInterval;
    device->GL_GetSwapInterval = MALI_GLES_GetSwapInterval;
    device->GL_SwapWindow = MALI_GLES_SwapWindow;
    device->GL_DestroyContext = MALI_GLES_DestroyContext;

    device->PumpEvents = MALI_PumpEvents;
    return device;
}

VideoBootStrap MALI_bootstrap = {
    "mali", "Mali EGL Video Driver",
    MALI_Create,
    NULL,
    false
};

bool MALI_VideoInit(SDL_VideoDevice *_this)
{
    SDL_DisplayData *data = SDL_calloc(1, sizeof(SDL_DisplayData));
    if (data == NULL) {
        return SDL_OutOfMemory();
    }

    int fd = open("/dev/fb0", O_RDWR, 0);
    if (fd < 0) {
        return SDL_SetError("mali-fbdev: Could not open framebuffer device");
    }

    struct fb_var_screeninfo vinfo;
    if (ioctl(fd, FBIOGET_VSCREENINFO, &vinfo) < 0) {
        return SDL_SetError("mali-fbdev: Could not get framebuffer information");
    }

    close(fd);

    SDL_DisplayMode current_mode;
    SDL_zero(current_mode);

    current_mode.w = vinfo.xres;
    current_mode.h = vinfo.yres;
    current_mode.refresh_rate = 60;
    current_mode.format = SDL_PIXELFORMAT_RGBX8888;
    current_mode.internal = NULL;

    SDL_VideoDisplay display;
    SDL_zero(display);

    display.desktop_mode = current_mode;
    display.current_mode = &display.desktop_mode;
    display.internal = data;

    SDL_AddVideoDisplay(&display, true);

#ifdef SDL_INPUT_LINUXEV
    if (!SDL_EVDEV_Init()) {
        return false;
    }
#endif
    return true;
}

void MALI_VideoQuit(SDL_VideoDevice *_this)
{
#ifdef SDL_INPUT_LINUXEV
    SDL_EVDEV_Quit();
#endif
}

bool MALI_CreateWindow(SDL_VideoDevice *_this, SDL_Window *window, SDL_PropertiesID create_props)
{
    SDL_WindowData *windowData = SDL_calloc(1, sizeof(SDL_WindowData));
    if (windowData == NULL) {
        return SDL_OutOfMemory();
    }

    SDL_DisplayID displayID = SDL_GetPrimaryDisplay();
    SDL_DisplayData *displayData = SDL_GetDisplayDriverData(displayID);
    if (displayData == NULL) {
        return false;
    }

    if (!_this->egl_data && !SDL_GL_LoadLibrary(NULL)) {
        return false;
    }

    windowData->egl_surface = SDL_EGL_CreateSurface(_this, window, (NativeWindowType)displayData->native_display);
    if (windowData->egl_surface == EGL_NO_SURFACE) {
        return false;
    }

    window->w = 1024;
    window->h = 768;
    window->flags |= SDL_WINDOW_OPENGL;
    window->internal = windowData;
    return true;
}

void MALI_DestroyWindow(SDL_VideoDevice *_this, SDL_Window *window)
{
    SDL_WindowData *data = window->internal;
    if (data) {
        if (data->egl_surface != EGL_NO_SURFACE) {
            SDL_EGL_DestroySurface(_this, data->egl_surface);
            data->egl_surface = EGL_NO_SURFACE;
        }
        SDL_free(data);
    }
    window->internal = NULL;
}

void MALI_PumpEvents(SDL_VideoDevice *_this)
{
#ifdef SDL_INPUT_LINUXEV
    SDL_EVDEV_Poll();
#endif
}

void MALI_Free(SDL_VideoDevice *_this)
{
    if (_this->internal != NULL) {
        SDL_free(_this->internal);
        _this->internal = NULL;
    }
    SDL_free(_this);
}

#endif /* SDL_VIDEO_DRIVER_MALI */
