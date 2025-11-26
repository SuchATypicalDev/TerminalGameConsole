#include "mazerunner.hpp"
#include <iostream>
#include <vector>
#include <chrono>
#include "generalutils.hpp"
void color_pixel(std::string &buffer, int r, int g, int b) {

  buffer += "\033[38;2;";
  buffer += std::to_string(r);
  buffer += ";";
  buffer += std::to_string(g);
  buffer += ";";
  buffer += std::to_string(b);
  buffer += "m\033[48;2;";
  buffer += std::to_string(r);
  buffer += ";";
  buffer += std::to_string(g);
  buffer += ";";
  buffer += std::to_string(b);
  buffer += "mC\033[m";
}
void init_mazerunner() {
  int width;
  int height;
  std::vector<int> mslist = {};
  std::string screen_buffer = "";
  bool running = true;
  int it = 255;
  while (running) {
    auto t1 = std::chrono::high_resolution_clock::now();
    Clear_Terminal();
    screen_buffer = "";
    int r = 255;
    int g = 255;
    int b = 255;
    if (it > 255)
      it = 0;
    get_terminal_size(width, height);
    for (int h = 0; h < height; h++) {
      for (int w = 0; w < width; w++) {
        r = (int)(((float)w / width) * 255);
        g = (int)(((float)h / height) * 255);
        b = 0;

        color_pixel(screen_buffer, r, g, b);
      }
    }
    std::cout << screen_buffer;
    auto t2 = std::chrono::high_resolution_clock::now();
    int ms = (std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1))
                 .count();
    mslist.push_back(ms);
    float fps = (float)ms;
    int quit;
    std::cin >> quit;
    if (quit > 0) {
      for (int mis : mslist) {
        fps = ((float)(fps + mis)) / 2.0;
      }
      fps = 1000.0 / fps;
      std::cout << "\n" << fps;
      return;
    }
  }
}

