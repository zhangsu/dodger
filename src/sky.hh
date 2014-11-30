#ifndef DODGER_SKY_HH_
#define DODGER_SKY_HH_

#include "primitive.hh"

// A class that stores the properties of a sky.
class Sky : public Primitive {
  public:
    // Constructs a sky.
    Sky();

    // Renders this sky.
    virtual void render(Renderer&, glm::mat4 trans = glm::mat4()) const;
};

#endif // DODGER_SKY_HH_
