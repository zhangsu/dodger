#include "light.hh"

using glm::vec3;

// Public methods.

Light::Light(vec3 color, vec3 attenuation, bool cast_shadow)
    : color_(color),
      attenuation_(attenuation),
      cast_shadow_(cast_shadow) {}

const vec3& Light::color() const {
    return color_;
}

const vec3& Light::attenuation() const {
    return attenuation_;
}

bool Light::cast_shadow() const {
    return cast_shadow_;
}
