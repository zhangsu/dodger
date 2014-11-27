#ifndef DODGER_TERRAIN_HH_
#define DODGER_TERRAIN_HH_

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "scene_node.hh"

// A class that stores the properties of a terrain.
class Terrain : public SceneNode {
  public:
    // Constructs a terrain from the specified heightmap. The real height at
    // a certain point is `modifier` * the value at the corresponding location
    // in the heightmap ([0, 1]). The width and height of the terrain grid is
    // `xyscale`.
    explicit Terrain(std::string heightmap_filename,
                     float modifier = 0.5,
                     float xyscale = 1);

    // Renders this terrain and its descendents.
    virtual void render(const Renderer&, glm::mat4 trans = glm::mat4()) const;
    // Gets the number of heights in the x direction.
    int width() const;
    // Gets the number of heights in the y direction.
    int height() const;
    // Gets the specified row of the heightmap.
    const std::vector<float>& operator [](int index) const;

  private:
    const float modifier_;
    const float xyscale_;

    int width_;
    int height_;
    std::vector<std::vector<float>> heightmap_;
};

#endif // DODGER_TERRAIN_HH_
