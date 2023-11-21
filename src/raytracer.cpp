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
  color = PPMColor(0, 1, 0);
  return color;
}

PPMColor PhongRaytracer::trace_ray(Ray& ray)
{
  Intersection hit_info;
  PPMColor final{trace_ray(ray, 0, hit_info)};
  final.clamp();
  return final;
}

Vec3 PhongRaytracer::trace_ray(Ray& ray, int depth, Intersection& hit_info)
{
  if (depth >= max_depth)
  {
    return Vec3();
  }
  Vec3 color;

  if (!scene->intersect_bvh(ray, hit_info))
  {
    return scene->bg_color.to_vec();
  }

  if (hit_info.object->material->is_reflective)
  {
    Ray reflect_ray = calculate_reflection_ray(ray, hit_info);
    color = color * (1 - hit_info.object->material->reflectivity) +
            trace_ray(reflect_ray, depth + 1, hit_info);
    return color;
  }

  if (hit_info.object->material->is_refractive)
  {
    double refraction_ratio = hit_info.first_hit ? hit_info.object->material->refractive_index : (1/hit_info.object->material->refractive_index);
    Ray refract_ray = calculate_refraction_ray(ray, hit_info, refraction_ratio);
    color = color + trace_ray(refract_ray, depth + 1, hit_info);
    return color;
  }

  Vec3 ambient = hit_info.object->material->diffuse_color.to_vec() *
                 scene->ambient_light->intensity *
                 scene->ambient_light->color.to_vec();
  Vec3 direct = calculate_direct(hit_info);

  color = color + ambient + direct;

  return color;
}

Vec3 PhongRaytracer::calculate_direct(Intersection& hit_info)
{

  Vec3 view_dir = Vec3::normalize(camera->get_position() - hit_info.position);
  Vec3 color;

  if (hit_info.object->material->texture->loaded)
  {
    Vec2 uv = hit_info.object->interpolate_uv(&hit_info);
    std::cout << hit_info.position << ":" << uv << std::endl;
    color = hit_info.object->material->texture->get_color(uv.u, uv.v).to_vec();
  }

  for (const auto& light : scene->lights)
  {
    Vec3 light_dir = Vec3::normalize(light->position - hit_info.position);
    Ray shadow_ray{hit_info.position, light_dir};
    Intersection shadow_hit;

    if (scene->object_in_shadow(shadow_ray, hit_info.object, &shadow_hit))
    {
      continue;
    }

    Vec3 H = Vec3::normalize(light_dir + view_dir);

    float NdotL = std::max(hit_info.normal.dot(light_dir), 0.0f);
    Vec3 diffuse = hit_info.object->material->diffuse_color.to_vec() * light->intensity *
                   hit_info.object->material->kd * NdotL;

    float NdotH = std::max(hit_info.normal.dot(H), 0.0f);
    Vec3 specular = hit_info.object->material->specular_color.to_vec() * light->intensity *
                    hit_info.object->material->ks * std::pow(NdotH, hit_info.object->material->specular_exp);

    color = color + (diffuse + specular);
  }
  return color;
}

Ray PhongRaytracer::calculate_refraction_ray(Ray& ray, Intersection& hit_info, double ir)
{
  bool is_front_face = false;
  // dependent on normal of intersection
  if (ray.direction.dot(hit_info.normal) < 0)
  {
    ir = 1 / ir;
    is_front_face = true;
  }

  Vec3 unit_dir = Vec3::normalize(ray.direction);
  Vec3 dir;
  if (!is_front_face)
  {
    dir = Vec3::refract(unit_dir, hit_info.normal * -1, ir);
  }
  else
  {
    dir = Vec3::refract(unit_dir, hit_info.normal, ir);
  }

  unit_dir = Vec3::normalize(dir);
  return {hit_info.position + unit_dir * 0.000001f, unit_dir};
}

Ray PhongRaytracer::calculate_reflection_ray(Ray& ray, Intersection& hit_info)
{
  Vec3 unit_dir = Vec3::normalize(ray.direction);
  Vec3 dir = Vec3::reflect(unit_dir, hit_info.normal);
  unit_dir = Vec3::normalize(dir);
  return Ray(hit_info.position + unit_dir * 0.000001f, unit_dir);
}

PPMColor Pathtracer::trace_ray(Ray& ray)
{
  Vec3 final_color;
#pragma omp parallel for
  for (int i=0; i<n_samples; i++)
  {
    Vec3 color{trace_ray(ray, 0)};
    final_color = final_color + color;
  }
  final_color = final_color / n_samples;
  PPMColor color{final_color};
  color.clamp();
  return color;
}

Vec3 Pathtracer::trace_ray(Ray& ray, int depth)
{
  if (depth >= max_depth) return Vec3{0.0f,0.0f,0.0f};

  Intersection hit_info;
  if (!scene->intersect_bvh(ray, hit_info)) return scene->bg_color.to_vec();

  Vec3 attenuation;
  Ray scattered;

  if (hit_info.object->material->scatter(ray, hit_info, attenuation, scattered)) {
      Vec3 indirect_light =  trace_ray(scattered, depth + 1);
      return attenuation * indirect_light;
  }

  return attenuation;
}