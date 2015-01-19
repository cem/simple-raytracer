#pragma once

#include <string>
#include <istream>

#include "scene.h"

void readTo(std::istream& file, V3* destination);

void readTo(std::istream& file, Color* destination);

template <typename T>
void readTo(std::istream& file, T* destination) {
  file >> *destination;
}

void initSceneFromFile(std::istream& file, Scene* scene, int* width, int* height, std::string* outfile_name);