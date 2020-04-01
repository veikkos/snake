#ifndef _POPUP_H_
#define _POPUP_H_

class Popup
{
public:
  virtual ~Popup();

  virtual bool Update() = 0;

  virtual void Render() = 0;
};

#endif
