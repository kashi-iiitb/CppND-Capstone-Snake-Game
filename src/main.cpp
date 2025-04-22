#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"
#include <fstream>
#include <string>

int main() {
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{640};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};
  std::string username;
  std::string filename = "../src/highest.txt";
  std::ifstream inScoreFile(filename);
  std::ofstream outScoreFile;


  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
  Controller controller;
  std::cout <<  "Hi!! Welcome to Snake Game!\n"
                "Please, enter your name to Proceed\n";
  std::cout << "User: " ;
  std::cin >> username;
  //std::cout << "Size: " << game.GetSize() << "\n";
  Game game(kGridWidth, kGridHeight);
  game.Run(controller, renderer, kMsPerFrame);
  std::cout << "Thank you for playing: " << username <<"\n";
  std::cout << "Game has terminated successfully!\n";
  std::cout << "Score: " << game.GetScore() << "\n";
  std::cout << "Size: " << game.GetSize() << "\n";
  if(inScoreFile.is_open()){
    std::string HSUserName, strHSScore;
    int HSScore;
    std::getline(inScoreFile, HSUserName);
    std::getline(inScoreFile, strHSScore);
    HSScore = std::stoi(strHSScore);
    inScoreFile.close();
    if(game.GetScore() > HSScore){
      std::cout << "You have got highest score " << username << "!!\n";
      outScoreFile.open(filename);
      if(outScoreFile.is_open()){
        outScoreFile << username << "\n";
        outScoreFile << std::to_string(game.GetScore());
        outScoreFile.close();
      }
    } else{
      std::cout << "The Highest score for the game is: " << HSScore << " by " << HSUserName << "\n";
    }
  } else {
    std::cout << "Error: Unable to open the Highest score file.\n";
  }
  return 0;
}