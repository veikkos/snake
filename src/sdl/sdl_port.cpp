#include "port.h"
#include "defines.h"

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#include <stdlib.h>
#include <time.h>
#include <cstring>

#define SCREEN_FLAGS    (SDL_WINDOW_OPENGL)

using namespace Port;

struct HandleImpl
{
  SDL_Window *sdlWindow;
  SDL_Renderer *sdlRenderer;
  SDL_Surface *screenSurface;
  unsigned int framestarttime;
} handle;

Handle Port::Init() {
  memset(&handle, 0, sizeof(handle));

  srand((int)time(NULL));

  // Initialize all SDL subsystems
  if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
    return NULL;
  }

  // Set up the screen
  if (!(handle.sdlWindow = SDL_CreateWindow(
    "Snake v0.5",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    SCREEN_FLAGS))) {
    return NULL;
  }

  if (!(handle.sdlRenderer = SDL_CreateRenderer(handle.sdlWindow, -1,
    SDL_RENDERER_ACCELERATED))) {
    return NULL;
  }

  // Initialize SDL_ttf
  if (TTF_Init() == -1) {
    return NULL;
  }

  handle.screenSurface = SDL_GetWindowSurface(handle.sdlWindow);

  if (!handle.screenSurface) {
    return NULL;

  }
  return (Handle)&handle;
}

void Port::Deinit(Handle handle)
{
  HandleImpl* handleImpl = (HandleImpl*)handle;
  if (handleImpl->sdlRenderer) {
    SDL_DestroyRenderer(handleImpl->sdlRenderer);
  }

  if (handleImpl->sdlWindow) {
    SDL_DestroyWindow(handleImpl->sdlWindow);
  }

  TTF_Quit();
  SDL_Quit();
}

void Time::FrameLimit(Handle handle)
{
  HandleImpl* handleImpl = (HandleImpl*)handle;

  unsigned int waittime = (int)(1000.0f / FPS);
  int delaytime;

  delaytime = waittime - (SDL_GetTicks() - handleImpl->framestarttime);

  if (delaytime > 0) {
    SDL_Delay((Uint32)delaytime);
  }

  handleImpl->framestarttime = SDL_GetTicks();
}

void Time::Delay(unsigned int delay)
{
  SDL_Delay(delay);
}

GameSelection Input::Game(Snake *snake) {
  bool dirGot = false;
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      return GAME_QUIT;
    }

    // Accept only one direction change when
    // GetInput is called so we won't crash
    // to itself in one move.
    if (dirGot) {
      continue;
    }

    //If a key was pressed
    if (event.type == SDL_KEYDOWN) {

      //Set the proper message surface
      switch (event.key.keysym.sym) {
      case SDLK_UP:
        snake->SetDir(s_up);
        dirGot = 1;
        break;

      case SDLK_DOWN:
        snake->SetDir(s_down);
        dirGot = 1;
        break;

      case SDLK_LEFT:
        snake->SetDir(s_left);
        dirGot = 1;
        break;

      case SDLK_RIGHT:
        snake->SetDir(s_right);
        dirGot = 1;
        break;

      case SDLK_ESCAPE:
        return GAME_QUIT;

      default:
        break;
      }
    }
  }

  return GAME_NONE;
}

MenuSelection Input::Menu()
{
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      return MenuSelection::MENU_QUIT;

      // If a key was pressed
    }
    else if (event.type == SDL_KEYDOWN) {

      switch (event.key.keysym.sym) {
      case SDLK_1:
        return MenuSelection::MENU_SINGLE;
      case SDLK_2:
        return MenuSelection::MENU_AI;
      case SDLK_3:
        return MenuSelection::MENU_SMART_AI;
      case SDLK_ESCAPE:
        return MenuSelection::MENU_QUIT;
      default:
        break;
      }
    }
  }

  return MenuSelection::MENU_NONE;
}

AiSelection Input::Ai()
{
  SDL_Event event;

  while (SDL_PollEvent(&event)) {

    if (event.type == SDL_QUIT) {
      return AiSelection::AI_QUIT;
    }
    else if (event.type == SDL_KEYDOWN) {

      switch (event.key.keysym.sym) {
      case SDLK_r:
        return AiSelection::AI_FRAMELIMIT;
      case SDLK_d:
        return AiSelection::AI_PATH;
      case SDLK_ESCAPE:
        return AiSelection::AI_QUIT;
      default:
        break;
      }
    }
  }

  return AiSelection::AI_NONE;
}

void Render::Draw(Handle handle)
{
  HandleImpl* handleImpl = (HandleImpl*)handle;
  SDL_UpdateWindowSurface(handleImpl->sdlWindow);
}

Font Resources::LoadFont(const char* path, unsigned int size)
{
  return (Font)TTF_OpenFont(path, size);
}

void Resources::FreeFont(Font font)
{
  TTF_CloseFont((TTF_Font*)font);
}

void Render::Text(Handle handle, int x, int y, Font font, const char* text, Color color, bool center)
{
  SDL_Color sdlColor = {
    color.r,
    color.g,
    color.b,
    255
  };

  SDL_Surface *textSurface = TTF_RenderText_Solid((TTF_Font*)font, text, sdlColor);

  if (textSurface) {
    if (center) {
      x -= textSurface->w / 2;
      y -= textSurface->h / 2;
    }

    Blit(handle, x, y, (Image)textSurface);
    Resources::FreeImage((Image)textSurface);
  }
}

Image Resources::LoadImage(Handle handle, const char* filename)
{
  HandleImpl* handleImpl = (HandleImpl*)handle;
  SDL_Surface *optimizedImage = NULL;
  SDL_Surface *loadedImage = NULL;

  loadedImage = IMG_Load(filename);

  if (loadedImage) {
    optimizedImage = SDL_ConvertSurfaceFormat(loadedImage, SDL_PIXELFORMAT_RGBA8888, 0);

    SDL_FreeSurface(loadedImage);

    return (Image)optimizedImage;
  }
  else {
    return NULL;
  }
}

void Resources::FreeImage(Image image)
{
  SDL_FreeSurface((SDL_Surface*)image);
}

void Render::Blit(Handle handle, int x, int y, Image source, Rect* clip)
{
  HandleImpl* handleImpl = (HandleImpl*)handle;

  SDL_Rect offset;

  offset.x = x;
  offset.y = y;

  SDL_Rect sdlClip;

  if (clip) {
    sdlClip = {
      clip->x,
      clip->y,
      clip->w,
      clip->h
    };
  }

  // Blit
  SDL_BlitSurface((SDL_Surface*)source, clip ? &sdlClip : NULL, (SDL_Surface*)handleImpl->screenSurface, &offset);
}
