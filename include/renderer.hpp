#pragma once

#include "input.hpp"

#include "camera.hpp"
#include "image.hpp"
#include "scene.hpp"

enum RenderMode
{
  BINARY,
};

class Renderer
{
  private:
  Camera camera;
  RenderMode render_mode;
  Scene scene;

  JSONParser parser;

  public:
  Renderer();
  int load_file(const std::string&);
};
