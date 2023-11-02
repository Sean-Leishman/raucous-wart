#include "vector.hpp"

Vec3::Vec3() : x(0), y(0), z(0){};

Vec3::Vec3(float x, float y, float z) : x(x), y(y), z(z){};

Vec3::Vec3(std::vector<float> vec) : x(vec[0]), y(vec[1]), z(vec[2]){};
