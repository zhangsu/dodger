#include <vector>
#include <glm/gtc/type_ptr.hpp>
#include "image.hh"
#include "glerror.hh"
#include "terrain_renderer.hh"

using std::vector;
using glm::mat4;
using glm::vec3;

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
    for (int x = 0; x < terrain.width(); x += 2) {
        // Triangle strip positions from left to right.
        int z;
        for (z = 0; z < terrain.height(); z += 2) {
            addPosition(terrain, positions, x + 1, z);
            addPosition(terrain, positions, x, z);
            addPosition(terrain, positions, x + 1, z + 1);
            addPosition(terrain, positions, x, z + 1);
            addNormal(terrain, normals, x + 1, z);
            addNormal(terrain, normals, x, z);
            addNormal(terrain, normals, x + 1, z + 1);
            addNormal(terrain, normals, x, z + 1);
        }
        if (x + 2 >= terrain.width()) {
            // Last row may not need the reverse direction.
            break;
        }
        // Repeat last position of the above row twice to avoid backfacing.
        z -= 2;
        for (int k = 0; k < 2; ++k) {
            addPosition(terrain, positions, x, z + 1);
            addNormal(terrain, normals, x, z + 1);
        }
        // Triangle strip positions from right to left.
        for (z = terrain.width() - 1; z >= 0; z -= 2) {
            addPosition(terrain, positions, x + 1, z);
            addPosition(terrain, positions, x + 2, z);
            addPosition(terrain, positions, x + 1, z - 1);
            addPosition(terrain, positions, x + 2, z - 1);
            addNormal(terrain, normals, x + 1, z);
            addNormal(terrain, normals, x + 2, z);
            addNormal(terrain, normals, x + 1, z - 1);
            addNormal(terrain, normals, x + 2, z - 1);
        }
        // Repeat last position of the above row twice to avoid backfacing.
        z += 2;
        for (int k = 0; k < 2; ++k) {
            addPosition(terrain, positions, x + 2, z - 1);
            addNormal(terrain, normals, x + 2, z - 1);
        }
    }

    vertex_array_.addAttribute(positions, "position", 3);
    vertex_array_.addAttribute(normals, "normal", 3);
    vertex_count_ = positions.size() / 3;
}

void TerrainRenderer::addPosition(const Terrain& terrain,
                                  vector<GLfloat>& positions,
                                  int x, int z) const {
    positions.push_back(x);
    positions.push_back(terrain[x][z]);
    positions.push_back(z);
}

void TerrainRenderer::addNormal(const Terrain& terrain,
                                vector<GLfloat>& normals,
                                int x, int z) const {
    vec3 center = vec3(x, terrain[x][z], z);
    vec3 zero;
    vec3 neighbors[] = {
        z > 0 ? (vec3(x, terrain[x][z - 1], z - 1) - center) : zero,
        x > 0 ? (vec3(x - 1, terrain[x - 1][z], z) - center) : zero,
        z < terrain.height() - 1 ? (vec3(x, terrain[x][z + 1], z + 1) - center)
                                 : zero,
        x < terrain.width() - 1 ? (vec3(x + 1, terrain[x + 1][z], z) - center)
                                : zero
    };
    vec3 normal;
    for (int i = 0; i < 4; ++i)
        normal += glm::cross(neighbors[i], neighbors[(i + 1) % 4]);
    normals.push_back(normal.x);
    normals.push_back(normal.y);
    normals.push_back(normal.z);
}
