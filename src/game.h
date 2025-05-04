#ifndef GAME_H
#define GAME_H

#include <random>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"

class Game {
 public:

  Game(std::size_t grid_width, std::size_t grid_height, int dLevel);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;
  //bool gamePause;

 private:
  Snake snake;
  int food_size{2};
  std::vector<SDL_Point> foods;
  SDL_Point poison{poison.x = -1, poison.y = -1};
  std::vector<SDL_Point> wall;
  
  //SDL_Point speed_dec;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};
  int diff_level{0};

  void PlaceFood(SDL_Point &food);
  void Update();
  void PlacePoison(SDL_Point &poison);
  void PlaceWall(SDL_Point &wall);  
};

#endif