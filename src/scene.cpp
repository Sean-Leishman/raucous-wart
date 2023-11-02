#include "scene.hpp"
#include "shape.hpp"

#include <vector>

Scene::Scene()
{
  bg_color = {0, 0, 0};
  shapes = std::vector<Shape>();
}

void Scene::add_shape(Shape shape) { shapes.push_back(shape); }
