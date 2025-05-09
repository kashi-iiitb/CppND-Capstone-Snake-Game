#include "renderer.h"
#include <iostream>
#include <string>
#include <SDL2/SDL_ttf.h>
//#include "game.h"
class Game;

Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height)
    : screen_width(screen_width),
      screen_height(screen_height),
      grid_width(grid_width),
      grid_height(grid_height) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
  //Initialize SDL_ttf
  if( TTF_Init() == -1 )
  {
      //printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
      //success = false;
      std::cerr << "TTF_Init could not initialize.\n";
  }

  // Create Window
  sdl_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);

  if (nullptr == sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
}

Renderer::~Renderer() {
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();
}

void Renderer::Render(Snake const snake, std::vector<SDL_Point> const &foods,
                      SDL_Point const & poison, std::vector<SDL_Point> &wall,
                      bool gamePause, Game &game) {
  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);

  if(gamePause){
    RenderPauseScreen();
    //return;
  }

  // Render food
  for(auto &food: foods){
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);
    block.x = food.x * block.w;
    block.y = food.y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
  }
  // Render poison
  if(poison.x != -1) {
    SDL_SetRenderDrawColor(sdl_renderer, 0x8F, 0x00, 0xFF, 0xFF);
    block.x = poison.x * block.w;
    block.y = poison.y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
  }
  // Render wall's body
  if(!wall.empty()) {
    SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0xFF, 0xFF, 0xFF);
    for (SDL_Point const &point : wall) {
      block.x = point.x * block.w;
      block.y = point.y * block.h;
      SDL_RenderFillRect(sdl_renderer, &block);
    }
  }
  // Render snake's body
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  for (SDL_Point const &point : snake.body) {
    block.x = point.x * block.w;
    block.y = point.y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
  }

  // Render snake's head
  block.x = static_cast<int>(snake.head_x) * block.w;
  block.y = static_cast<int>(snake.head_y) * block.h;
  if (snake.alive) {
    SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);
  } else {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
  }
  SDL_RenderFillRect(sdl_renderer, &block);

  // Update Screen
  SDL_RenderPresent(sdl_renderer);

  /*if (!snake.alive) {
    std::string msgText{"Score: " + std::to_string(game.GetScore()) + "\n Size: " + std::to_string(game.GetSize())};
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "GAME OVER!", msgText.c_str(), NULL); 
  }*/
}

void Renderer::UpdateWindowTitle(int score, int fps) {
  std::string title{"Snake Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}

void Renderer::RenderPauseScreen()
{
    TTF_Font *font = TTF_OpenFont("../res/roboto.ttf", 24);
    if(nullptr == font){
      std::cout << "Error in font\n";
    }
    SDL_Color white = {255, 255, 255};
    SDL_Surface *textSurface =
        TTF_RenderText_Solid(font, "PAUSE", white);
    if(nullptr == textSurface){
      std::cout << "Error in textSurface\n";
    }
    SDL_Texture *text = SDL_CreateTextureFromSurface(sdl_renderer, textSurface);
    if(nullptr == font){
      std::cout << "Error in text\n";
    }
    SDL_FreeSurface(textSurface);
    SDL_Rect renderQuad = {
        20, static_cast<int>(screen_height) - 30, textSurface->w, textSurface->h};
    SDL_RenderCopy(sdl_renderer, text, NULL, &renderQuad);
    SDL_DestroyTexture(text);
}
