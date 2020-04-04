#include "score_popup.h"
#include <stdio.h>

ScorePopup::ScorePopup(PortHandle handle, int x, int y, int score, Font font)
  : handle(handle), x(x + GRID_SIZE / 2), y(y - GRID_SIZE),
  tick(0), score(score), opacity(255), font(font)
{
#if !GBA_BUILD
  y -= GRID_SIZE;
#endif
}

ScorePopup::~ScorePopup()
{
}

bool ScorePopup::Update()
{
  if (tick++ > FPS / 3)
  {
    opacity = (int)(opacity * 0.75f);

    if (opacity < 40) {
      return false;
    }
  }

  y -= 1;

  return true;
}

void ScorePopup::Render()
{
  if (x < SCREEN_WIDTH && y < SCREEN_HEIGHT)
  {
    char score_array[3];
    snprintf(score_array, sizeof(score_array), "+%d", score);

    Color color(200, 200, 200, opacity);
    Port::Render::Text(handle, x, y, font, score_array, &color, true);
  }
}
