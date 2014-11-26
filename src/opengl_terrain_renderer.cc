#include <vector>
#include <glm/gtc/type_ptr.hpp>
#include "image.hh"
#include "glerror.hh"
#include "opengl_terrain_renderer.hh"

using std::vector;
using glm::mat4;

// Public methods.

OpenGLTerrainRenderer::OpenGLTerrainRenderer(
    const Game& game,
    const ShaderProgram& program
) : game_(game),
    program_(program),
    initialized(false) {}

void OpenGLTerrainRenderer::render(const Terrain& terrain,
                                   const mat4& mvp) const {
    program_.use();

    if (!initialized) {
        initVertices(terrain);
        initTexture();
        initialized = true;
    }

    glBindVertexArray(vert_arr_);
    checkGlError();
    glActiveTexture(GL_TEXTURE0);
    checkGlError();
    glBindTexture(GL_TEXTURE_2D, texture_);
    checkGlError();

    program_.uniformMat4("mvp", glm::value_ptr(mvp));
    program_.uniform1i("sampler", 0);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, vertex_count_);
    checkGlError();
}

// Private methods.

void OpenGLTerrainRenderer::initVertices(const Terrain& terrain) const {
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

    glGenVertexArrays(1, &vert_arr_);
    checkGlError();
    glBindVertexArray(vert_arr_);
    checkGlError();

    GLuint buf;
    glGenBuffers(1, &buf);
    checkGlError();
    glBindBuffer(GL_ARRAY_BUFFER, buf);
    checkGlError();
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof (GLfloat),
                 &positions[0], GL_STATIC_DRAW);
    checkGlError();

    program_.enableVertexAttribArray("position");
    program_.vertexAttribPointer("position", 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    vertex_count_ = positions.size() / 3;
}

void OpenGLTerrainRenderer::initTexture() const {
    glGenTextures(1, &texture_);
    checkGlError();
    glBindTexture(GL_TEXTURE_2D, texture_);
    checkGlError();

    Image texture_map;
    texture_map.loadPng("data/images/grass_texture.png");
    size_t len = texture_map.width() * texture_map.height()
                 * texture_map.elements();
    vector<float> data(len);
    for (size_t i = 0; i < len; ++i)
        data[i] = texture_map.data()[i];
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_map.width(),
                 texture_map.height(), 0, GL_RGB, GL_FLOAT, &data[0]);
    checkGlError();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    checkGlError();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    checkGlError();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    checkGlError();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    checkGlError();
    glGenerateMipmap(GL_TEXTURE_2D);
    checkGlError();
}
