#include "menu.h"
#include "port.h"
#include <stdio.h>

#if GBA_BUILD
#define BUTTON_1 "A"
#define BUTTON_2 "L"
#define BUTTON_3 "R"
#else
#define BUTTON_1 "1"
#define BUTTON_2 "2"
#define BUTTON_3 "3"
#endif

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
  int y_pos = SCREEN_HEIGHT / 10;
  const int y_increment = SCREEN_HEIGHT / 12;
  char score_array[20];

  Color textColor = { 230, 230, 230 };

  // Draw background to the screen
  Port::Render::Blit(handle, 0, 0, background);

  snprintf(score_array, sizeof(score_array), "High score: %d", Port::Persistent::GetHighScore(handle));

  PrintText(handle, x_pos, y_pos, score_array, font, &textColor);
  y_pos += y_increment * 2;
  PrintText(handle, x_pos, y_pos, (char *)"Select " BUTTON_1 " to play", font, &textColor);
  y_pos += y_increment;
  PrintText(handle, x_pos, y_pos, (char *)"Select " BUTTON_2 " to AI play", font,
    &textColor);
  y_pos += y_increment;
  PrintText(handle, x_pos, y_pos, (char *)"Select " BUTTON_3 " to AI2 play", font,
    &textColor);

#ifndef GBA_BUILD
  y_pos += y_increment;
  PrintText(handle, x_pos, y_pos, (char *)"Select ESC to quit", font,
    &textColor);
#endif

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
  Port::Render::Text(handle, x, y, font, text, color);
  return true;
}

MenuSelection Menu::GetInput() {
  return Port::Input::Menu();
}
