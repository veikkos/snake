#include "port.h"
#include "defines.h"

#include <cstring>

using namespace Port;

struct HandleImpl
{
} handle;

Handle Port::Init()
{
  return (Handle)&handle;
}

void Port::Deinit(Handle handle)
{
}

void Time::FrameLimit(Handle handle)
{
}

void Time::Delay(unsigned int delay)
{
}

GameSelection Input::Game(Snake *snake)
{
  return GameSelection::GAME_NONE;
}

MenuSelection Input::Menu()
{
  return MenuSelection::MENU_NONE;
}

AiSelection Input::Ai()
{
  return AiSelection::AI_NONE;
}

void Render::Draw(Handle handle)
{
}

Font Resources::LoadFont(const char* path, unsigned int size)
{
  return NULL;
}

void Resources::FreeFont(Font font)
{
}

void Render::Text(Handle handle, int x, int y, Font font, const char* text, Color color, bool center)
{
}

Image Resources::LoadImage(Handle handle, const char* filename)
{
  return NULL;
}

void Resources::FreeImage(Image image)
{
}

void Render::Blit(Handle handle, int x, int y, Image source, Rect* clip)
{
}
