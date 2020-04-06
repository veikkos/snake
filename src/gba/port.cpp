#include "port.h"
#include "defines.h"
#include "SuperCrateBox-GBA/gba.h"
#include "SuperCrateBox-GBA/font.h"

#include "sprites.h"
#include "g_bg.h"
#include "m_bg.h"

#include <cstring>

using namespace Port;

#define MEM_SRAM ((uint8_t*)0xE000000)
#define MEM_SRAM_HIGH_SCORE MEM_SRAM
#define MEM_SRAM_HIGH_SCORE_VERSION (MEM_SRAM_HIGH_SCORE + 3)

enum Images {
  SNAKE = 1,
  EATABLE = 3,
  DYN_EATABLE = 4,
  PATH = 11,
  DYN_EATABLE_2 = 12,
  M_BG,
  G_BG
};

struct PortImpl
{
  int active;
  Images bg;
  uint16_t framestarttime;
};

static const uint8_t highScoreVersion = 1;

const unsigned short fontPal[1] __attribute__((aligned(4)))=
{
  0x7BDE
};

inline uint16_t GetTicks()
{
  return REG_TM0D;
}

inline uint16_t GetTimerFrequency()
{
  return 16384;
}

PortHandle Port::Init()
{
  PortImpl* handle = new PortImpl;

  // Set display options.
  REG_DISPCNT = DCNT_MODE0 | DCNT_OBJ | DCNT_BG0;

  // Set background 0 options.
  REG_BG0CNT = BG_CBB(0) | BG_SBB(30) | BG_8BPP | BG_REG_32x32;
  REG_BG0HOFS = 0;
  REG_BG0VOFS = 0;

  LoadTileData(4, 0, spritesTiles, spritesTilesLen);
  LoadPaletteObjData(0, spritesPal, spritesPalLen);

  LoadTileData(4, 128, font_bold, sizeof(font_bold));
  LoadPaletteObjData(64, fontPal, sizeof(fontPal));

  handle->active = 0;
  Render::Clear(handle);

  // Enable timer with 16.384 kHz frequency
  REG_TM0CNT = (1 << 7) | 3;
  REG_TM0D = 0;

  handle->framestarttime = GetTicks();

  return (PortHandle)handle;
}

void Port::Deinit(PortHandle handle)
{
  PortImpl* handleImpl = (PortImpl*)handle;
  delete handleImpl;
}

int Persistent::GetHighScore(PortHandle handle)
{
  int score = 0;
  for(size_t i = 0; i < sizeof(score) - 1; ++i)
    score += MEM_SRAM_HIGH_SCORE[i] << (i * 8);

  const uint8_t storedVersion = *MEM_SRAM_HIGH_SCORE_VERSION;
  return score != -1 && storedVersion == highScoreVersion ? score : 0;
}

void Persistent::SetHighScore(PortHandle handle, int score)
{
  for(size_t i = 0; i < sizeof(score) - 1; ++i)
    MEM_SRAM_HIGH_SCORE[i] = (score >> (i * 8)) & 0xFF;

  *MEM_SRAM_HIGH_SCORE_VERSION = highScoreVersion;
}

void Time::FrameLimit(PortHandle handle)
{
  PortImpl* handleImpl = (PortImpl*)handle;

  const uint16_t waittime = GetTimerFrequency() / FPS;
  const uint16_t endtime = handleImpl->framestarttime + waittime;
  const uint16_t normalizing = 0 - handleImpl->framestarttime;
  const uint16_t endtime_normalized = endtime + normalizing;

  while((uint16_t)(GetTicks() + normalizing) < endtime_normalized)
    ;

  handleImpl->framestarttime = GetTicks();
}

void Time::Delay(unsigned int delay)
{
  const uint16_t endtime = GetTicks() + (GetTimerFrequency() * delay / 1000);

  while(GetTicks() != endtime)
    ;
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

void Render::Draw(PortHandle handle)
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

void Render::Text(PortHandle handle, int x, int y, Font font, const char* text, const Color* color, bool center)
{
  PortImpl* handleImpl = (PortImpl*)handle;
  const size_t len = strlen(text);

  if(center) {
    x -= (len * 8) / 2;
    y -= 8 / 2;
  }

  if (y >= 0) {
    for(int i = 0; i < len; i++) {
      const int x_pos = x+(i*8);
      if (x_pos >= 0) {
        SetObject(--(handleImpl->active),
                  ATTR0_SHAPE(0) | ATTR0_8BPP | ATTR0_REG | ATTR0_Y(y),
                  ATTR1_SIZE(0) | ATTR1_X(x_pos),
                  ATTR2_ID8(text[i] + 128));
      }
    }
  }
}

void Render::Clear(PortHandle handle)
{
  PortImpl* handleImpl = (PortImpl*)handle;
  for(int i = handleImpl->active; i < NUM_OBJECTS; i++){
    SetObject(i, ATTR0_HIDE, 0, 0);
  }
  handleImpl->active = NUM_OBJECTS;
}

Image Resources::LoadImage(PortHandle handle, const char* filename)
{
  if(!strcmp(filename, "img/snake.png"))
    return (Image)Images::SNAKE;
  else if(!strcmp(filename, "img/eatable.png"))
    return (Image)Images::EATABLE;
  else if(!strcmp(filename, "img/dyn_eatable.png"))
    return (Image)Images::DYN_EATABLE;
  else if(!strcmp(filename, "img/dyn_eatable_2.png"))
    return (Image)Images::DYN_EATABLE_2;
  else if(!strcmp(filename, "img/path.png"))
    return (Image)Images::PATH;
  else if(!strcmp(filename, "img/g_bg.png"))
    return (Image)Images::G_BG;
  else if(!strcmp(filename, "img/m_bg.png"))
    return (Image)Images::M_BG;

  return (Image)100;
}

void Resources::FreeImage(Image image)
{
}

void SetBackground(const unsigned short* tiles, size_t tilesLen, const unsigned short* pal, size_t palLen)
{
  LoadTileData(0, 0, tiles, tilesLen);
  LoadPaletteBGData(0, pal, palLen);

  int i = 0;
  for (int y = 0; y < 160 / 8; ++y){
    for (int x = 0; x < 240 / 8; ++x){
      SetTile(30, x, y, i++);
    }
  }
}

void Render::Blit(PortHandle handle, int x, int y, Image source, Rect* clip)
{
  PortImpl* handleImpl = (PortImpl*)handle;

  if ((int)source < Images::M_BG) {
    const int id = clip ? clip->x / GRID_SIZE : 0;
    const int offset = (int)source - 1;

    SetObject(--(handleImpl->active),
              ATTR0_SHAPE(0) | ATTR0_8BPP | ATTR0_REG | ATTR0_Y(y),
              ATTR1_SIZE(0) | ATTR1_X(x),
              ATTR2_ID8(id + offset));
  } else {
    switch((int)source) {
    case Images::M_BG:
      if (handleImpl->bg != Images::M_BG) {
        SetBackground(m_bgTiles, m_bgTilesLen, m_bgPal, m_bgPalLen);
        handleImpl->bg = Images::M_BG;
      }
      break;
    case Images::G_BG:
      if (handleImpl->bg != Images::G_BG) {
        SetBackground(g_bgTiles, g_bgTilesLen, g_bgPal, g_bgPalLen);
        handleImpl->bg = Images::G_BG;
      }
      break;
    default:
      break;
    }
  }
}
