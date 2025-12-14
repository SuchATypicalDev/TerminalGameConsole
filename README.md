# TerminalGameConsole

A set of games right in the terminal!

# introduction

this project was only possible due to the collaboration of 4 talented individuals, it was a way to explore various 3rd-party libraries for creating simple games.
this project doesnt represent anything significant other than being a learning experience and a way to experience game development first hand.

# dependencies

the project currently only runs on linux based systems and only on some compatible terminal emulators due to the reliance on ANSI escape codes to render 24-bit color on the terminal, but note that all games aside from mazerunner could be compiled separately into their own executables and run on multiple platforms due to SDL2 being cross platform.

below is a list of tested environments and operating systems for each game and for the whole project:

entire project -> runs on wsl on an arch distro, requires SDL2 library to be installed

->tested and runs on an arch linux machine with the kitty terminal emulator.

->fails to run correctly on windows 10 command prompt or powershell, partially works on terminal emulators such as msys2 though results are not that good.

pong -> compiles and runs on all platforms supported by SDL2, requires SDL2.

breakout -> compiles and runs on all platforms supported by SDL2, requires SDL2.

snake -> compiles and runs on all platforms supported by SDL2, requires SDL2.

mazerunner -> tested on an x86_64 arch linux distro on a kitty terminal and on an arch linux distro running on WSL, but theoretically it should work on all linux distros with terminal emulators that properly support ANSI escape codes, it should theoretically compile and run on windows 10 machines with some minor tweakes, doesnt have any dependencies other than a compatiible terminal.

# project structure

the project is divided into multiple header/source pairs, following a modular design where games could be added independant of eachother
main.cpp is the entry point of the program where the main function is, each game has its own header/source pair where the entry poing of each game is a function called init_GAMENAME(), which means that every game could be compiled separately into its own executable by renaming the initialization function to main and compiling the file.

furthermore each game manages its own dependencies which means that no game is dependant on another game.
below is a list of the files responsible for managing every game separately:

for pong: pong.hpp/pong.cpp

for breakout: breakout.hpp/breakout.cpp

for snake: snake.hpp/snake.cpp

for mazerunner:vectors.hpp/vectors.cpp--shapes.hpp/shapes.cpp--generalutils.hpp/generalutils.cpp--mazerunner.hpp/mazerunner.cpp

note that main.cpp is only responsible for initializing games requested by user and has no functionality other than being a start menu.

note that for mazerunner its recommended to compile the project using the -Ofast parameter as the algorithm used for rendering is resource intensive and various manual optimizations were not implemented due to time constraints, such as using fixed point decimals rather than floating point decimals, also note that the algorithm used for rendering is a 2d derivation of the 3d raymarching algorithm which runs on each pixel and is generally intended to run on a dedicated gpu, or at the very least to run on multiple threads, due to time constraints the algorithm is implemented to run on a single thread.
