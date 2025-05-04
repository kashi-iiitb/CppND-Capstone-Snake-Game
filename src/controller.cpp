#include "controller.h"
#include <iostream>
#include "SDL.h"
#include "snake.h"
#include "game.h"

void Controller::ChangeDirection(Snake &snake, Snake::Direction input,
                                 Snake::Direction opposite) const {
  if (snake.direction != opposite || snake.size == 1) snake.direction = input;
  return;
}

void Controller::HandleInput(bool &running, Snake &snake,
                              bool &gamePause) const {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    switch (e.type){
      case SDL_QUIT:
        running = false;
        break;
      case SDL_KEYDOWN:
        if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE){
          gamePause = true;
          return;
        } else if (e.key.keysym.scancode == SDL_SCANCODE_RETURN){
          gamePause = false;
          return;
        } else {
          switch (e.key.keysym.sym) {
            case SDLK_UP:
              ChangeDirection(snake, Snake::Direction::kUp,
                              Snake::Direction::kDown);
              break;

            case SDLK_DOWN:
              ChangeDirection(snake, Snake::Direction::kDown,
                              Snake::Direction::kUp);
              break;

            case SDLK_LEFT:
              ChangeDirection(snake, Snake::Direction::kLeft,
                              Snake::Direction::kRight);
              break;

            case SDLK_RIGHT:
              ChangeDirection(snake, Snake::Direction::kRight,
                              Snake::Direction::kLeft);
              break;
          }
      }
    }
  }
}