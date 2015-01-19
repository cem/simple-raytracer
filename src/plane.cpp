#include "plane.h"

Plane::Plane(V3 origin_coord, V3 normal_unit_vec, Material* material)
    : origin_coord_(origin_coord), normal_unit_vec_(normal_unit_vec) {
  material_ = material;
}
   
Intersection Plane::intersect(Ray ray) {
  Intersection result;
  result.happened_ = false;

  float top = (origin_coord_ - ray.origin_coord_).dot(normal_unit_vec_);
  float bottom = ray.direction_unit_vec_.dot(normal_unit_vec_);
    
  if (abs(bottom) < 0.001)
    return result;

  float k = top / bottom;
  if (k < 0)
    return result;

  result.happened_ = true;
  result.normal_unit_vec_ = normal_unit_vec_;
  result.contact_coord_ = ray.direction_unit_vec_ * k + ray.origin_coord_;
  return result;
}