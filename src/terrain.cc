#include <string>
#include "image.hh"
#include "material.hh"
#include "renderer.hh"
#include "terrain.hh"

using std::string;
using std::vector;
using glm::mat4;
using glm::vec3;
using glm::vec4;

// Public methods.

Terrain::Terrain(string heightmap_filename)
    : Primitive(Material(vec3(0.9f, 1.0f, 0.9f), vec3(0, 0, 0), 0)) {

    Image heightmap;
    heightmap.loadPng(heightmap_filename);
    width_ = heightmap.width();
    height_ = heightmap.height();
    // Only use an even heightmap dimension.
    if (width_ % 2 == 1)
        width_--;
    if (height_ % 2 == 1)
        height_--;
    heightmap_.resize(height_);

    for (int z = 0; z < height_; ++z) {
        heightmap_[z].resize(width_);
        for (int x = 0; x < width_; ++x) {
            heightmap_[z][x] = heightmap(x, z, 0);
        }
    }
}

void Terrain::render(const Renderer& renderer, mat4 trans) const {
    mat4 stack = trans * this->trans();
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

float Terrain::height(float x, float z) const {
    if (x < 0 || z < 0 || x >= width() - 1 || z >= height() - 1)
        return 0;

    int x0 = x;
    int z0 = z;
    int x1 = x0 + 1;
    int z1 = z0 + 1;

    float y0 = (*this)[z0][x0];
    float y1 = (*this)[z0][x1];
    float y2 = (*this)[z1][x0];
    float y3 = (*this)[z1][x1];

    // Linear interpolation on the terrain triangles.
    float dx = x - x0;
    float dz = z - z0;

    float y;
    if (dx + dz < 1)
        y = y0 + (y1 - y0) * dx + (y2 - y0) * dz;
    else
        y = y3 + (y1 - y3) * (1 - dz) + (y2 - y3) * (1 - dx);
    return (trans_ * vec4(x, y, z, 1)).y;
}
