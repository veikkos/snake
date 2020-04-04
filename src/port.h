#ifndef _PORT_H_
#define _PORT_H_

#include "defines.h"
#include "color.h"
#include "snake.h"
#include <stddef.h>

namespace Port
{
  PortHandle Init();

  void Deinit(PortHandle handle);

  namespace Persistent
  {
    int GetHighScore(PortHandle handle);

    void SetHighScore(PortHandle handle, int score);
  }

  namespace Time
  {
    void FrameLimit(PortHandle handle);

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

    Image LoadImage(PortHandle handle, const char* path);

    void FreeImage(Image image);
  }

  namespace Render
  {
    void Blit(PortHandle handle, int x, int y, Image source, Rect* clip = NULL);

    void Text(PortHandle handle, int x, int y, Font font, const char* text, Color* color, bool center = false);

    void Clear(PortHandle handle);

    void Draw(PortHandle handle);
  }
}

#endif
