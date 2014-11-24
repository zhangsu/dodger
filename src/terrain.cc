#include <string>
#include "image.hh"
#include "renderer.hh"
#include "terrain.hh"

using std::string;
using glm::mat4;

// Public methods.

Terrain::Terrain(string heightmap_filename, float modifier, float xyscale)
    : modifier_(modifier),
      xyscale_(xyscale) {
    Image heightmap;
    heightmap.loadPng(heightmap_filename);

    width_ = heightmap.width();
    height_ = heightmap.height();
    heightmap_.resize(width_);

    for (int i = 0; i < width_; ++i) {
        heightmap_[i].resize(height_);
        for (int j = 0; j < height_; ++j)
            heightmap_[i][j] = heightmap(i, j, 0);
    }
}

void Terrain::render(const Renderer& renderer, mat4 transformation) const {
    mat4 stack = transformation * transformation_;
    renderer.render(*this, stack);
    for (auto child : children_)
        child->render(renderer, stack);
}

int Terrain::width() const {
    return width_;
}

int Terrain::height() const {
    return height_;
}
