#include "shape.hpp"
#include "intersection.hpp"
#include "material.hpp"
#include "ray.hpp"
#include "vector.hpp"

#define EPSILON 0.00000001f

Sphere::Sphere()
{
  center = Vec3();
  radius = 0;
}

Sphere::Sphere(Vec3 center, float radius, Material material) : center(center), radius(radius), material(material){};

bool Sphere::intersect(const Ray& ray, float tmin, float tmax, Intersection* intersection)
{
        Vec3 oc = ray.origin - center;
        float a = ray.direction.dot(ray.direction);
        float b = oc.dot(ray.direction);
        float c = oc.dot(oc) - radius * radius;
        float discriminant = b * b - a * c;

        if (discriminant > 0) {
            float temp = (-b - sqrt(discriminant)) / a;
            if (temp < tmax && temp > tmin) {
                intersection->distance = temp;
                intersection->position = ray.point_at_parameter(intersection->distance);
                intersection->normal = (intersection->position - center) / radius;
                intersection->object = get_shared_ptr();
                return true;
            }
            temp = (-b + sqrt(discriminant)) / a;
            if (temp < tmax && temp > tmin) {
                intersection->distance = temp;
                intersection->position = ray.point_at_parameter(intersection->distance);
                intersection->normal = (intersection->position - center) / radius;
                intersection->object = get_shared_ptr();
                return true;
            }
        }
        return false;

}

Cylinder::Cylinder()
{
  center = Vec3();
  axis = Vec3();
  radius = 0;
  height = 0;
}

Cylinder::Cylinder(Vec3 center, Vec3 axis, float radius, float height,
                   Material material )
    : center(center), axis(axis), radius(radius), height(height), material(material){};

bool Cylinder::intersect(const Ray& ray, float tmin, float tmax, Intersection* intersection) {
  Vec3 co = ray.origin - center;
  float a = ray.direction.dot(ray.direction) - pow(ray.direction.dot(axis), 2);
  float b = 2 * (ray.direction.dot(co) - (ray.direction.dot(axis) * co.dot(axis)));
  float c = co.dot(co) - pow(co.dot(axis), 2) - radius * radius;

  float discriminant = b * b - 4 * a * c;
  if (discriminant < 0) {
            return {};  // No intersection
  }

  float sqrt_discriminant = sqrt(discriminant);
  float t1 = (-b - sqrt_discriminant) / (2 * a);
  float t2 = (-b + sqrt_discriminant) / (2 * a);

  // Check if the intersections are within the bounds of the cylinder's height
  float y_min = center.y - height / 2;
  float y_max = center.y + height / 2;

  float x_min = center.x - radius / 2;
  float x_max = center.x + radius / 2;

  float t = t1;
  if (t < tmin || t > tmax) {
            t = t2;
            if (t < tmin || t > tmax) {
                return {};  // Both intersection points are out of bounds
            }
  }

  // Get the intersection point
  Vec3 point = ray.point_at_parameter(t);

  // Check if the intersection point is within the height bounds of the cylinder
  if (point.y < y_min || point.y > y_max) return false;
  if (point.x < x_min || point.x > x_max) return false;

  // Calculate the normal at the intersection point
  Vec3 normal = (point - center - axis * (point - center).dot(axis));

  intersection->position = point;
  intersection->normal = normal;
  intersection->object = get_shared_ptr();
  intersection->distance = t;
  return true;


}

Triangle::Triangle(): v0(Vec3()), v1(Vec3()), v2(Vec3()), material(Material()) {}
Triangle::Triangle(Vec3 v0, Vec3 v1, Vec3 v2, Material material) : v0(v0), v1(v1), v2(v2), material(material){};

bool Triangle::intersect(const Ray& ray, float tmin, float tmax, Intersection* intersection)
{
  Vec3 edge1 = v1 - v0;
  Vec3 edge2 = v2 - v0;

  Vec3 h = ray.direction.cross(edge2);
  float a = edge1.dot(h);

  if (a > -EPSILON && a < EPSILON)
            return false;

  float f = 1.0 / a;
  Vec3 s = ray.origin - v0;
  float u = f * s.dot(h);

  if (u < 0.0 || u > 1.0)
            return false;

  Vec3 q = s.cross(v1 - v0);
  float v = f * ray.direction.dot(q);

  if (v < 0.0 || u + v > 1.0)
            return false;

  float t = f * edge2.dot(q);
  if (t > tmin && t < tmax)
  {
            intersection->position = ray.point_at_parameter(t);
            intersection->distance = t;
            intersection->normal = edge1.cross(edge2);
            intersection->object = get_shared_ptr();
            std::cout << intersection->object->material.diffuse_color.r << " " << get_shared_ptr().use_count() << "\n";
            return true;
  }

  return false;
}