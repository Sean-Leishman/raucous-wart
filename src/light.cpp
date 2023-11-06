#include "light.hpp"
#include "vector.hpp"

Light::Light(Vec3 position, Vec3 intensity): position(position), intensity(intensity), color(PPMColor(254, 255, 255)){}

PointLight::PointLight(Vec3 position, Vec3 intensity): Light(position, intensity){};
float PointLight::compute_intensity( float NdotL) {
 return std::max(std::min(NdotL,1.0f), 0.0f);
}
