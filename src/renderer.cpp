#include "renderer.hpp"
#include "camera.hpp"
#include "input.hpp"
#include "scene.hpp"
#include "shape.hpp"

#include "json.hpp"

Renderer::Renderer(){};

int Renderer::load_file(const std::string& filename)
{
  parser.read_file(filename);
  std::string input_render = parser.get<std::string>("rendermode");

  if (input_render == std::string("binary"))
    render_mode = BINARY;

  int width = parser.get<int>("camera", "width");
  int height = parser.get<int>("camera", "height");
  std::vector<float> position =
      parser.get<std::vector<float>>("camera", "position");
  std::vector<float> lookAt =
      parser.get<std::vector<float>>("camera", "lookAt");
  std::vector<float> upVector =
      parser.get<std::vector<float>>("camera", "upVector");
  float fov = parser.get<float>("camera", "fov");
  float exposure = parser.get<float>("camera", "exposure");

  camera = PRenderHole{width,          height, Vec3(position), Vec3(lookAt),
                       Vec3(upVector), fov,    exposure};

  std::vector<float> bg =
      parser.get<std::vector<float>>("scene", "backgroundcolor");

  std::vector<nlohmann::json> shapes =
      parser.get<std::vector<nlohmann::json>>("scene", "shapes");
  for (const auto& shape : shapes)
  {
    std::string type = shape["type"];
    Shape new_shape;
    if (type == "sphere")
    {
      std::vector<float> center = shape["center"];
      float radius = shape["radius"];

      new_shape = Sphere{Vec3{center}, radius};
    }
    else if (type == "cylinder")
    {
      std::vector<float> center = shape["center"];
      std::vector<float> axis = shape["axis"];
      float radius = shape["radius"];
      float height = shape["height"];

      new_shape = Cylinder{Vec3{center}, Vec3{axis}, radius, height};
    }
    else if (type == "triange")
    {
      std::vector<float> v0 = shape["v0"];
      std::vector<float> v1 = shape["v1"];
      std::vector<float> v2 = shape["v2"];

      new_shape = Triangle{Vec3{v0}, Vec3{v1}, Vec3{v2}};
    }

    scene.add_shape(new_shape);
  }

  return 0;
}
