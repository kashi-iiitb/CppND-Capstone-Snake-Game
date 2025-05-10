# Modified Snake Game

This is a starter repo for the Capstone project in the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213). The code for this repo was inspired by [this](https://codereview.stackexchange.com/questions/212296/snake-game-in-c-with-sdl) excellent StackOverflow post and set of responses.

<img src="Modified_Snake_Game.gif"/>

Snake initially starts with length 1 and higher speed and gradually speed keeps decreasing as the snake length increases. This helps to increase the user confidence to play the game and player doesn't feel frustrated/anxious. Instead, at the end of the game the player feels an emotion of accomplishment.
To make the game a bit more challenging I have introduced a RED food, if snake eats it, snake speed will increase. User needs to avoid the RED food.

## Added Game features:
1. Introduced Difficulty levels (Easy/Normal/Hard).
   Easy: No poison, Normal: Poison, Hard: Poison + Wall
3. Multiple food items for Snake to eat: Eating Yellow food increases the length and reduces the Snake speed.
4. Poison (Red food): Eating Red food will kill the snake. User needs to avoid the Red food.
5. If snake hits the wall, it will die.
6. Keeps track of the highest score and UserName to the file (/src/highest.txt) and is displayed at the end of the game.

# Addressed rubric points
### Loop, Functions, I/O:
| Criteria                                                                                       | Explanation and link(s)                                                       |
|------------------------------------------------------------------------------------------------|-------------------------------------------------------------------------------|
| The project demonstrates an understanding of C++ functions and control structures.             | The whole project is organized with proper use of functions and flow of data. |
| The project reads data from a file and process the data, or the program writes data to a file. | Reads the user name at the console and from the input filestream. https://github.com/kashi-iiitb/CppND-Capstone-Snake-Game/blob/2c23394d9f020a1943c878c92177829db38812c2/src/main.cpp#L26 |
| The project accepts user input and processes the input.                                        | stores the user name and score to the file if score is highest https://github.com/kashi-iiitb/CppND-Capstone-Snake-Game/blob/2c23394d9f020a1943c878c92177829db38812c2/src/main.cpp#L45 |

### Object Oriented Programming:
1. Class constructors utilize member initialization lists: Renderer, Game and Snake constructors use initialization lists. For example Snake constructor: https://github.com/kashi-iiitb/CppND-Capstone-Snake-Game/blob/2c23394d9f020a1943c878c92177829db38812c2/src/snake.h#L11
2. Classes abstract implementation details from their interfaces: The operations/methods which can be applied on Object of the class are declared with public access specifier and operations/methods used internal to the object are declared with private access specifiers in all the classes - Snake, Game, Renderer and Controller. 
3. Classes are organized with attributes to hold data and methods to perform tasks.
All class data members are explicitly specified as public, protected, or private.
Member data that is subject to an invariant is hidden from the user and accessed via member methods.
For example 'std::vector<SDL_Point> foods' and the function which operates on it 'void PlaceSpeedInc(SDL_Point &speed_inc);' is added as private member. https://github.com/kashi-iiitb/CppND-Capstone-Snake-Game/blob/2c23394d9f020a1943c878c92177829db38812c2/src/game.h#L21

### Memory Management:
1. The project makes use of references in function declarations. Snake object is passed by reference in https://github.com/kashi-iiitb/CppND-Capstone-Snake-Game/blob/2c23394d9f020a1943c878c92177829db38812c2/src/controller.cpp#L12
2. The project uses destructors appropriately. Renderer destructor destroys sdl_window.
3. The project uses scope / Resource Acquisition Is Initialization (RAII) where appropriate.

## Dependencies for Running Locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* SDL2 >= 2.0
  * All installation instructions can be found [here](https://wiki.libsdl.org/Installation)
  >Note that for Linux, an `apt` or `apt-get` installation is preferred to building from source. 
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./SnakeGame`.


## CC Attribution-ShareAlike 4.0 International


Shield: [![CC BY-SA 4.0][cc-by-sa-shield]][cc-by-sa]

This work is licensed under a
[Creative Commons Attribution-ShareAlike 4.0 International License][cc-by-sa].

[![CC BY-SA 4.0][cc-by-sa-image]][cc-by-sa]

[cc-by-sa]: http://creativecommons.org/licenses/by-sa/4.0/
[cc-by-sa-image]: https://licensebuttons.net/l/by-sa/4.0/88x31.png
[cc-by-sa-shield]: https://img.shields.io/badge/License-CC%20BY--SA%204.0-lightgrey.svg
