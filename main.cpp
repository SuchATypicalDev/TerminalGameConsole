#include "generalutils.hpp"
#include "mazerunner.hpp" 
#include <iostream>
int main(int argc, char *argv[]) {
	Clear_Terminal();
  int resx;
  int resy;
  get_terminal_size(resx, resy);
  bool running = true;
  while (running) {
    int game_id = 0;
    std::cout << "What game do you wanna play?\n1-maze runner\t0-quit\n";
    std::cin >> game_id;
    switch (game_id) {
    case 1:Clear_Terminal();
      init_mazerunner();
      std::cout << "\033[38;2;0;255;0mYou just played maze runner, game by "
                   "Suhaib Magdy, i truly wish you had fun!!\033[m\n";
      break;
    case 0:Clear_Terminal();
      std::cout << "\033[38;2;0;0;255mShutting down...\033[m\n";running = false;
      break;
    default:system("clear");
      std::cout << "\033[38;2;255;0;0minvalid game, please try again\033[m\n";
    }
  }
  return 0;
}
