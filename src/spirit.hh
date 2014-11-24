#ifndef DODGER_SPIRIT_H_
#define DODGER_SPIRIT_H_

#include <glm/glm.hpp>
#include "scene_node.hh"

// A class that stores the properties of a terrain.
class Spirit : public SceneNode {
  public:
    // Constructs a spherical sprit with the specified radius.
    explicit Spirit();

    // Render this spirit and its descendents.
    virtual void render(const Renderer&, glm::mat4 trans = glm::mat4()) const;

  private:
};

#endif // DODGER_SPIRIT_H_
