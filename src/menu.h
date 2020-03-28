#ifndef _MENU_H_
#define _MENU_H_

#include "defines.h"
#include "port.h"

class Menu {
public:

  Menu();
  ~Menu();

  bool Init(Handle handle);

  MenuSelection Main(Handle handle);

private:

  MenuSelection GetInput();
  int PrintText(Handle handle, int x, int y, char *text,
    Font font, Color *color);

  Image background;

  Font font;
};

#endif
