#ifndef DODGER_TERRAIN_HH_
#define DODGER_TERRAIN_HH_

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "primitive.hh"

// A class that stores the properties of a terrain.
class Terrain : public Primitive {
  public:
    // Constructs a terrain from the specified heightmap, each grid of which
    // has a unit length of 1 in the x and z directions, and a value between 0
    // and 1 in the y direction.
    explicit Terrain(std::string heightmap_filename);

    // Renders this terrain and its descendents.
    virtual void render(const Renderer&, glm::mat4 trans = glm::mat4()) const;
    // Gets the number of heights in the x direction.
    int width() const;
    // Gets the number of heights in the y direction.
    int height() const;
    // Gets the specified row of the heightmap.
    const std::vector<float>& operator [](int index) const;

  private:
    int width_;
    int height_;
    std::vector<std::vector<float>> heightmap_;
};

#endif // DODGER_TERRAIN_HH_
