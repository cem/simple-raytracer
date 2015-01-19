#pragma once

#include "scene_base.h"

class Sphere : public SceneObject {
 public:
  Sphere(V3 origin_coord, float radius, Material* material);

  virtual Intersection intersect(Ray ray);

 private:
  V3 origin_coord_;
  float radius_;
};