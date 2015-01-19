#pragma once

#include "scene_base.h"

class Plane : public SceneObject {
 public:
  Plane(V3 origin_coord, V3 normal_unit_vec, Material* material);
   
  Intersection intersect(Ray ray);

 private:
  V3 origin_coord_;
  V3 normal_unit_vec_;
};