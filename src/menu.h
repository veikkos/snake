#ifndef _MENU_H_
#define _MENU_H_

#include "defines.h"
#include "port.h"

class Menu {
public:

  Menu();
  ~Menu();

  bool Init(PortHandle handle);

  MenuSelection Main(PortHandle handle);

private:

  MenuSelection GetInput();
  int PrintText(PortHandle handle, int x, int y, char *text,
    Font font, Color *color);

  Image background;

  Font font;
};

#endif
