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

  const Material& material = hit_info.object->material;
  if (material.is_reflective)
  {
    Ray reflect_ray = calculate_reflection_ray(ray, hit_info);
    hit_info.first_hit = false;
    color = color * (1 - hit_info.object->material.reflectivity) +
            trace_ray(reflect_ray, depth + 1, hit_info);
    return color;
  }

  if (material.is_refractive)
  {
    double refraction_ratio = hit_info.first_hit ? hit_info.object->material.refractive_index : (1/hit_info.object->material.refractive_index);
    Ray refract_ray = calculate_refraction_ray(ray, hit_info, refraction_ratio);
    hit_info.first_hit = false;
    color = color + trace_ray(refract_ray, depth + 1, hit_info);
    return color;
  }

  Vec3 ambient = material.diffuse_color.to_vec() *
                 scene->ambient_light->intensity *
                 scene->ambient_light->color.to_vec();
  Vec3 direct = calculate_direct(hit_info);

  color = color + ambient + direct;

  return color;
}

Vec3 PhongRaytracer::calculate_direct(Intersection& hit_info)
{
  const Material& material = hit_info.object->material;

  Vec3 view_dir = Vec3::normalize(camera->get_position() - hit_info.position);
  Vec3 color;

  if (material.texture->loaded)
  {
    Vec2 uv = hit_info.object->interpolate_uv(&hit_info);
    std::cout << hit_info.position << ":" << uv << std::endl;
    color = material.texture->get_color(uv.u, uv.v).to_vec();
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
    Vec3 diffuse = material.diffuse_color.to_vec() * light->intensity *
                   material.kd * NdotL;

    float NdotH = std::max(hit_info.normal.dot(H), 0.0f);
    Vec3 specular = material.specular_color.to_vec() * light->intensity *
                    material.ks * std::pow(NdotH, material.specular_exp);

    color = color + (diffuse + specular);
  }
  return color;
}

Ray PhongRaytracer::calculate_refraction_ray(Ray& ray, Intersection& hit_info, double ir)
{
  ray.direction.normalize();
  float cos_theta = fmin((ray.direction * -1).dot(hit_info.normal), 1.0);
  float sin_theta = sqrt(1.0 - cos_theta * cos_theta);
  bool cannot_refract = ir * sin_theta > 1.0;

 // Schlick's Approx
  auto r0 = (1 - ir) / (1+ir);
  r0 *= r0;
  r0 = r0 + (1-r0)*pow((1-cos_theta),5);

  Vec3 unit_vector = Vec3::normalize(ray.direction);
  Vec3 dir;
  if (cannot_refract || r0 > random_double()){
    dir = Vec3::reflect(unit_vector, hit_info.normal);
  }
  else {
    dir = Vec3::refract(unit_vector, hit_info.normal, ir);
  }
  dir = Vec3::normalize(dir);

  if (hit_info.object.get()->name() == "Sphere")
  {
    /*
    std::cout << "Refraction: " << hit_info.object.get()->name() << "; "
              << cannot_refract << "; " << dir << "; " << ray.direction << "; "
              << hit_info.normal << std::endl;
              */
  }
  return {hit_info.position + dir * 0.0000001f, dir};


 /*
  float cosi = std::max(-1.0f, std::min(1.0f, ray.direction.dot(hit_info.normal)));
  float etai = 1, etat = hit_info.object->material.refractive_index;
  Vec3 n = hit_info.normal;
  if (cosi < 0) {
    cosi = -cosi;
  } else {
    std::swap(etai, etat);
    n= hit_info.normal * -1;
  }

  float eta = etai / etat;
  float k = 1 - eta * eta * (1 - cosi * cosi);
  Vec3 dir;
  if (k < 0)
  {
    dir = Vec3(0.0f, 0.0f, 0.0f);
  }
  else
    dir = ray.direction * eta + n * (eta * cosi - sqrt(k));

  return {hit_info.position + dir * 0.000001f, dir};
  */
}

Ray PhongRaytracer::calculate_reflection_ray(Ray& ray, Intersection& hit_info)
{
  Vec3 dir =
      ray.direction - hit_info.normal * 2 * ray.direction.dot(hit_info.normal);
  dir = Vec3::normalize(dir);
  return {hit_info.position, dir};
}


PPMColor Pathtracer::trace_ray(Ray& ray)
{
  Vec3 final_color;
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
  if (!scene->intersect(ray, hit_info)) return scene->bg_color.to_vec();

  Vec3 attenuation;
  Ray scattered;
  const Material& material = hit_info.object->material;

  Vec3 direct_lighting = calculate_direct_lighting(ray, hit_info);

  if (material.scatter(ray, hit_info, attenuation, scattered)) {
      Vec3 color =  attenuation * trace_ray(scattered, depth + 1);
      return direct_lighting + color;
  }
}

Vec3 Pathtracer::calculate_direct_lighting(Ray& ray, Intersection& hit_info) {
  const Material& material = hit_info.object->material;

  Vec3 view_dir = Vec3::normalize(camera->get_position() - hit_info.position);
  Vec3 color;

  if (material.texture->loaded)
  {
    Vec2 uv = hit_info.object->interpolate_uv(&hit_info);
    std::cout << hit_info.position << ":" << uv << std::endl;
    color = material.texture->get_color(uv.u, uv.v).to_vec();
  }

  for (const auto& light : scene->lights)
  {
    Vec3 light_dir = Vec3::normalize(light->get_position() - hit_info.position);
    Ray shadow_ray{hit_info.position, light_dir};
    Intersection shadow_hit;

    if (scene->object_in_shadow(shadow_ray, hit_info.object, &shadow_hit))
    {
      continue;
    }

    Vec3 H = Vec3::normalize(light_dir + view_dir);

    float NdotL = std::max(hit_info.normal.dot(light_dir), 0.0f);
    Vec3 diffuse = material.diffuse_color.to_vec() * light->intensity *
                   material.kd * NdotL;

    float NdotH = std::max(hit_info.normal.dot(H), 0.0f);
    Vec3 specular = material.specular_color.to_vec() * light->intensity *
                    material.ks * std::pow(NdotH, material.specular_exp);

    color = color + (diffuse + specular);
  }
  return color;
}