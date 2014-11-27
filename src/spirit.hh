#ifndef DODGER_SPIRIT_HH_
#define DODGER_SPIRIT_HH_

#include <glm/glm.hpp>
#include "primitive.hh"

// A class that stores the properties of a terrain.
class Spirit : public Primitive {
  public:
    // Constructs a spherical sprit with the specified radius.
    explicit Spirit();

    // Renders this spirit and its descendents.
    virtual void render(const Renderer&, glm::mat4 trans = glm::mat4()) const;

  private:
};

#endif // DODGER_SPIRIT_HH_
