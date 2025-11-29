#include "mazerunner.hpp"
#include "generalutils.hpp"
#include "shapes.hpp"
#include "vectors.hpp"
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <math.h>
#include <vector>
const int sqsidelength = 5;
float initial_dist_to_crystal = 1.0;
int cull_factor = 2;
int number_of_iterations = 0;
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
} /////////////////////////////////////////////////////////////////////////////////////////
int ray_march(SH::vec2 pixel_position, std::vector<SH::square> &sqlist,
              SH::vec2 CENTER_PIXEL) {
  float march_distance = 99999.0;
  while (march_distance > 0.01) {
    for (SH::square square : sqlist) {
      number_of_iterations++;
      float signed_distance = square.get_signed_distance(pixel_position);
      if (signed_distance == 0.0)
        return 0;
      march_distance = std::min(march_distance, signed_distance);
    }
    if (SH::distance(pixel_position, CENTER_PIXEL) < march_distance)
      return 1;
    pixel_position =
        SH::add(pixel_position,
                SH::scalar_multiply(
                    SH::normalize(SH::subtract(CENTER_PIXEL, pixel_position)),
                    march_distance));
  }
  return 2;
} /////////////////////////////////////////////////////////////////////////////////////////
void render_pixel(int x, int y, int width, int height,
                  std::string &screen_buffer, std::vector<SH::square> &sqlist,
                  SH::vec2 &CENTER_PIXEL, SH::vec2 &crystal) {
  // center the "screen" or the "camera" on the center pixel, i.e the player
  // byshifting the current pixel by half the width/height
  SH::vec2 CURRENT_PIXEL((float)x - width / 2, (float)y - height / 2);
  int r, g, b, rayres;
  rayres = ray_march(CURRENT_PIXEL, sqlist, CENTER_PIXEL);
  switch (rayres) {
  case 0:
    r = 0;
    break;
  case 1:
    r = 255 / (std::max((int)SH::distance(CURRENT_PIXEL, CENTER_PIXEL), 1));
    break;
  case 2:
    r = 0;
  }

  float temp =
      (1 - (SH::distance(CENTER_PIXEL, crystal) / initial_dist_to_crystal));
  float crystal_interpolation =
      temp < 0.0 ? crystal_interpolation = 0.0 : crystal_interpolation = temp;
  g = std::max(
      r - ((int)(((float)r) * crystal_interpolation * crystal_interpolation)) -
          1,
      0);
  b = g;
  color_pixel(screen_buffer, r, g, b);
} /////////////////////////////////////////////////////////////////////////////////////////
void clear_buffer(std::string &screen_buffer) {
  Clear_Terminal();
  screen_buffer = "";
} /////////////////////////////////////////////////////////////////////////////////////////
std::vector<SH::square> generate_level(SH::vec2 &crystal) {
  std::vector<SH::square> sqlist;
  int target = 5000;
  SH::vec2 origin(0.0, 0.0);
  srand(time(NULL));
  while (SH::distance(origin, crystal) < 100) {
    crystal.x = rand() % 1000 - 500;
    crystal.y = rand() % 1000 - 500;
  }
  for (int i = 0; i < target; i++) {
    SH::vec2 sqpos(rand() % 1000 - 500, rand() % 1000 - 500);
    for (SH::square square : sqlist) {
      if (SH::distance(square.position, sqpos) <= square.side_length) {
        target++;
        continue;
      }
    }
    if (SH::distance(sqpos, origin) < 1.5 + sqsidelength / 1.5) {
      target++;
      continue;
    }
    if (SH::distance(sqpos, crystal) <
        sqsidelength / 1.5 + sqsidelength / 1.5) {
      target++;
      continue;
    }
    SH::square new_square(sqpos, sqsidelength);
    sqlist.push_back(new_square);
  }
  SH::square crystalsq(crystal, sqsidelength);
  sqlist.push_back(crystalsq);
  initial_dist_to_crystal = SH::distance(origin, crystal);
  return sqlist;
} /////////////////////////////////////////////////////////////////////////////////////////
void init_mazerunner() {
  int width;
  int height;
  int width_offset = 0;
  int height_offset = 0;
  std::string screen_buffer = "";
  bool running = true;
  SH::vec2 crystal(0.0, 0.0);
  std::vector<SH::square> sqlist = generate_level(crystal);
  while (
      running) { /////////////////////////////////////////////////////////////////////////////////////////
    auto tf = std::chrono::high_resolution_clock::now();
    clear_buffer(screen_buffer);
    get_terminal_size(width, height);
    // leave some space for text
    height -= 3;
    SH::vec2 CENTER_PIXEL(width_offset, height_offset);
    std::vector<SH::square> culled_sqlist;
    for (SH::square square : sqlist) {
      // cull all objects further than 100 pixel away(the limit of the player
      // vision practically)
      if (SH::distance(CENTER_PIXEL, square.position) < width / cull_factor)
        culled_sqlist.push_back(square);
    }
    for (int h = 0; h < height; h++) {
      for (int w = 0; w < width; w++) {
        render_pixel(w + width_offset, h + height_offset, width, height,
                     screen_buffer, culled_sqlist, CENTER_PIXEL, crystal);
      }
    }
    std::cout << screen_buffer;
    ////////////////////////////////////////////////////////////////////
    for (
        SH::square square :
        culled_sqlist) { ////////////////////////////////////////////////////////////////////
      if (SH::distance(CENTER_PIXEL, square.position) <=
          1.5 - 0.1 + sqsidelength / 2.0) {
        if (square.position.x == crystal.x && square.position.y == crystal.y) {
          std::cout
              << "\033[38;2;0;255;0mCONGRATULATIONS, YOU FOUND THE HIDDEN "
                 "CRYSTAL!!, q to quit to menu, r to play again\033[m\n";
        } else {
          std::cout
              << "\033[38;2;255;0;0mGAME OVER,q to quit to menu, r to play "
                 "again\033[m\n";
        }
        char option;
        while (option != 'q' && option != 'r') {
          std::cin >> option;
          std::cout << "\033[2K";
        }
        if (option == 'q') {
          return;
        } else {
          init_mazerunner();
          return;
        }
        return;
      }
    } /////////////////////////////////////////////////////////////////////////////////

    auto tl = std::chrono::high_resolution_clock::now();
    int mspf =
        std::chrono::duration_cast<std::chrono::milliseconds>(tl - tf).count();
    std::cout
        << "\033[38;2;200;125;125mDISTANCE TO CRYSTAL IS:"
        << SH::distance(CENTER_PIXEL, crystal) - 1.5 - sqsidelength / 2.0
        /*<< "\t\tNUMBER OF ITERATIONS PER FRAME:" << number_of_iterations*/
        << "\t\t MSPF:" << mspf << "\033[m\n";
    number_of_iterations = 0;
    char input;
    std::cin >> input;
    switch (input) {
    case 'r':
      init_mazerunner();
      return;
    case '+':
      cull_factor += 2;
      break;
    case '-':
      cull_factor = std::max(2, cull_factor - 2);
      break;
    case 'q':
      return;
      break;
    case 'a':
      width_offset -= 1;
      break;
    case 'd':
      width_offset += 1;
      break;
    case 's':
      height_offset += 1;
      break;
    case 'w':
      height_offset -= 1;
      break;
    }
    // test func
  }
}
