#include "raytracer.hpp"
#include "image.hpp"
#include "ray.hpp"
#include "renderer.hpp"
#include "scene.hpp"


PPMColor BinaryRaytracer::trace_ray(Ray& ray)
{
  Intersection hit_info;
  PPMColor color;

  if (!scene->intersect(ray, hit_info))
  {
    color = scene->bg_color;
    return color;
  }
  color = PPMColor(0,1,0);
  return color;
}

PPMColor PhongRaytracer::trace_ray(Ray& ray)
{
  Intersection hit_info;
  Vec3 color;

  if (!scene->intersect(ray, hit_info)){
    return PPMColor(scene->bg_color);
  }

  const Material& material = hit_info.object->material;

  Vec3 view_dir = Vec3::normalize(camera->get_position() - hit_info.position);

  Vec3 ambient = material.diffuse_color.to_vec() * scene->ambient_light->intensity * scene->ambient_light->color.to_vec();

  for (const auto& light : scene->lights)
  {
    Vec3 light_dir = Vec3::normalize(light->position - hit_info.position);
    Ray shadow_ray{hit_info.position, light_dir};
    Intersection shadow_hit;

    if (scene->object_in_shadow(shadow_ray, hit_info.object, &shadow_hit)){
      continue;
    }

    Vec3 H = Vec3::normalize(light_dir + view_dir);

    float NdotL = std::max(hit_info.normal.dot(light_dir), 0.0f);
    Vec3 diffuse = material.diffuse_color.to_vec() * light->intensity * material.kd * NdotL;

    float NdotH = std::max(hit_info.normal.dot(H), 0.0f);
    Vec3 specular =
        material.specular_color.to_vec() * light->intensity * material.ks * std::pow(NdotH, material.specular_exp);

    color = color + (diffuse + specular);
  }
  PPMColor final{color + ambient};
  final.clamp();
  return final;
}
