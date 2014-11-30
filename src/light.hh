#ifndef DODGER_LIGHT_HH_
#define DODGER_LIGHT_HH_

#include <glm/glm.hpp>
#include "scene_node.hh"

// A representation of a point light with attenuation.
class Light : public SceneNode {
  public:
    // Constructs a light with the specified color and attenuation.
    Light(glm::vec3 color, glm::vec3 attenuation, bool cast_shadow = false);

    // Gets the color of this light.
    const glm::vec3& color() const;
    // Gets the attenuation of this light.
    const glm::vec3& attenuation() const;
    // Determines whether this light should cast shadow.
    bool cast_shadow() const;

  private:
    const glm::vec3 color_;
    const glm::vec3 attenuation_;
    const bool cast_shadow_;
};

#endif // DODGER_LIGHT_HH_
