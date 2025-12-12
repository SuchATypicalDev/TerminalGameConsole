#include "Breakoutgame.hpp"
#include "generalutils.hpp"
#include "mazerunner.hpp"
#include "pong.hpp"
#include "snake.hpp"
#include <iostream>
int main(int argc, char *argv[]) {
  Clear_Terminal();
  int resx;
  int resy;
  get_terminal_size(resx, resy);
  bool running = true;
  while (running) {
    int game_id = 0;
    std::cout << "What game do you wanna play?\n1-maze "
                 "runner\t2-pong\t3-breakout\t4-snake\t0-quit\n";
    std::cin >> game_id;
    switch (game_id) {
    case 1:
      Clear_Terminal();
      init_mazerunner();
      Clear_Terminal();
      std::cout << "\033[38;2;0;255;0mYou just played maze runner, game by "
                   "SuchATypicalDev, i truly wish you had fun!!\033[m\n";
      break;
    case 2:
      init_pong();
      Clear_Terminal();
      std::cout << "\033[38;2;0;255;0mYou just played pong, game by "
                   "BASEL891, i truly wish you had fun!!\033[m\n";
      break;
    case 3:
      init_breakout();
      Clear_Terminal();
      std::cout << "\033[38;2;0;255;0mYou just played breakout, game by "
                   "hossamhosam890, i truly wish you had fun!!\033[m\n";
      break;
    case 4:
      init_snake();
      Clear_Terminal();
      std::cout << "\033[38;2;0;255;0mYou just played snake, game by "
                   "0mar-Adel, i truly wish you had fun!!\033[m\n";
      break;
    case 0:
      Clear_Terminal();
      std::cout << "\033[38;2;0;0;255mShutting down...\033[m\n";
      running = false;
      break;
    default:
      system("clear");
      std::cout << "\033[38;2;255;0;0minvalid game, please try again\033[m\n";
    }
  }
  return 0;
}
