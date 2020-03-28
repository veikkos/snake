#include "menu.h"
#include "port.h"

Menu::Menu() {

  background = NULL;
  font = NULL;
}

Menu::~Menu() {

  if (background != NULL) {
    Port::FreeImage(background);
  }

  if (font != NULL) {
    Port::FreeFont(font);
  }
}

bool Menu::Init(Handle handle) {

  // Open the font
  font = Port::LoadFont("fonts/DigitalDream.ttf", 28);

  // If there was an error in loading the font
  if (font == NULL) {
    return false;
  }

  background = Port::LoadImage(handle, "img/m_bg.png");

  if (background == NULL) {
    return false;
  }

  return true;
}


MenuSelection Menu::Main(Handle handle) {

  MenuSelection i_selection = MenuSelection::MENU_NONE;
  int x_pos = (SCREEN_WIDTH - 440) / 2;

  Color textColor = { 230, 230, 230 };

  // Draw background to the screen
  Port::Blit(handle, 0, 0, background);

  PrintText(handle, x_pos, 60, (char *)"Select 1 to play", font, &textColor);
  PrintText(handle, x_pos, 100, (char *)"Select 2 to AI play", font,
    &textColor);
  PrintText(handle, x_pos, 140, (char *)"Select 3 to AI2 play", font,
    &textColor);
  PrintText(handle, x_pos, 180, (char *)"Select ESC to quit", font,
    &textColor);

  Port::Render(handle);

  while (i_selection == MenuSelection::MENU_NONE) {
    Port::Delay(10);
    i_selection = GetInput();
  }

  return i_selection;
}

int Menu::PrintText(Handle handle, int x, int y, char *text,
  Font font, Color *color) {
  Port::RenderText(handle, x, y, font, text, *color);
  return true;
}

MenuSelection Menu::GetInput() {
  return Port::GetMenuInput();
}
