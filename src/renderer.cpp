#include "renderer.hpp"
#include "camera.hpp"
#include "input.hpp"
#include "scene.hpp"
#include "shape.hpp"

#include "json.hpp"
#include "material.hpp"
#include <memory>

Renderer::Renderer(){};

void Renderer::render_frame()
{
  for (int x = 0; x < image_width; ++x)
  {
    for (int y = 0; y < image_height; ++y)
    {
      Ray ray = camera.compute_ray((float)x, (float)y);
      PPMColor color = trace_ray(ray);
      image.set_pixel(x, y, color);
    }
  }

  image.save_to_file("/home/seanleishman/University/cg/cw2/materials/1.ppm");
}

PPMColor Renderer::trace_ray(Ray ray)
{
  Intersection hit_info;
  PPMColor color;

  if (scene.intersect(ray, hit_info))
  {
    switch (render_mode){
      case BINARY:
        color = PPMColor(0,255,0);
    }
  }
  else{
    color = scene.bg_color;
  }

  return color;
}

Material Renderer::load_material(nlohmann::json j){
  Material mat{};
  if (j.empty()){
    return mat;
  }

  mat.ks = j['ks'];
  mat.kd = j['kd'];
  mat.specular_exp = j['specularexponent'];
  mat.diffuse_color = PPMColor(j['diffusecolor'][0], j['diffusecolor'][1], j['diffusecolor'][2]);
  mat.specular_color = PPMColor(j['specularcolor'][0], j['specularcolor'][1], j['specularcolor'][2]);
  mat.is_reflective = j['isreflective'];
  mat.reflectivity = j['reflectivity'];
  mat.is_refractive = j['isrefractive'];
  mat.refractive_index = j['refractiveindex'];

  return mat;
}

int Renderer::load_file(const std::string& filename)
{
  parser.read_file(filename);
  auto input_render = parser.get<std::string>("rendermode");

  if (input_render == std::string("binary"))
    render_mode = BINARY;

  image_width = parser.get<int>("camera", "width");
  image_height = parser.get<int>("camera", "height");

  image = PPMImage(image_width, image_height, 255);

  auto position =
      parser.get<std::vector<float>>("camera", "position");
  auto lookAt =
      parser.get<std::vector<float>>("camera", "lookAt");
  auto upVector =
      parser.get<std::vector<float>>("camera", "upVector");
  auto fov = parser.get<float>("camera", "fov");
  auto exposure = parser.get<float>("camera", "exposure");

  camera =
      PRenderHole{image_width,    image_height, Vec3(position), Vec3(lookAt),
                  Vec3(upVector), fov,          exposure};

  auto bg =
      parser.get<std::vector<float>>("scene", "backgroundcolor");

  auto shapes =
      parser.get<std::vector<nlohmann::json>>("scene", "shapes");
  for (const auto& shape : shapes)
  {
    std::string type = shape["type"];
    std::shared_ptr<Shape> new_shape;
    if (type == "sphere")
    {
      std::vector<float> center = shape["center"];
      float radius = shape["radius"];

      Material material;
      if (shape.contains("material")){
        material = load_material(shape['material']);
      }

      new_shape = std::make_shared<Sphere>(Vec3{center}, radius, material);
    }
    else if (type == "cylinder")
    {
      std::vector<float> center = shape["center"];
      std::vector<float> axis = shape["axis"];
      float radius = shape["radius"];
      float height = shape["height"];

      Material material;
      if (shape.contains("material")){
        material = load_material(shape['material']);
      }
      new_shape =
          std::make_shared<Cylinder>(Vec3{center}, Vec3{axis}, radius, height, material);
    }
    else if (type == "triangle")
    {
      std::vector<float> v0 = shape["v0"];
      std::vector<float> v1 = shape["v1"];
      std::vector<float> v2 = shape["v2"];

      Material material;
      if (shape.contains("material")){
        material = load_material(shape['material']);
      }
      new_shape = std::make_shared<Triangle>(Vec3{v0}, Vec3{v1}, Vec3{v2}, material);
    }

    if (new_shape != nullptr){
    scene.shapes.push_back(new_shape);}
  }

  return 0;
}


