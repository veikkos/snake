#include "game.h"
#include "menu.h"
#include "port.h"

int main(int argc, char *args[]) {
  Game *game = NULL;
  Menu *menu = NULL;

  MenuSelection sel;
  bool end;
  PortHandle handle = Port::Init();

  if (!handle) {
    return 0;
  }

  menu = new Menu;

  if (menu->Init(handle) == false) {
    return 0;
  }

  end = false;

  while (end != true) {

    sel = menu->Main(handle);

    if (sel != MenuSelection::MENU_QUIT) {

      game = new Game;

      if (game->Init(handle) == false) {
        return 0;
      }

      switch (sel) {

      case MenuSelection::MENU_SINGLE:
        if (game->Execute(handle, Game::SINGLE) == false) {
          return 0;
        }

        break;

      case MenuSelection::MENU_AI:
        if (game->Execute(handle, Game::AI) == false) {
          return 0;
        }

        break;

      case MenuSelection::MENU_SMART_AI:
        if (game->Execute(handle, Game::SMART_AI) == false) {
          return 0;
        }

        break;

      default:
        break;
      }

      delete game;
      game = NULL;

    }
    else {
      end = true;
      break;
    }
  }

  if (game) {
    delete game;
  }

  if (menu) {
    delete menu;
  }

  Port::Deinit(handle);

  return 1;
}
