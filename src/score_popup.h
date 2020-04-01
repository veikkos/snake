#ifndef _SCORE_POPUP_H_
#define _SCORE_POPUP_H_

#include "popup.h"
#include "defines.h"
#include "port.h"

class ScorePopup : public Popup
{
public:
  ScorePopup(Handle handle, int x, int y, int score, Font font);

  virtual ~ScorePopup();

  virtual bool Update() override;

  virtual void Render() override;

private:
  Handle handle;
  int x;
  int y;
  int score;
  int tick;
  int opacity;
  Font font;
};

#endif
