#ifndef DODGER_PRIMITIVE_HH_
#define DODGER_PRIMITIVE_HH_

#include "material.hh"
#include "scene_node.hh"

// The base type for any node in a scene graph.
class Primitive : public SceneNode {
  public:
    // Gets the material for this terrain.
    const Material& material() const;

  protected:
    Primitive(Material);

  private:
    const Material material_;
};

#endif // DODGER_PRIMITIVE_HH_
