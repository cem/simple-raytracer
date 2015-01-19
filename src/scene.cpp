#include <map>

#include "scene.h"
#include "sphere.h"
#include "plane.h"
#include "triangle.h"

namespace {
V3 randomUnit() {
  const float rand_max = static_cast<float>(RAND_MAX);
  return V3((static_cast<float>(rand()) / rand_max) - 0.5f,
            (static_cast<float>(rand()) / rand_max) - 0.5f,
            (static_cast<float>(rand()) / rand_max) - 0.5f).unit();
}
}

bool Scene::getFirstObject(Ray ray, SceneObject** object, Intersection* intersection) {
  bool found = false;
  float distance = -1;

  for (unsigned int i = 0; i < objects_.size(); ++i) {
    SceneObject* current_object = objects_[i];
    Intersection current_intersection = current_object->intersect(ray);

    if (current_intersection.happened_) {
      float current_distance = (current_intersection.contact_coord_ - ray.origin_coord_).length();
      if (current_distance > 0.001 && (distance < 0 || current_distance < distance)) {
        found = true;
        distance = current_distance;
        if (object)
          *object = current_object;
        if (intersection)
          *intersection = current_intersection;
      }
    }
  }

  return found;
}

Color Scene::trace(Ray ray, int reflection_count) {
  Color result(0, 0, 0);
  if (reflection_count > max_reflections_)
    return result;

  SceneObject* object;
  Material* material;
  Intersection intersection;

  if (!getFirstObject(ray, &object, &intersection))
    return result;

  material = object->material_;

  // Calculate ambient
  Color ambient = ambient_ * material->kAmbient;

  // Calculate diffuse and specular
  Color diffuse(0, 0, 0);
  Color specular(0, 0, 0);
  V3 reflected = - ray.reflect(intersection.normal_unit_vec_).direction_unit_vec_;
  for (unsigned int i = 0; i < lights_.size(); ++i) {
    Light* light = lights_[i];

    int softlight_steps = softlight_enabled_ ? softlight_quality_ : 1;
    for (int j = 0; j < softlight_steps; ++j) {
      Ray lightray;
      V3 light_origin = light->origin_coord_;
      if (softlight_enabled_)
        light_origin += randomUnit() * light->radius_;
      lightray.origin_coord_ = light_origin;
      lightray.direction_unit_vec_ = (intersection.contact_coord_ - light_origin).unit();
      Intersection light_intersection;
      bool found = getFirstObject(lightray, NULL, &light_intersection);

      if (!found || !light_intersection.happened_
          || (light_intersection.contact_coord_ - intersection.contact_coord_).length() > 0.0001)
        continue;

      float falloff = (intersection.contact_coord_ - light_origin).length();
      falloff *= falloff;
        
      float diffusion = intersection.normal_unit_vec_.dot((light_origin - intersection.contact_coord_).unit());
      if (diffusion > 0)
        diffuse += light->color_ * material->cDiffuse * material->kDiffuse * light->intensity_ * diffusion * (1 / falloff);

      float specularity = reflected.dot(-lightray.direction_unit_vec_);
      if (specularity > 0) 
        specular += light->color_ * material->cSpecular * material->kSpecular * light->intensity_ * pow(specularity, material->shininess) * (1 / falloff);
    }
    diffuse = diffuse / static_cast<float>(softlight_steps);
    specular = specular / static_cast<float>(softlight_steps);
  }

  // Calculate ambient occlusion
  Color occlusion(0, 0, 0);
  if (occlusion_enabled_) {
    float occlusion_amount = 1.0f;
    const float occlusion_step = 1.0f / occlusion_quality_;
    for (int i = 0; i < occlusion_quality_; ++i) {
      V3 random_dir = randomUnit();
      random_dir = random_dir.unit();
      if (random_dir.dot(intersection.normal_unit_vec_) < 0)
        random_dir = -random_dir;
      Ray occlusion_ray(intersection.contact_coord_, random_dir);
      SceneObject* occlusion_object;
      Intersection occlusion_intersection;
      if (getFirstObject(occlusion_ray, &occlusion_object, &occlusion_intersection)) {
        occlusion_amount -= occlusion_step;
      }
    }
    occlusion = material->cDiffuse * occlusion_amount * material->kOcclusion;
  }

  // Calculate reflection
  Color reflection = trace(Ray(intersection.contact_coord_, reflected.unit()), reflection_count + 1)
      * material->cReflection
      * material->kReflection;

  result = ambient + diffuse + specular + occlusion + reflection;
  result.clamp();
  return result;
}

void Scene::render(Image* image, int width, int height) {
  V3 normal = (camera_->plane_center_coord_ - camera_->origin_coord_).unit();
  V3 up(0, 0, 1);

  float plane_left_length = camera_->plane_width_ / width;
  float plane_up_length = camera_->plane_height_ / height;
    
  V3 plane_left = up.cross(normal).unit() * plane_left_length;
  V3 plane_right = -plane_left;
  V3 plane_up = normal.cross(plane_left).unit() * plane_up_length;
  V3 plane_down = - plane_up;

  // Coordinate
  V3 plane_top_left_corner_coord = camera_->plane_center_coord_ +
                                   (plane_left * static_cast<float>(width) / 2.0f) +
                                   (plane_up * static_cast<float>(height) / 2.0f);

  // Vector from camera
  V3 plane_top_left_corner = plane_top_left_corner_coord - camera_->origin_coord_;

  for (int w = 0; w < width; ++w) {
    for (int h = 0; h < height; ++h) {
      Ray ray;
      ray.origin_coord_ = camera_->origin_coord_;
      ray.direction_unit_vec_ = (plane_top_left_corner +
                                 plane_right * static_cast<float>(w) +
                                 plane_down * static_cast<float>(h)).unit();

      Color pixel = trace(ray);
      pixel.gammaCorrect();
      image->set(w, h, static_cast<unsigned char>(pixel.r() * 255),
                       static_cast<unsigned char>(pixel.g() * 255),
                       static_cast<unsigned char>(pixel.b() * 255),
                       255);
    }
  }
}