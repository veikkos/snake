#ifndef _PORT_H_
#define _PORT_H_

#include "defines.h"
#include "color.h"
#include "snake.h"
#include <stddef.h>

namespace Port
{
  Handle Init();

  void Deinit(Handle handle);

  namespace Persistent
  {
    int GetHighScore(Handle handle);

    void SetHighScore(Handle handle, int score);
  }

  namespace Time
  {
    void FrameLimit(Handle handle);

    void Delay(unsigned int delay);
  }

  namespace Input
  {
    GameSelection Game(Snake *snake);

    MenuSelection Menu();

    AiSelection Ai();
  }

  namespace Resources
  {
    Font LoadFont(const char* path, unsigned int size);

    void FreeFont(Font font);

    Image LoadImage(Handle handle, const char* path);

    void FreeImage(Image image);
  }

  namespace Render
  {
    void Blit(Handle handle, int x, int y, Image source, Rect* clip = NULL);

    void Text(Handle handle, int x, int y, Font font, const char* text, Color* color, bool center = false);

    void Clear(Handle handle);

    void Draw(Handle handle);
  }
}

#endif
