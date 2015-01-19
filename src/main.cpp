#include <vector>
#include <fstream>
#include <iostream>
#include <ctime>

#include "v3.h"
#include "scene_base.h"
#include "scene.h"
#include "sphere.h"
#include "image.h"
#include "plane.h"
#include "input_format.h"

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cout << "Usage: raytracer.exe scenefile" << std::endl;
    return 0;
  }

  std::string infile_name = argv[1];
  std::ifstream infile(infile_name);
  if (!infile) {
    std::cout << infile_name << " doesn't exist.";
    return -1;
  }

  int width, height;
  std::string outfile_name;

  Scene scene;
  initSceneFromFile(infile, &scene, &width, &height, &outfile_name);

  if (outfile_name.empty()) {
    std::cout << "Unexpected EOF. Corrupt input file?" << std::endl;
    return -1;
  }

  Image image(width, height);

  std::cout << "Rendering..." << std::endl;
  
  time_t start = time(0);
  scene.render(&image, width, height);
  std::cout << "Rendering finished in " << difftime(time(0), start) << " seconds." << std::endl;

  std::cout << "Writing to " << outfile_name << "." << std::endl;
  image.save(outfile_name);
}