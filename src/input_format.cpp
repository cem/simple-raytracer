#include "input_format.h"

#include <string>
#include <istream>
#include <map>

#include "scene.h"
#include "sphere.h"
#include "triangle.h"
#include "plane.h"

void readTo(std::istream& file, V3* destination) {
  while (file && file.get() != '(')
    continue;
  float x, y, z;
  file >> x;
  while (file && file.get() != ',')
    continue;
  file >> y;
  while (file && file.get() != ',')
    continue;
  file >> z;
  while (file && file.get() != ')')
    continue;
  *destination = V3(x, y, z);
}

void readTo(std::istream& file, Color* destination) {
  V3 vec;
  readTo(file, &vec);
  *destination = Color(vec.x(), vec.y(), vec.z());
}

void initSceneFromFile(std::istream& file, Scene* scene, int* width, int* height, std::string* outfile_name) {
  std::string directive;
  std::map<std::string, Material*> materials;

  while (file >> directive) {
    if (directive == "//") {
      while (file && file.get() != '\n')
        continue;
    }
    else if (directive == "ambient") {
      readTo(file, &scene->ambient_);
    }
    else if (directive == "camera") {
      scene->camera_ = new Camera();
      readTo(file, &scene->camera_->origin_coord_);
      readTo(file, &scene->camera_->plane_center_coord_);
      readTo(file, &scene->camera_->plane_width_);
      readTo(file, &scene->camera_->plane_height_);
    }
    else if (directive == "light") {
      Light* light = new Light();
      readTo(file, &light->origin_coord_);
      readTo(file, &light->color_);
      readTo(file, &light->intensity_);
      readTo(file, &light->radius_);
      scene->lights_.push_back(light);
    }
    else if (directive == "sphere") {
      V3 origin;
      float radius;
      std::string material_name;
      readTo(file, &origin);
      file >> radius;
      file >> material_name;
      scene->objects_.push_back(new Sphere(origin, radius, materials[material_name]));
    }
    else if (directive == "plane") {
      V3 origin;
      V3 normal;
      std::string material_name;
      readTo(file, &origin);
      readTo(file, &normal);
      file >> material_name;
      scene->objects_.push_back(new Plane(origin, normal, materials[material_name]));
    }
    else if (directive == "triangle") {
      std::string material_name;
      V3 p0, p1, p2;
      readTo(file, &p0);
      readTo(file, &p1);
      readTo(file, &p2);
      file >> material_name;
      Material* material  = materials[material_name];
      scene->objects_.push_back(new Triangle(p0, p1, p2, material));
    }
    else if (directive == "material") {
      Material* material = new Material();
      std::string material_name;
      file >> material_name;
      readTo(file, &material->cDiffuse);
      readTo(file, &material->cReflection);
      readTo(file, &material->cSpecular);
      readTo(file, &material->kAmbient);
      readTo(file, &material->kDiffuse);
      readTo(file, &material->kOcclusion);
      readTo(file, &material->kReflection);
      readTo(file, &material->kSpecular);
      readTo(file, &material->shininess);
      materials[material_name] = material;
    }
    else if (directive == "render") {
      readTo(file, width);
      readTo(file, height);
      file >> *outfile_name;
    }
    else if (directive == "qualityparams") {
      readTo(file, &scene->max_reflections_);
      readTo(file, &scene->softlight_quality_);
      scene->softlight_enabled_ = scene->softlight_quality_ > 0;
      readTo(file, &scene->occlusion_quality_);
      scene->occlusion_enabled_ = scene->occlusion_quality_ > 0;
    }
    else {
      exit(-1);
    }
  }
}