#include "port.h"
#include "defines.h"
#include "SuperCrateBox-GBA/gba.h"
#include "SuperCrateBox-GBA/font.h"

#include <cstring>

using namespace Port;

struct HandleImpl
{
  int activeLetters;
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

Handle Port::Init()
{
  // Set display options.
  REG_DISPCNT = DCNT_MODE0 | DCNT_OBJ | DCNT_BG0;

  // Set background 0 options.
  REG_BG0CNT = BG_CBB(0) | BG_SBB(30) | BG_8BPP | BG_REG_32x32;
  REG_BG0HOFS = 0;
  REG_BG0VOFS = 0;

  LoadPaletteBGData(0, bg_palette, sizeof(bg_palette));
  LoadTileData(4, 64, font_bold, 8192);

  //Clear backgrounds
  for (int y = 0; y < 32; ++y){
    for (int x = 0; x < 32; ++x){
      SetTile(30, x, y, 0);
      SetTile(29, x, y, 0);
      SetTile(28, x, y, 0);
      SetTile(27, x, y, 0);
    }
  }

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
  if (delay < 17)
    delay = 17;
  for (int i = 0; i < delay / 17; ++i)
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
  int startPos = handleImpl->activeLetters;
  if(center)
    x -= 28;
  for(int i = 0; i < strlen(text); i++) {
    handleImpl->activeLetters++;
    SetObject(startPos+i+45,
              ATTR0_SHAPE(0) | ATTR0_8BPP | ATTR0_REG | ATTR0_Y(y),
              ATTR1_SIZE(0) | ATTR1_X(x+(i*8)),
              ATTR2_ID8(text[i] + 64));
  }
}

void Render::Clear(Handle handle)
{
  HandleImpl* handleImpl = (HandleImpl*)handle;
  for(int i = 0; i < handleImpl->activeLetters; i++){
    SetObject(i+45, ATTR0_HIDE, 0, 0);
  }
  handleImpl->activeLetters = 0;
}

Image Resources::LoadImage(Handle handle, const char* filename)
{
  if(!strcmp(filename, "img/snake.png"))
    return (Image)1;
  else if(!strcmp(filename, "img/eatable.png"))
    return (Image)2;
  else if(!strcmp(filename, "img/dyn_eatable.png"))
    return (Image)3;
  else if(!strcmp(filename, "img/path.png"))
    return (Image)4;

  return (Image)100;
}

void Resources::FreeImage(Image image)
{
}

void Render::Blit(Handle handle, int x, int y, Image source, Rect* clip)
{
  char data[] = " ";
  switch((int)source)
    {
    case 1:
      data[0] += 'O';
      break;
    case 2:
      data[0] += 'X';
      break;
    case 3:
      data[0] += 'Z';
      break;
    case 4:
      data[0] += 'S';
      break;
    default:
      return;
    }

  Color dummyColor = { 230, 230, 230 };
  Render::Text(handle, x, y, 0, data, dummyColor);
}
