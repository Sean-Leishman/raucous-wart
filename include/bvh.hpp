#pragma once

#include "intersection.hpp"
#include "ray.hpp"
#include "shape.hpp"
#include "vector.hpp"

#include <memory>

struct BoundingBox
{
  Vec3 min;
  Vec3 max;

  bool intersect(const Ray& ray) { return false; }
};

class BVHNode
{
  public:
  BoundingBox box;
  std::unique_ptr<BVHNode> left;
  std::unique_ptr<BVHNode> right;
  std::vector<std::unique_ptr<Shape>> objects;

  BVHNode() = default;
  BVHNode(std::vector<std::unique_ptr<Shape>> shapes) : objects(shapes){};

  bool is_leaf() const { return objects.size() > 0; }
};

class BVHTree
{
  using Shapes = std::vector<std::unique_ptr<Shape>>;

  private:
  std::pair<Shapes, Shapes> split_objects(Shapes);

  public:
  float threshold;
  std::unique_ptr<BVHNode> root;

  std::unique_ptr<BVHNode>
  buildBVH(const std::vector<std::unique_ptr<Shape>>& objects);

  bool intersectBVH(const BVHNode& node, const Ray& ray,
                    Intersection& hit_info);
};
