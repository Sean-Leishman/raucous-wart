#include "renderer.hpp"
#include "camera.hpp"
#include "input.hpp"
#include "scene.hpp"
#include "shape.hpp"

#include "json.hpp"
#include "light.hpp"
#include "material.hpp"
#include <filesystem>
#include <memory>

void Renderer::render_frame()
{
  scene.build_bvh();

  for (int x = 0; x < image_width; ++x)
  {
    for (int y = 0; y < image_height; ++y)
    {
      if (x == 560 && y == 325)
      {
        int r = 0;
      }
      Ray ray = camera.compute_ray((float)x, (float)y);
      PPMColor color = raytracer->trace_ray(ray);
      image.set_pixel(x, y, color);
    }
  }
  std::cout << scene.shapes[0] << "\n";

  std::filesystem::path path(std::filesystem::current_path());
  path += "/materials/2.ppm";
  image.save_to_file(path);
}

Material Renderer::load_material(nlohmann::json j)
{
  Material mat = Material();
  if (j.empty())
  {
    return mat;
  }

  mat.ks = j["ks"];
  mat.kd = j["kd"];
  mat.specular_exp = j["specularexponent"];
  mat.diffuse_color = PPMColor(j["diffusecolor"]);
  mat.specular_color = PPMColor(j["specularcolor"]);
  mat.is_reflective = j["isreflective"];
  mat.reflectivity = j["reflectivity"];
  mat.is_refractive = j["isrefractive"];
  mat.refractive_index = j["refractiveindex"];

  if (j.contains("texture"))
  {
    std::string key = j["texture"];
    if (textures.find(key) == textures.end())
    {
      std::filesystem::path filename(std::filesystem::current_path());
      filename += key;
      auto ptr = std::make_shared<Texture>(Texture{filename});

      textures.insert(std::make_pair(key, ptr));
    }
    auto texture_ptr = textures.find(key);
    mat.texture = texture_ptr->second;
  }

  return mat;
}

void Renderer::load_lights(nlohmann::json lights)
{
  scene.ambient_light =
      std::make_shared<AmbientLight>(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f));
  for (const auto& light : lights)
  {
    std::string type = light["type"];
    std::shared_ptr<Light> new_light;
    if (type == "pointlight")
    {
      std::vector<float> position = light["position"];
      std::vector<float> intensity = light["intensity"];

      new_light = std::make_shared<PointLight>(position, intensity);
      scene.lights.push_back(new_light);
    }
    else if (type == "ambientlight")
    {
      std::vector<float> intensity = light["intensity"];
      PPMColor color{light["color"]};
      new_light = std::make_shared<AmbientLight>(intensity, color);
      scene.ambient_light = new_light;
    }
  }
}

void Renderer::load_shapes(nlohmann::json shapes)
{
  for (const auto& shape : shapes)
  {
    std::string type = shape["type"];
    std::shared_ptr<Shape> new_shape;
    if (type == "sphere")
    {
      std::vector<float> center = shape["center"];
      float radius = shape["radius"];

      Material material;
      if (shape.contains("material"))
      {
        material = load_material(shape["material"]);
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
      if (shape.contains("material"))
      {
        material = load_material(shape["material"]);
      }
      new_shape = std::make_shared<Cylinder>(Vec3{center}, Vec3{axis}, radius,
                                             height, material);
    }
    else if (type == "triangle")
    {
      std::vector<float> v0 = shape["v0"];
      std::vector<float> v1 = shape["v1"];
      std::vector<float> v2 = shape["v2"];

      Material material;
      if (shape.contains("material"))
      {
        material = load_material(shape["material"]);
      }
      new_shape =
          std::make_shared<Triangle>(Vec3{v0}, Vec3{v1}, Vec3{v2}, material);
    }

    if (new_shape != nullptr)
    {
      scene.shapes.push_back(new_shape);
    }
  }
}

int Renderer::load_file(const std::string& filename)
{
  std::filesystem::path path(std::filesystem::current_path());
  path += filename;
  parser.read_file(path.string());

  auto input_render = parser.get<std::string>("rendermode");

  image_width = parser.get<int>("camera", "width");
  image_height = parser.get<int>("camera", "height");

  image.set(image_width, image_height, 255);

  auto position = parser.get<std::vector<float>>("camera", "position");
  auto lookAt = parser.get<std::vector<float>>("camera", "lookAt");
  auto upVector = parser.get<std::vector<float>>("camera", "upVector");
  auto fov = parser.get<float>("camera", "fov");
  auto exposure = parser.get<float>("camera", "exposure");

  camera =
      PRenderHole{image_width,    image_height, Vec3(position), Vec3(lookAt),
                  Vec3(upVector), fov,          exposure};

  auto scene_json = parser.get<nlohmann::json>("scene");
  auto bg = scene_json["backgroundcolor"];

  auto shapes = parser.get<std::vector<nlohmann::json>>("scene", "shapes");
  load_shapes(shapes);

  if (scene_json.contains("lightsources"))
  {
    load_lights(
        parser.get<std::vector<nlohmann::json>>("scene", "lightsources"));
  }

  if (input_render == std::string("binary"))
  {
    raytracer = std::make_unique<BinaryRaytracer>(&scene, &camera);
  }
  else if (input_render == std::string("phong"))
  {
    nbounces = parser.get<int>("nbounces");
    raytracer = std::make_unique<PhongRaytracer>(&scene, &camera, nbounces);
  }
  else
  {
    throw std::runtime_error("Raytracer not initialised");
  }

  return 0;
}

Renderer::Renderer() {}
