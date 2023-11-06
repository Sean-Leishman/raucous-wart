#include "light.hpp"
#include "vector.hpp"

PointLight::PointLight(Vec3 position, Vec3 intensity): position(position), intensity(intensity), color(PPMColor(255, 255, 255)){};
float PointLight::compute_intensity( float NdotL) {
 return std::max(std::min(NdotL,1.0f), 0.0f);
}
