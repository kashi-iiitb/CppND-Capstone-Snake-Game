#include "game.h"
#include <iostream>
//#include "SDL.h"
#include <SDL2/SDL.h>
#include <mutex>
#include <thread>
#include <condition_variable>

Game::Game(std::size_t grid_width, std::size_t grid_height, int dLevel)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)),
      diff_level(dLevel) {
  for (int i = 0; i < food_size; ++i) {
    SDL_Point food;
    PlaceFood(food);
    foods.emplace_back(food);
  }
  
  wall.clear();

  if(diff_level == 1){
    snake.speed = 0.3f;
  } else if(diff_level>=2) { 
    snake.speed = 0.35f;
    PlacePoison(poison);
    if(diff_level == 3){
      PlaceWall(wall);
    }
  }
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;
  bool gamePause = false;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake, gamePause);
    //Pause the game if 'Escape' key is pressed
    //if(gamePause == false)
      Update(running, gamePause);
    renderer.Render(snake, foods, poison, wall, gamePause, *this, bonus_food);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
  

}

void Game::PlaceFood(SDL_Point &food) {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y)) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

bool Game::PoisonCell(int x, int y){
  if(poison.x == x && poison.y == y)
    return true;
  else
    return false;
}
bool Game::FoodCell(int x, int y){
  for (auto const &item : Game::wall) {
    if (x == item.x && y == item.y) {
      return true;
    }    
  }
  return false;
}

void Game::PlaceWall(std::vector<SDL_Point> &wall) {
  SDL_Point point1;
  SDL_Point point2;
  SDL_Point point3;
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake/Poison/Food item before placing
    // food.
    if (!snake.SnakeCell(x, y) && !Game::PoisonCell(x, y)){ // && !Game::FoodCell(x, y)) {
      wall.clear();
      point2.x = x;
      point2.y = y;
      point1.x = x-1;
      point1.y = y;
      point3.x = x+1;
      point3.y = y;
      wall.push_back(point1);
      wall.push_back(point2);
      wall.push_back(point3);
      return;
    }
  }
  return;
}

void Game::PlacePoison(SDL_Point &poison) {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing poison.
    bool is_Occupied = false;
    if (snake.SnakeCell(x, y)) {
      is_Occupied = true;
    }
    if (is_Occupied) {
      continue;
    }
    poison.x = x;
    poison.y = y;
    return;
  }
}

void Game::BonusFoodTimer()
{
  const int bonusSeconds = 15;
  std::condition_variable condition_var;
  auto startTime = std::chrono::high_resolution_clock::now();
  std::unique_lock<std::mutex> lock(mutex);
  while (is_bonus_food_active)
  {
    lock.unlock();
    auto current_Time = std::chrono::high_resolution_clock::now();
    auto elapsed_Seconds = std::chrono::duration_cast<std::chrono::seconds>(current_Time - startTime).count();
    if (elapsed_Seconds >= bonusSeconds)
    {
      mutex.lock();
      // Bonus food time is up
      is_bonus_food_active = false;
      already_appeared = true;
      mutex.unlock();
      bonus_food.x = -1;
      bonus_food.y = -1;
      break;
    }
    lock.lock();
    // Wait for a short interval or until the condition_variable is notified
    condition_var.wait_for(lock, std::chrono::milliseconds(800));
  }
}

void Game::PlaceBonusFood(){
  int x, y;  
  x = random_w(engine);
  y = random_h(engine);

  //place bonus food whenever snake length grows by factor of 8 (8/16/24/etc.)
  if ((score != 0) && (score%8 == 0)){
    mutex.lock();
    if(already_appeared) {    
      already_appeared = false;
    }
    mutex.unlock();
  }
  
  //is_bonus_food_active and already_appeared are out shared data/critical section
  //protect them with mutex
  mutex.lock();
    if (!is_bonus_food_active && !already_appeared)
    { // Check if bonus food is already active
      bonus_food.x = x;
      bonus_food.y = y;
      is_bonus_food_active = true;
      mutex.unlock(); //unlock the mutex inside if(){}
      std::thread bonusFoodThread = std::thread(&Game::BonusFoodTimer, this);
      bonusFoodThread.detach();
    } else {
      //unlock the mutex inside else{} part
      mutex.unlock();
    }  
}

void Game::Update(bool &running, bool gamePause) {
  if(gamePause){
    return;
  }
  int dummy;
  if (!snake.alive) {
    
    //std::string msgText{"Score: " + std::to_string(game.GetScore()) + "\n Size: " + std::to_string(game.GetSize())};
    //SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "GAME OVER!", msgText.c_str(), NULL); 
    running = false;
    return;
  }
  
  snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if there's food over here
  for(auto &food: foods){
    if (food.x == new_x && food.y == new_y) {
      score++;
      PlaceFood(food);
      PlaceBonusFood();
      // Grow snake
      snake.GrowBody();
      //reduce the snake speed as length increases.
      //Lower threshold speed 0.12f to keep it a bit challenging
      if(snake.speed > 0.15f)
        snake.speed -= 0.01;
      //change the position of poison on snake eating food
      //PlacePoison(poison);
    } 
  }
  //check for bonus_food
  if(bonus_food.x == new_x && bonus_food.y == new_y){
    if(snake.speed > 0.15f)
      snake.speed -= 0.02;
      bonus_food.x = -1;
      bonus_food.y = -1;
  }
  //Check for poisons
  if (poison.x == new_x && poison.y == new_y) {    
    snake.alive = false;
    //PlacePoison(poison);
    //snake.speed += 0.02;
    return;
  }
  //check for wall
  if(!Game::wall.empty()){
    for(auto &brick: Game::wall){
      if(brick.x == new_x && brick.y == new_y){
        snake.alive = false;
        return;
      }
    }
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }