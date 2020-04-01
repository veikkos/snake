#ifndef _DEFINES_H_
#define _DEFINES_H_

#include <stdint.h>

#if GBA_BUILD
#define SCREEN_WIDTH    240
#define SCREEN_HEIGHT   160
#else
#define SCREEN_WIDTH    640
#define SCREEN_HEIGHT   480
#endif

#define FPS             20

#define X_AREA          SCREEN_WIDTH
#define Y_AREA          SCREEN_HEIGHT

#if GBA_BUILD
#define GRID_SIZE       8
#define MAX_LENGTH      128
#else
#define GRID_SIZE       10
#define MAX_LENGTH      50000
#endif

#define SNAKE_PAUSE     0

#define E_DYN_FRAME_AGE FPS
#define E_DYN_FRAMES    5

struct HandleImpl;
typedef HandleImpl* Handle;

struct FontImpl;
typedef FontImpl* Font;

struct ImageImpl;
typedef ImageImpl* Image;

typedef enum {
  GAME_NONE,
  GAME_QUIT
} GameSelection;

typedef enum {
  MENU_NONE,
  MENU_QUIT,
  MENU_SINGLE,
  MENU_AI,
  MENU_SMART_AI
} MenuSelection;

typedef enum {
  AI_NONE,
  AI_QUIT,
  AI_FRAMELIMIT,
  AI_PATH
} AiSelection;

struct Rect {
  int x, y;
  int w, h;
};

#endif
