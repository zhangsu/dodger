#include "material.hh"

using glm::vec3;

// Public methods.

Material::Material() {}

Material::Material(vec3 diffuse, vec3 specular, float shininess)
    : diffuse_(diffuse),
      specular_(specular),
      shininess_(shininess) {}

const vec3& Material::diffuse() const {
    return diffuse_;
}

const vec3& Material::specular() const {
    return specular_;
}

float Material::shininess() const {
    return shininess_;
}
