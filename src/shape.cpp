#include "shape.hpp"
#include "ray.hpp"
#include "vector.hpp"
#include "intersection.hpp"

Sphere::Sphere()
{
  center = Vec3();
  radius = 0;
}

Sphere::Sphere(Vec3 center, float radius) : center(center), radius(radius){};

bool Sphere::intersect(const Ray& ray, float tmin, float tmax, Intersection& intersection) const {
Vec3 oc = ray.origin - center;
        float a = ray.direction.dot(ray.direction);
        float b = oc.dot(ray.direction);
        float c = oc.dot(oc) - radius * radius;
        float discriminant = b * b - a * c;

        if (discriminant > 0) {
            float temp = (-b - sqrt(discriminant)) / a;
            if (temp < tmax && temp > tMin) {
                intersection.distance = temp;
                intersection.position = ray.pointAtParameter(intersection.distance);
                intersection.normal = (intersection.position - center) / radius;
                intersection.object = this;
                return true;
            }
            temp = (-b + sqrt(discriminant)) / a;
            if (temp < tmax && temp > tMin) {
                intersection.distance = temp;
                intersection.position = ray.pointAtParameter(intersection.distance);
                intersection.normal = (intersection.position - center) / radius;
                intersection.object = this;
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

Cylinder::Cylinder(Vec3 center, Vec3 axis, float radius, float height)
    : center(center), axis(axis), radius(radius), height(height){};

bool Cylinder::intersect(const Ray& ray, float tmin, float tmax, Intersection& intersection) const {}

Triangle::Triangle(Vec3 v0, Vec3 v1, Vec3 v2) : v0(v0), v1(v1), v2(v2){};

bool Triangle::intersect(const Ray& ray, float tmin, float tmax, Intersection& intersection) const {}
