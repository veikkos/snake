#include "game.h"
#include "defines.h"
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

  snake = NULL;
  s_eatable = NULL;
  d_eatable = NULL;
  path_mark = NULL;

  score = 0;
  done = 0;

  framelimit = true;
  render_path = false;

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
}

bool Game::Init(Handle handle) {

  if (LoadContent(handle) == false) {
    return false;
  }

  snake = new Snake(GRID_SIZE * 3, GRID_SIZE * 3, 3);

  s_eatable = new Eatable(e_static, NULL);

  return true;
}

bool Game::Execute(Handle handle, game_mode mode) {
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

    end = Update();
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

int Game::LoadContent(Handle handle) {
  font = Port::Resources::LoadFont("fonts/DigitalDreamFat.ttf", 24);

  if (font == NULL) {
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

int Game::Update() {
  int i;

  snake->Move();

  // Check that if snake has contact with itself
  for (i = 1; i < snake->GetLength(); i++) {

    if ((snake->GetPosX(0) == snake->GetPosX(i)) && \
      (snake->GetPosY(0) == snake->GetPosY(i))) {
      done = DIED;
      return 1;
    }
  }

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

    // Delete eaten "eatable" and
    // create a new one.
    delete s_eatable;
    s_eatable = new Eatable(e_static, &used);
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

      if (d_eatable->GetType() == eatable_type::e_dynamic_shrink) {
        score += 2;
      }
      else {
        score += d_eatable->GetFrame() + 1;
        snake->Grow(1);
      }

      delete d_eatable;
      d_eatable = NULL;
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

  return 0;
}

void Game::Render(Handle handle, int end) {

  int i;
  char score_array[20];
  Color textColor = { 230, 230, 230 };
  Rect clip[5];

  // Draw background to the screen
  Port::Render::Blit(handle, 0, 0, background);

  // Draw score to the screen
  snprintf(score_array, sizeof(score_array), "SCORE: %d", score);
  Port::Render::Text(handle, 5, 5, font, score_array, &textColor);

  // Draw snake to the screen
  clip[0].x = 0;
  clip[0].y = 0;
  clip[0].w = GRID_SIZE;
  clip[0].h = GRID_SIZE;

  clip[1].x = GRID_SIZE;
  clip[1].y = 0;
  clip[1].w = GRID_SIZE;
  clip[1].h = GRID_SIZE;

  Port::Render::Blit(handle, snake->GetPosX(0), snake->GetPosY(0), snake_t, &clip[0]);

  for (i = 1; i < snake->GetLength(); i++) {
    Port::Render::Blit(handle, snake->GetPosX(i), snake->GetPosY(i), snake_t, &clip[1]);
  }

  if (smartai && render_path && path_mark) {
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

  // Draw eatable(s)
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

  if (end) {
    Port::Render::Text(handle, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, font,
      "GAME OVER!", &textColor, true);

    // Re-apply score, so it will be on top.
    Port::Render::Text(handle, 5, 5, font, score_array, &textColor);
  }

  // Update the screen
  Port::Render::Draw(handle);

  Port::Render::Clear(handle);
}
