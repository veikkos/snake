#ifndef _SDL_PORT_H_
#define _SDL_PORT_H_

#include "defines.h"
#include "snake.h"
#include <stddef.h>

namespace Port
{
  Handle Init();

  void Deinit(Handle handle);

  void FrameLimit(Handle handle);

  void Delay(unsigned int delay);

  GameSelection GetGameInput(Snake *snake);

  MenuSelection GetMenuInput();

  AiSelection GetAiInput();

  void Render(Handle handle);

  Font LoadFont(const char* path, unsigned int size);

  void FreeFont(Font font);

  void RenderText(Handle handle, int x, int y, Font font, const char* text, Color color, bool center = false);

  Image LoadImage(Handle handle, const char* path);

  void FreeImage(Image image);

  void Blit(Handle handle, int x, int y, Image source, Rect* clip = NULL);
}

#endif
