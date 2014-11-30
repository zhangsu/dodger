#ifndef DODGER_SPIRIT_HH_
#define DODGER_SPIRIT_HH_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "scene_node.hh"
#include "terrain.hh"

// A class that stores the properties of a terrain.
class Spirit : public SceneNode {
  public:
    // Constructs a spherical sprit with the specified terrain on which the
    // spirit walks.
    explicit Spirit(const Terrain*);

    // Translate this spirit by x, z on the terrain.
    void translate(float x, float z);
    // Renders this spirit and its descendents.
    virtual void render(Renderer&, glm::mat4 trans = glm::mat4()) const;

  private:
    const Terrain* const terrain_;
};

#endif // DODGER_SPIRIT_HH_
