#ifndef GAME_H
#define GAME_H

#include <random>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include <mutex>
class Renderer;

class Game {
 public:

  Game(std::size_t grid_width, std::size_t grid_height, int dLevel);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;

 private:
  Snake snake;
  int food_size{2};
  std::vector<SDL_Point> foods;
  SDL_Point poison{poison.x = -1, poison.y = -1};
  std::vector<SDL_Point> wall;
  
  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  SDL_Point bonus_food{bonus_food.x = -1, poison.y = -1};
  bool is_bonus_food_active = false;
  bool already_appeared = true;
  std::mutex mutex;


  int score{0};
  int diff_level{0};

  void PlaceFood(SDL_Point &food);
  void Update(bool &running, bool gamePause);
  void PlacePoison(SDL_Point &poison);
  void PlaceWall(std::vector<SDL_Point> &wall);  
  bool PoisonCell(int x, int y);
  bool FoodCell(int x, int y);
  void BonusFoodTimer();
  void PlaceBonusFood();
};

#endif