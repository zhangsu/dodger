#include <string>
#include <vector>
#include "image.hh"
#include "glerror.hh"
#include "terrain_renderer.hh"

using std::string;
using std::to_string;
using std::vector;
using glm::mat4;
using glm::vec3;

// Public methods.

TerrainRenderer::TerrainRenderer(const Game& game)
    : program_("src/terrain.vert", "src/terrain.frag"),
      game_(game),
      grass_texture_("data/images/grass_texture.png"),
      rock_texture_("data/images/mountain_texture.png"),
      initialized(false),
      vertex_array_(program_) {}

void TerrainRenderer::render(const Terrain& terrain, const mat4& mv,
                             const mat4& p) const {
    if (!initialized) {
        genVertices(terrain);
        initialized = true;
    }

    vertex_array_.bind();
    program_.use();

    for (unsigned i = 0; i < game_.lights().size(); ++i) {
        const Light* light = game_.lights()[i];
        string prefix = string("lights[") + to_string(i) + "].";
        program_.uniformVec3(prefix + "position",
                             light->position(game_.camera()));
        program_.uniformVec3(prefix + "color", light->color());
        program_.uniformVec3(prefix + "attenuation", light->attenuation());
    }

    const Material& material = terrain.material();
    program_.uniform1i("light_count", game_.lights().size());
    program_.uniformVec3("ambient", game_.ambient() * material.diffuse());
    program_.uniformVec3("diffuse", material.diffuse());
    program_.uniformVec3("specular", material.specular());
    program_.uniform1f("shininess", material.shininess());

    grass_texture_.activateAndBind(GL_TEXTURE0);
    rock_texture_.activateAndBind(GL_TEXTURE1);
    program_.uniform1i("grass_sampler", 0);
    program_.uniform1i("rock_sampler", 1);

    program_.uniformMat4("mv", mv);
    program_.uniformMat4("mvp", p * mv);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, vertex_array_.count());
    checkGlError();
}

// Private methods.

void TerrainRenderer::genVertices(const Terrain& terrain) const {
    vector<GLfloat> positions;
    vector<GLfloat> normals;

    // Triangle strip positions from top to bottom.
    for (int z = 0; z < terrain.height(); z += 2) {
        // Triangle strip positions from left to right.
        int x;
        for (x = 0; x < terrain.width(); x += 2) {
            addPosition(terrain, positions, x, z);
            addPosition(terrain, positions, x, z + 1);
            addPosition(terrain, positions, x + 1, z);
            addPosition(terrain, positions, x + 1, z + 1);
            addNormal(terrain, normals, x, z);
            addNormal(terrain, normals, x, z + 1);
            addNormal(terrain, normals, x + 1, z);
            addNormal(terrain, normals, x + 1, z + 1);
        }
        if (z + 2 >= terrain.width()) {
            // Last row may not need the reverse direction.
            break;
        }
        // Repeat last position of the above row twice to avoid backfacing.
        x -= 2;
        for (int k = 0; k < 1; ++k) {
            addPosition(terrain, positions, x + 1, z + 1);
            addNormal(terrain, normals, x + 1, z + 1);
        }
        // Triangle strip positions from right to left.
        for (x = terrain.width() - 1; x >= 0; x -= 2) {
            addPosition(terrain, positions, x, z + 1);
            addPosition(terrain, positions, x, z + 2);
            addPosition(terrain, positions, x - 1, z + 1);
            addPosition(terrain, positions, x - 1, z + 2);
            addNormal(terrain, normals, x, z + 1);
            addNormal(terrain, normals, x, z + 2);
            addNormal(terrain, normals, x - 1, z + 1);
            addNormal(terrain, normals, x - 1, z + 2);
        }
        // Repeat last position of the above row twice to avoid backfacing.
        x += 2;
        for (int k = 0; k < 1; ++k) {
            addPosition(terrain, positions, x - 1, z + 2);
            addNormal(terrain, normals, x - 1, z + 2);
        }
    }

    vertex_array_.addAttribute(positions, "position", 3);
    vertex_array_.addAttribute(normals, "normal", 3);
    vertex_array_.set_count(positions.size() / 3);
}

#include <cstdio>
void TerrainRenderer::addPosition(const Terrain& terrain,
                                  vector<GLfloat>& positions,
                                  int x, int z) const {
    positions.push_back(x);
    positions.push_back(terrain[z][x]);
    positions.push_back(z);

}

void TerrainRenderer::addNormal(const Terrain& terrain,
                                vector<GLfloat>& normals,
                                int x, int z) const {
    vec3 center = vec3(x, terrain[z][x], z);
    vec3 zero;
    vec3 neighbors[] = {
        z > 0 ? (vec3(x, terrain[z - 1][x], z - 1) - center) : zero,
        x > 0 ? (vec3(x - 1, terrain[z][x - 1], z) - center) : zero,
        z < terrain.height() - 1 ? (vec3(x, terrain[z + 1][x], z + 1) - center)
                                 : zero,
        x < terrain.width() - 1 ? (vec3(x + 1, terrain[z][x + 1], z) - center)
                                : zero
    };
    // Vertex normal is the sum of neighboring surface normals.
    vec3 normal;
    for (int i = 0; i < 4; ++i)
        normal += glm::cross(neighbors[i], neighbors[(i + 1) % 4]);
    normals.push_back(normal.x);
    normals.push_back(normal.y);
    normals.push_back(normal.z);
}
