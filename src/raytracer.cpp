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
  PPMColor final{trace_ray(ray, 0)};
  final.clamp();
  return final;
}

Vec3 PhongRaytracer::trace_ray(Ray& ray, int depth)
{
  if (depth >= max_depth) {
    return Vec3();
  }
  Intersection hit_info;
  Vec3 color;

  if (!scene->intersect(ray, hit_info)){
    return scene->bg_color.to_vec();
  }

  const Material& material = hit_info.object->material;

  Vec3 ambient = material.diffuse_color.to_vec() * scene->ambient_light->intensity * scene->ambient_light->color.to_vec();
  Vec3 direct = calculate_direct(hit_info);

  color = color + ambient + direct;
  if (material.is_reflective) {
    Ray reflect_ray = calculate_reflection_ray(ray, hit_info);
    color = color * (1-hit_info.object->material.reflectivity) + trace_ray(reflect_ray, depth + 1);
  }

  if (material.is_refractive) {
    Ray refract_ray = calculate_refraction_ray(ray, hit_info);
    color = color + trace_ray(refract_ray, depth + 1);
  }

  return color;
}

Vec3 PhongRaytracer::calculate_direct(Intersection& hit_info){
  const Material& material = hit_info.object->material;

  Vec3 view_dir = Vec3::normalize(camera->get_position() - hit_info.position);
  Vec3 color;

  if (material.texture.loaded){
    Vec2 uv = hit_info.object->get_uv(hit_info.position);
    color=material.texture.get_color(uv.u, uv.v).to_vec();
  }

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
  return color;
}

Ray PhongRaytracer::calculate_refraction_ray(Ray& ray, Intersection& hit_info)
{
  float cos_theta = fmin((ray.direction * -1).dot(hit_info.normal), 1.0);
  Vec3 r_out_perp = (hit_info.normal + ray.direction * cos_theta) * hit_info.object->material.refractive_index;
  Vec3 r_out_parallel = hit_info.normal * -sqrt(fabs(1.0 - r_out_perp.dot(r_out_perp)));
  return {hit_info.position, r_out_perp + r_out_parallel};
}
Ray PhongRaytracer::calculate_reflection_ray(Ray& ray, Intersection& hit_info)
{
  Vec3 dir = ray.direction - hit_info.normal * 2 * ray.direction.dot(hit_info.normal);
  return {hit_info.position, dir};
}
