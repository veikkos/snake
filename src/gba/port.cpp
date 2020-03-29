#include "port.h"
#include "defines.h"
#include "SuperCrateBox-GBA/gba.h"
#include "SuperCrateBox-GBA/font.h"

#include "sprites.h"

#include <cstring>

using namespace Port;

struct HandleImpl
{
  int active;
} handle;

const unsigned short bg_palette[256] __attribute__((aligned(4)))=
{
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,

  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,

  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,

  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
};

enum Images {
  SNAKE = 1,
  EATABLE = 3,
  DYN_EATABLE = 4,
  PATH = 11
};

Handle Port::Init()
{
  // Set display options.
  REG_DISPCNT = DCNT_MODE0 | DCNT_OBJ | DCNT_BG0;

  // Set background 0 options.
  REG_BG0CNT = BG_CBB(0) | BG_SBB(30) | BG_8BPP | BG_REG_32x32;
  REG_BG0HOFS = 0;
  REG_BG0VOFS = 0;

  LoadPaletteBGData(0, bg_palette, sizeof(bg_palette));

  LoadTileData(4, 0, spritesTiles, spritesTilesLen);
  LoadPaletteObjData(0, spritesPal, spritesPalLen);

  LoadTileData(4, 128, font_bold, sizeof(font_bold));

  handle.active = NUM_OBJECTS;
  Render::Clear(&handle);

  return (Handle)&handle;
}

void Port::Deinit(Handle handle)
{
}

void Time::FrameLimit(Handle handle)
{
  for (int i = 0; i < 3; ++i)
    WaitVSync();
}

void Time::Delay(unsigned int delay)
{
  int skips = delay / 17;
  if (!skips)
    skips = 1;

  while (skips--)
    WaitVSync();
}

GameSelection Input::Game(Snake *snake)
{
  if(!(REG_KEYINPUT & KEY_UP))
    snake->SetDir(s_up);
  else if(!(REG_KEYINPUT & KEY_DOWN))
    snake->SetDir(s_down);
  else if(!(REG_KEYINPUT & KEY_LEFT))
    snake->SetDir(s_left);
  else if(!(REG_KEYINPUT & KEY_RIGHT))
    snake->SetDir(s_right);
  else if(!(REG_KEYINPUT & KEY_START))
    return GameSelection::GAME_QUIT;

  return GameSelection::GAME_NONE;
}

MenuSelection Input::Menu()
{
  if(!(REG_KEYINPUT & KEY_A))
    return MenuSelection::MENU_SINGLE;
  else if(!(REG_KEYINPUT & KEY_L))
    return MenuSelection::MENU_AI;
  else if(!(REG_KEYINPUT & KEY_R))
    return MenuSelection::MENU_SMART_AI;

  return MenuSelection::MENU_NONE;
}

AiSelection Input::Ai()
{
  if(!(REG_KEYINPUT & KEY_A))
    return AiSelection::AI_FRAMELIMIT;
  else if(!(REG_KEYINPUT & KEY_B))
    return AiSelection::AI_PATH;
  else if(!(REG_KEYINPUT & KEY_START))
    return AiSelection::AI_QUIT;

  return AiSelection::AI_NONE;
}

void Render::Draw(Handle handle)
{
  WaitVSync();
  UpdateObjects();
}

Font Resources::LoadFont(const char* path, unsigned int size)
{
  static int i;
  return (Font)++i;
}

void Resources::FreeFont(Font font)
{
}

void Render::Text(Handle handle, int x, int y, Font font, const char* text, Color color, bool center)
{
  HandleImpl* handleImpl = (HandleImpl*)handle;
  const size_t len = strlen(text);

  if(center) {
    x -= (len * 8) / 2;
    y -= 8 / 2;
  }

  for(int i = 0; i < len; i++) {
    SetObject(handleImpl->active++,
              ATTR0_SHAPE(0) | ATTR0_8BPP | ATTR0_REG | ATTR0_Y(y),
              ATTR1_SIZE(0) | ATTR1_X(x+(i*8)),
              ATTR2_ID8(text[i] + 128));
  }
}

void Render::Clear(Handle handle)
{
  HandleImpl* handleImpl = (HandleImpl*)handle;
  for(int i = 0; i < handleImpl->active; i++){
    SetObject(i, ATTR0_HIDE, 0, 0);
  }
  handleImpl->active = 0;
}

Image Resources::LoadImage(Handle handle, const char* filename)
{
  if(!strcmp(filename, "img/snake.png"))
    return (Image)Images::SNAKE;
  else if(!strcmp(filename, "img/eatable.png"))
    return (Image)Images::EATABLE;
  else if(!strcmp(filename, "img/dyn_eatable.png"))
    return (Image)Images::DYN_EATABLE;
  else if(!strcmp(filename, "img/path.png"))
    return (Image)Images::PATH;

  return (Image)100;
}

void Resources::FreeImage(Image image)
{
}

void Render::Blit(Handle handle, int x, int y, Image source, Rect* clip)
{
  HandleImpl* handleImpl = (HandleImpl*)handle;
  const int id = clip ? clip->x / GRID_SIZE : 0;
  const int offset = (int)source - 1;

  SetObject(handleImpl->active++,
            ATTR0_SHAPE(0) | ATTR0_8BPP | ATTR0_REG | ATTR0_Y(y),
            ATTR1_SIZE(0) | ATTR1_X(x),
            ATTR2_ID8(id + offset));
}
