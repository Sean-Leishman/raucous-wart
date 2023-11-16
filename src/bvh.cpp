#include "bvh.hpp"

std::unique_ptr<BVHNode>
BVHTree::buildBVH(const std::vector<std::unique_ptr<Shape>>& objects)
{
  if (objects.size() <= threshold)
  {
    return std::make_unique<BVHNode>(objects);
  }
  auto [leftObjects, rightObjects] = split_objects(objects);
  std::unique_ptr<BVHNode> leftChild = buildBVH(leftObjects);
  std::unique_ptr<BVHNode> rightChild = buildBVH(rightObjects);

  auto node = std::make_unique<BVHNode>();
  node->left = std::move(leftChild);
  node->right = std::move(rightChild);
  updateBoundingBox(node.get());

  root = node;
}

std::pair<BVHTree::Shapes, BVHTree::Shapes>
BVHTree::split_objects(BVHTree::Shapes objects)
{
  char axis = 'x';
  std::vector<float> centroids;
  centroids.reserve(objects.size());
  for (const auto& obj : objects)
  {
    centroids.push_back(obj->bbox.centroid()[axis]);
  }

  std::nth_element(centroids.begin(), centroids.begin() + centroids.size() / 2,
                   centroids.end());
  float median = centroids[centroids.size() / 2];

  // Split objects into two groups based on the median
  std::vector<Object*> leftObjects, rightObjects;
  for (const auto& obj : objects)
  {

    if (obj->bbox.centroid()[axis] < median)
    {
      leftObjects.push_back(obj);
    }
    else
    {
      rightObjects.push_back(obj);
    }
  }

  return {leftObjects, rightObjects};
}
