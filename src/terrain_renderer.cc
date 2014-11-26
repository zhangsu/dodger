#include <vector>
#include <glm/gtc/type_ptr.hpp>
#include "image.hh"
#include "glerror.hh"
#include "terrain_renderer.hh"

using std::vector;
using glm::mat4;

// Public methods.

TerrainRenderer::TerrainRenderer(
    const Game& game,
    const ShaderProgram& program
) : game_(game),
    program_(program),
    vertex_array_(program),
    texture_("data/images/grass_texture.png"),
    initialized(false) {}

void TerrainRenderer::render(const Terrain& terrain, const mat4& mvp) const {
    if (!initialized) {
        genVertices(terrain);
        initialized = true;
    }

    vertex_array_.bind();
    texture_.activateAndBind(GL_TEXTURE0 + 0);

    program_.use();
    program_.uniform1i("sampler", 0);
    program_.uniformMat4("mvp", glm::value_ptr(mvp));
    glDrawArrays(GL_TRIANGLE_STRIP, 0, vertex_count_);
    checkGlError();
}

// Private methods.

void TerrainRenderer::genVertices(const Terrain& terrain) const {
    vector<GLfloat> positions;
    vector<GLfloat> normals;

    // Triangle strip positions from top to bottom.
    for (int i = 0; i < terrain.height(); i += 2) {
        // Triangle strip positions from left to right.
        int j;
        for (j = 0; j < terrain.width(); j += 2) {
            addPosition(terrain, positions, i + 1, j);
            addPosition(terrain, positions, i, j);
            addPosition(terrain, positions, i + 1, j + 1);
            addPosition(terrain, positions, i, j + 1);
        }
        if (i + 2 >= terrain.width()) {
            // Last row may not need the reverse direction.
            break;
        }
        // Repeat last position of the above row twice to avoid backfacing.
        j -= 2;
        for (int k = 0; k < 2; ++k)
            addPosition(terrain, positions, i, j + 1);
        // Triangle strip positions from right to left.
        for (j = terrain.width() - 1; j >= 0; j -= 2) {
            addPosition(terrain, positions, i + 1, j);
            addPosition(terrain, positions, i + 2, j);
            addPosition(terrain, positions, i + 1, j - 1);
            addPosition(terrain, positions, i + 2, j - 1);
        }
        // Repeat last position of the above row twice to avoid backfacing.
        j += 2;
        for (int k = 0; k < 2; ++k)
            addPosition(terrain, positions, i + 2, j - 1);
    }

    vertex_array_.addBuffer(positions, "position", 3);
    vertex_count_ = positions.size() / 3;
}

void TerrainRenderer::addPosition(const Terrain& terrain,
                                  vector<GLfloat>& positions,
                                  int i, int j) const {
    positions.push_back(i);
    positions.push_back(terrain[i][j]);
    positions.push_back(j);
}
