#include <string>
#include "image.hh"
#include "material.hh"
#include "renderer.hh"
#include "terrain.hh"

using std::string;
using std::vector;
using glm::mat4;
using glm::vec3;

// Public methods.

Terrain::Terrain(string heightmap_filename)
    : Primitive(Material(vec3(0.9f, 1.0f, 0.9f), vec3(0, 0, 0), 0)) {

    Image heightmap;
    heightmap.loadPng(heightmap_filename);
    width_ = heightmap.width();
    height_ = heightmap.height();
    // Only use a even heightmap dimension.
    if (width_ % 2 == 1)
        width_--;
    if (height_ % 2 == 1)
        height_--;
    heightmap_.resize(width_);

    for (int i = 0; i < width_; ++i) {
        heightmap_[i].resize(height_);
        for (int j = 0; j < height_; ++j)
            heightmap_[i][j] = heightmap(i, j, 0);
    }
}

void Terrain::render(const Renderer& renderer, mat4 trans) const {
    mat4 stack = trans * transformation();
    renderer.render(*this, stack);
    renderChildren(renderer, stack);
}

int Terrain::width() const {
    return width_;
}

int Terrain::height() const {
    return height_;
}

const vector<float>& Terrain::operator [](int index) const {
    return heightmap_[index];
}
