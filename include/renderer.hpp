#pragma once

#include "input.hpp"

#include "camera.hpp"
#include "image.hpp"
#include "ray.hpp"
#include "scene.hpp"

enum RenderMode
{
  BINARY,
};

class Renderer
{
  private:
  int image_width;
  int image_height;

  PRenderHole camera;
  RenderMode render_mode;
  Scene scene;
  PPMImage image;

  JSONParser parser;

  public:
  Renderer();
  int load_file(const std::string&);

  void render_frame();
  PPMColor trace_ray(Ray ray);
};
