#pragma once

#include "scene_base.h"

class Triangle : public SceneObject {
 public:
  Triangle(V3 p0, V3 p1, V3 p2, Material* material);

  virtual Intersection intersect(Ray ray);

 private:
  V3 p0_, p1_, p2_;
};