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
    vert_arr_(program),
    texture_("data/images/grass_texture.png"),
    initialized(false) {}

void TerrainRenderer::render(const Terrain& terrain, const mat4& mvp) const {
    if (!initialized) {
        initVertices(terrain);
        initialized = true;
    }

    vert_arr_.bind();
    texture_.activateAndBind(GL_TEXTURE0 + 0);

    program_.use();
    program_.uniform1i("sampler", 0);
    program_.uniformMat4("mvp", glm::value_ptr(mvp));
    glDrawArrays(GL_TRIANGLE_STRIP, 0, vert_count_);
    checkGlError();
}

// Private methods.

void TerrainRenderer::initVertices(const Terrain& terrain) const {
    vector<GLfloat> positions;

    // Triangle strip positions from top to bottom.
    for (int i = 0; i < terrain.height(); i += 2) {
        // Triangle strip positions from left to right.
        int j;
        for (j = 0; j < terrain.width(); j += 2) {
            positions.push_back(i + 1);
            positions.push_back(terrain[i + 1][j]);
            positions.push_back(j);

            positions.push_back(i);
            positions.push_back(terrain[i][j]);
            positions.push_back(j);

            positions.push_back(i + 1);
            positions.push_back(terrain[i + 1][j + 1]);
            positions.push_back(j + 1);

            positions.push_back(i);
            positions.push_back(terrain[i][j + 1]);
            positions.push_back(j + 1);
        }
        if (i + 2 >= terrain.width()) {
            // Last row may not need the reverse direction.
            break;
        }
        // Repeat last position of the above row twice to avoid backfacing.
        j -= 2;
        for (int k = 0; k < 2; ++k) {
            positions.push_back(i);
            positions.push_back(terrain[i][j + 1]);
            positions.push_back(j + 1);
        }
        // Triangle strip positions from right to left.
        for (j = terrain.width() - 1; j >= 0; j -= 2) {
            positions.push_back(i + 1);
            positions.push_back(terrain[i + 1][j]);
            positions.push_back(j);

            positions.push_back(i + 2);
            positions.push_back(terrain[i + 2][j]);
            positions.push_back(j);

            positions.push_back(i + 1);
            positions.push_back(terrain[i + 1][j - 1]);
            positions.push_back(j - 1);

            positions.push_back(i + 2);
            positions.push_back(terrain[i + 2][j - 1]);
            positions.push_back(j - 1);
        }
        // Repeat last position of the above row twice to avoid backfacing.
        j += 2;
        for (int k = 0; k < 2; ++k) {
            positions.push_back(i + 2);
            positions.push_back(terrain[i + 2][j - 1]);
            positions.push_back(j - 1);
        }
    }

    vert_arr_.addBuffer(positions, "position", 3);
    vert_count_ = positions.size() / 3;
}
