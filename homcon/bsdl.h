#include <SDL2/SDL.h>

#ifndef rustt

    #include "rust.h"
    
#endif

// Types
#define swin SDL_Window *
#define ssur SDL_Surface *
#define sren SDL_Renderer *
#define stxt SDL_Texture *
#define srec SDL_Rect *
#define evnt SDL_Event

// Consts
#define SDL_WPU SDL_WINDOWPOS_UNDEFINED
#define SDL_WSN SDL_WINDOW_SHOWN

#define KEY_ESC SDLK_ESCAPE
#define KEY_X   SDLK_x
#define KEY_C   SDLK_c
#define KEY_UP  SDLK_UP
#define KEY_LFT SDLK_LEFT
#define KEY_RGT SDLK_RIGHT
#define KEY_DWN SDLK_DOWN

// Functions
#define sdl_init SDL_Init
#define sdl_gerr SDL_GetError
#define img_gerr IMG_GetError
#define sdl_quit SDL_Quit

#define sdl_cwin SDL_CreateWindow
#define sdl_dwin SDL_DestroyWindow

#define sdl_gsur SDL_GetWindowSurface
#define sdl_fsur SDL_FreeSurface
#define sdl_usur SDL_UpdateWindowSurface

#define sdl_dtxt SDL_DestroyTexture

#define sdl_lbmp SDL_LoadBMP
#define sdl_mrgb SDL_MapRGB
#define sdl_poll SDL_PollEvent
#define sdl_blit SDL_BlitSurface
#define sdl_blts SDL_BlitScaled

#define sdl_dpxl SDL_RenderDrawPoint

#define sdl_ctfs SDL_CreateTextureFromSurface
#define sdl_cren SDL_CreateRenderer

#define sdl_rend_copy  SDL_RenderCopy
#define sdl_rend_pres  SDL_RenderPresent
#define sdl_rend_clear SDL_RenderClear
#define sdl_rend_dstry SDL_DestroyRenderer

#define sdl_dcol SDL_SetRenderDrawColor
#define sdl_line SDL_RenderDrawLine

#define sdl_sleep SDL_Delay
#define sdl_rectb SDL_FillRect