#pragma once

#include "input.hpp"

#include "camera.hpp"
#include "image.hpp"
#include "ray.hpp"
#include "raytracer.hpp"
#include "scene.hpp"

enum RenderMode
{
  BINARY,
  PHONG,
};

class Renderer
{
  private:
  int image_width;
  int image_height;
  int nbounces;

  PRenderHole camera;
  std::unique_ptr<Raytracer> raytracer;
  Scene scene;
  PPMImage image;

  JSONParser parser;

  static Material load_material(nlohmann::json j);
  void load_shapes(nlohmann::json j);
  void load_lights(nlohmann::json j);

  public:
      ~Renderer()=default;
  Renderer();
  int load_file(const std::string&);

  void render_frame();

};
