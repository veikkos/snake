#include "menu.h"
#include "port.h"

Menu::Menu() {

  background = NULL;
  font = NULL;
}

Menu::~Menu() {

  if (background != NULL) {
    Port::Resources::FreeImage(background);
  }

  if (font != NULL) {
    Port::Resources::FreeFont(font);
  }
}

bool Menu::Init(Handle handle) {

  // Open the font
  font = Port::Resources::LoadFont("fonts/DigitalDream.ttf", 28);

  // If there was an error in loading the font
  if (font == NULL) {
    return false;
  }

  background = Port::Resources::LoadImage(handle, "img/m_bg.png");

  if (background == NULL) {
    return false;
  }

  return true;
}

MenuSelection Menu::Main(Handle handle) {

  MenuSelection i_selection = MenuSelection::MENU_NONE;
  const int x_pos = SCREEN_WIDTH / 6;
  int y_pos = SCREEN_HEIGHT / 8;
  const int y_increment = SCREEN_HEIGHT / 12;

  Color textColor = { 230, 230, 230 };

  // Draw background to the screen
  Port::Render::Blit(handle, 0, 0, background);

  PrintText(handle, x_pos, y_pos, (char *)"Select 1 to play", font, &textColor);
  y_pos += y_increment;
  PrintText(handle, x_pos, y_pos, (char *)"Select 2 to AI play", font,
    &textColor);
  y_pos += y_increment;
  PrintText(handle, x_pos, y_pos, (char *)"Select 3 to AI2 play", font,
    &textColor);
  y_pos += y_increment;
  PrintText(handle, x_pos, y_pos, (char *)"Select ESC to quit", font,
    &textColor);

  Port::Render::Draw(handle);

  while (i_selection == MenuSelection::MENU_NONE) {
    Port::Time::Delay(10);
    i_selection = GetInput();
  }

  Port::Render::Clear(handle);

  return i_selection;
}

int Menu::PrintText(Handle handle, int x, int y, char *text,
  Font font, Color *color) {
  Port::Render::Text(handle, x, y, font, text, *color);
  return true;
}

MenuSelection Menu::GetInput() {
  return Port::Input::Menu();
}
