#ifndef DODGER_MATERIAL_HH_
#define DODGER_MATERIAL_HH_

#include <glm/glm.hpp>

// A representation of material under Phong lighting model.
class Material {
public:
    // Constructs a pure black non-reflective material.
    Material();
    // Constructs a material with the specified diffuse and specular
    // coefficients and shininess.
    Material(glm::vec3 diffuse, glm::vec3 specular, float shininess);

    // Gets the diffuse coefficients of this material.
    const glm::vec3& diffuse() const;
    // Gets the specular coefficients of this material.
    const glm::vec3& specular() const;
    // Gets the shininess of this material.
    float shininess() const;

private:
    const glm::vec3 diffuse_;
    const glm::vec3 specular_;
    float shininess_;
};

#endif // DODGER_MATERIAL_HH_
