#include "game.h"
#include "color.h"
#include "score_popup.h"
#include <cstring>
#include <stdio.h>
#include <vector>

using namespace std;

Game::Game() {
  background = NULL;
  snake_t = NULL;
  eatable = NULL;
  dyn_eatable = NULL;
  dyn_eatable_2 = NULL;

  font = NULL;
  font_s = NULL;

  snake = NULL;
  s_eatable = NULL;
  d_eatable = NULL;
  path_mark = NULL;

  score = 0;
  done = 0;

  framelimit = true;
  render_path = true;
  since_last_eat = 0;

  ai = NULL;
  smartai = NULL;
}

Game::~Game() {
  // Delete content here

  if (snake) {
    delete snake;
  }

  if (s_eatable) {
    delete s_eatable;
  }

  if (d_eatable) {
    delete d_eatable;
  }

  if (background != NULL) {
    Port::Resources::FreeImage(background);
  }

  if (snake_t != NULL) {
    Port::Resources::FreeImage(snake_t);
  }

  if (eatable != NULL) {
    Port::Resources::FreeImage(eatable);
  }

  if (dyn_eatable != NULL) {
    Port::Resources::FreeImage(dyn_eatable);
  }

  if (dyn_eatable_2 != NULL) {
    Port::Resources::FreeImage(dyn_eatable_2);
  }

  if (path_mark != NULL) {
    Port::Resources::FreeImage(path_mark);
  }

  if (font != NULL) {
    Port::Resources::FreeFont(font);
  }

  if (font_s != NULL) {
    Port::Resources::FreeFont(font_s);
  }
}

bool Game::Init(PortHandle handle) {

  if (LoadContent(handle) == false) {
    return false;
  }

  snake = new Snake(GRID_SIZE * 3, GRID_SIZE * 3, 3);

  s_eatable = new Eatable(e_static, NULL);

  return true;
}

bool Game::Execute(PortHandle handle, game_mode mode) {
  int end = 0;

  switch (mode) {
  case SMART_AI:
    smartai = new SmartAi;
    break;

  case AI:
    ai = new Ai;
    break;

  default:
    break;
  }


  while (!done) {
    if (!snake->IsPaused()) {
      switch (mode) {

      case SINGLE:
        GetInput();
        break;

      case SMART_AI:
        GetAi(smartai);
        break;

      case AI:
        GetAi(ai);
        break;
      }
    }

    end = Update(handle);
    Render(handle, end);

    if (framelimit) {
      Port::Time::FrameLimit(handle);
    }
  }

  if (mode == SINGLE) {
    int highScore = Port::Persistent::GetHighScore(handle);
    if (score > highScore) {
      Port::Persistent::SetHighScore(handle, score);
    }
  }

  if (done == DIED) {
    Port::Time::Delay(2000);
  }

  if (ai) {
    delete ai;
    ai = NULL;
  }

  if (smartai) {
    delete smartai;
    smartai = NULL;
  }

  return true;
}

// Private

int Game::LoadContent(PortHandle handle) {
  font = Port::Resources::LoadFont("fonts/DigitalDreamFat.ttf", 24);

  if (font == NULL) {
    return false;
  }

  font_s = Port::Resources::LoadFont("fonts/DigitalDreamFat.ttf", 16);

  if (font_s == NULL) {
    return false;
  }

  background = Port::Resources::LoadImage(handle, "img/g_bg.png");

  if (background == NULL) {
    return false;
  }

  snake_t = Port::Resources::LoadImage(handle, "img/snake.png");

  if (snake_t == NULL) {
    return false;
  }

  eatable = Port::Resources::LoadImage(handle, "img/eatable.png");

  if (eatable == NULL) {
    return false;
  }

  dyn_eatable = Port::Resources::LoadImage(handle, "img/dyn_eatable.png");

  if (dyn_eatable == NULL) {
    return false;
  }

  dyn_eatable_2 = Port::Resources::LoadImage(handle, "img/dyn_eatable_2.png");

  if (dyn_eatable_2 == NULL) {
    return false;
  }

  path_mark = Port::Resources::LoadImage(handle, "img/path.png");

  if (path_mark == NULL) {
    return false;
  }

  return true;
}

void Game::GetInput() {
  if (Port::Input::Game(snake) == GameSelection::GAME_QUIT) {
    done = QUITED;
  }
}

template <class TYPE>
void Game::GetAi(TYPE *ai) {
  // Chase dynamic eatable if
  // it exists, and if not,
  // go for the basic eatable.
  if (d_eatable != NULL) {
    ai->GetDir(snake, d_eatable);
  }
  else {
    ai->GetDir(snake, s_eatable);
  }

  // Poll keys if user want's to quit
  AiSelection selection = Port::Input::Ai();
  switch (selection)
  {
  case AI_QUIT:
    done = QUITED;
    break;
  case AI_FRAMELIMIT:
    framelimit = !framelimit;
    break;
  case AI_PATH:
    render_path = !render_path;
    break;
  default:
    break;
  }
}

int Game::Update(PortHandle handle) {
  snake->Move();

  // Check that if snake has contact with itself
  for (int i = 1; i < snake->GetLength(); i++) {

    if ((snake->GetPosX(0) == snake->GetPosX(i)) && \
      (snake->GetPosY(0) == snake->GetPosY(i))) {
      done = DIED;
      return 1;
    }
  }

  const bool ate = UpdateEatables(handle);

  InactivityBlocker(handle, ate);

  UpdatePopups();

  return 0;
}

bool Game::UpdateEatables(PortHandle handle) {
  bool ate = false;

  // Check if snake has collided with the eatable
  if ((snake->GetPosX(0) == s_eatable->GetPosX(0)) && \
    (snake->GetPosY(0) == s_eatable->GetPosY(0))) {

    int i;
    vector<position> used;

    for (i = 0; i < snake->GetLength(); i++) {
      used.push_back(snake->GetPos(i));
    }

    if (d_eatable) {
      used.push_back(d_eatable->GetPos(0));
    }

    score++;

    snake->Grow(1);

    ScorePopup* popup = new ScorePopup(handle, snake->GetPosX(0), snake->GetPosY(0), 1, font_s);
    popups.push_back(popup);

    // Delete eaten "eatable" and
    // create a new one.
    delete s_eatable;
    s_eatable = new Eatable(e_static, &used);

    ate = true;
  }

  if (d_eatable == NULL) {

    int i;
    vector<position> used;

    for (i = 0; i < snake->GetLength(); i++) {
      used.push_back(snake->GetPos(i));
    }

    if (s_eatable) {
      used.push_back(s_eatable->GetPos(0));
    }

    if ((rand() % 1000) < 5) {
      const eatable_type type = (eatable_type)(e_dynamic + (rand() % 2));
      d_eatable = new Eatable(type, &used);
    }
  }
  else {

    // Check if snake has collided with the dynamic eatable
    if ((snake->GetPosX(0) == d_eatable->GetPosX(0)) && \
      (snake->GetPosY(0) == d_eatable->GetPosY(0))) {
      int scoreIncrease;

      if (d_eatable->GetType() == eatable_type::e_dynamic_shrink) {
        scoreIncrease = 2;
      }
      else {
        scoreIncrease = d_eatable->GetFrame() + 1;
        snake->Grow(1);
      }

      score += scoreIncrease;

      ScorePopup* popup = new ScorePopup(handle, snake->GetPosX(0), snake->GetPosY(0),
        scoreIncrease, font_s);
      popups.push_back(popup);

      delete d_eatable;
      d_eatable = NULL;

      ate = true;
    }

    // Update dynamic eatable status
    // and check if it's still alive
    if (d_eatable != NULL) {
      if (d_eatable->Update() == false) {

        delete d_eatable;
        d_eatable = NULL;
      }
    }
  }

  return ate;
}

void Game::UpdatePopups() {
  for (std::vector<Popup*>::iterator it = popups.begin(); it != popups.end();) {
    if ((*it)->Update()) {
      ++it;
    }
    else {
      delete *it;
      it = popups.erase(it);
    }
  }
}

void Game::Render(PortHandle handle, int end) {

  // Draw background to the screen
  Port::Render::Blit(handle, 0, 0, background);

  RenderScore(handle);

  RenderSnake(handle);

  if (end) {
    RenderGameOver(handle);

    // Re-apply score, so it will be on top.
    RenderScore(handle);
  }

  RenderEatables(handle);

  if (smartai && render_path && path_mark) {
    RenderPath(handle);
  }

  RenderPopups(handle);

  // Update the screen
  Port::Render::Draw(handle);

  Port::Render::Clear(handle);
}

void Game::RenderSnake(PortHandle handle) {
  Rect clip[2];

  clip[0].x = 0;
  clip[0].y = 0;
  clip[0].w = GRID_SIZE;
  clip[0].h = GRID_SIZE;

  clip[1].x = GRID_SIZE;
  clip[1].y = 0;
  clip[1].w = GRID_SIZE;
  clip[1].h = GRID_SIZE;

  Port::Render::Blit(handle, snake->GetPosX(0), snake->GetPosY(0), snake_t, &clip[0]);

  for (int i = 1; i < snake->GetLength(); i++) {
    Port::Render::Blit(handle, snake->GetPosX(i), snake->GetPosY(i), snake_t, &clip[1]);
  }
}

void Game::RenderEatables(PortHandle handle) {
  Rect clip[5];

  Port::Render::Blit(handle, s_eatable->GetPosX(0), s_eatable->GetPosY(0), eatable);

  if (d_eatable != NULL) {

    clip[0].x = GRID_SIZE * 4;
    clip[0].y = 0;
    clip[0].w = GRID_SIZE;
    clip[0].h = GRID_SIZE;

    clip[1].x = GRID_SIZE * 3;
    clip[1].y = 0;
    clip[1].w = GRID_SIZE;
    clip[1].h = GRID_SIZE;

    clip[2].x = GRID_SIZE * 2;
    clip[2].y = 0;
    clip[2].w = GRID_SIZE;
    clip[2].h = GRID_SIZE;

    clip[3].x = GRID_SIZE;
    clip[3].y = 0;
    clip[3].w = GRID_SIZE;
    clip[3].h = GRID_SIZE;

    clip[4].x = 0;
    clip[4].y = 0;
    clip[4].w = GRID_SIZE;
    clip[4].h = GRID_SIZE;

    const eatable_type type = d_eatable->GetType();
    Port::Render::Blit(handle, d_eatable->GetPosX(0), d_eatable->GetPosY(0),
      type == eatable_type::e_dynamic_shrink ? dyn_eatable_2 : dyn_eatable,
      &clip[d_eatable->GetFrame()]);
  }
}

void Game::RenderPopups(PortHandle handle) {
  for (std::vector<Popup*>::iterator it = popups.begin(); it != popups.end(); ++it) {
    (*it)->Render();
  }
}

void Game::RenderScore(PortHandle handle) {
  const Color textColor = { 230, 230, 230 };
  char score_array[20];

  snprintf(score_array, sizeof(score_array), "SCORE: %d", score);
  Port::Render::Text(handle, 5, 5, font, score_array, &textColor);
}

void Game::RenderPath(PortHandle handle) {
  vector< pair <int, int> > path = smartai->GetPath();

  if (path.size()) {
    for (vector< pair <int, int> >::iterator it = path.begin() + 1; it != path.end();
      ++it) {
      pair <int, int> path_point = *it;

      Port::Render::Blit(handle, path_point.first, path_point.second, path_mark
      );
    }
  }
}

void Game::RenderGameOver(PortHandle handle) {
  const Color textColor = { 230, 230, 230 };
  Port::Render::Text(handle, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, font,
    "GAME OVER!", &textColor, true);
}

void Game::InactivityBlocker(PortHandle handle, bool ate) {
  if (!ate) {
    if (since_last_eat++ > (FPS * SECONDS_BEFORE_PENALTY)) {
      if (score) {
        const int scoreDecrease = -1;

        score += scoreDecrease;

        ScorePopup* popup = new ScorePopup(handle, snake->GetPosX(0), snake->GetPosY(0),
          scoreDecrease, font_s);
        popups.push_back(popup);
      }

      since_last_eat -= FPS;
    }
  }
  else {
    since_last_eat = 0;
  }
}
