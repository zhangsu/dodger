#include "light.hh"

using glm::vec3;

// Public methods.

Light::Light(vec3 color, vec3 attenuation)
    : color_(color),
      attenuation_(attenuation) {}

const vec3& Light::color() const {
    return color_;
}

const vec3& Light::attenuation() const {
    return attenuation_;
}
