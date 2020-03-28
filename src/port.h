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

  int GetInput(Snake *snake);

  MenuSelection GetMenuInput();

  AiSelection GetAiInput();

  void Render(Handle handle);

  Font LoadFont(const char* path, unsigned int size);

  void FreeFont(Font font);

  Image RenderText(Font font, const char* text, Color color);

  Image LoadImage(Handle handle, const char* path);

  void FreeImage(Image image);

  unsigned int GetW(Image image);

  unsigned int GetH(Image image);

  void ApplySurface(Handle handle, int x, int y, Image source, Rect* clip = NULL);
}

#endif
