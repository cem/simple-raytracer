#pragma once

#include <vector>
#include "scene_base.h"
#include "image.h"

class Scene {
 public:
  Camera* camera_;
  std::vector<Light*> lights_;
  std::vector<SceneObject*> objects_;
  Color ambient_;
  int softlight_quality_;
  int occlusion_quality_;
  bool softlight_enabled_;
  bool occlusion_enabled_;
  int max_reflections_;

  Scene() : softlight_quality_(50), occlusion_quality_(100), softlight_enabled_(true), occlusion_enabled_(true), max_reflections_(5) {}
  Color trace(Ray ray, int reflected = 0);
  void render(Image* image, int width, int height);
  void init(std::istream & file, int* width, int* height, std::string* outfile_name);

 private:
  bool getFirstObject(Ray ray, SceneObject** object, Intersection* intersection);
};
