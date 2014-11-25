#include <vector>
#include <glm/gtc/type_ptr.hpp>
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
    if (!initialized) {
        vertex_count_ = init(terrain);
        initialized = true;
    }

    program_.use();
    glBindVertexArray(vao_);
    checkGlError();

    program_.uniformMat4("mvp", glm::value_ptr(mvp));
    glDrawArrays(GL_TRIANGLE_STRIP, 0, vertex_count_);
}

// Private methods.

size_t OpenGLTerrainRenderer::init(const Terrain& terrain) const {
    vector<GLfloat> vertices;

    // Triangle strip vertices from top to bottom.
    for (int i = 0; i < terrain.height(); i += 2) {
        // Triangle strip vertices from left to right.
        int j;
        for (j = 0; j < terrain.width(); j += 2) {
            vertices.push_back(i + 1);
            vertices.push_back(terrain[i + 1][j]);
            vertices.push_back(j);

            vertices.push_back(i);
            vertices.push_back(terrain[i][j]);
            vertices.push_back(j);

            vertices.push_back(i + 1);
            vertices.push_back(terrain[i + 1][j + 1]);
            vertices.push_back(j + 1);

            vertices.push_back(i);
            vertices.push_back(terrain[i][j + 1]);
            vertices.push_back(j + 1);
        }
        if (i + 2 >= terrain.width()) {
            // Last row may not need the reverse direction.
            break;
        }
        // Repeat last vertex of the above row twice to avoid backfacing face.
        j -= 2;
        for (int k = 0; k < 2; ++k) {
            vertices.push_back(i);
            vertices.push_back(terrain[i][j + 1]);
            vertices.push_back(j + 1);
        }
        // Triangle strip vertices from right to left.
        for (j = terrain.width() - 1; j >= 0; j -= 2) {
            vertices.push_back(i + 1);
            vertices.push_back(terrain[i + 1][j]);
            vertices.push_back(j);

            vertices.push_back(i + 2);
            vertices.push_back(terrain[i + 2][j]);
            vertices.push_back(j);

            vertices.push_back(i + 1);
            vertices.push_back(terrain[i + 1][j - 1]);
            vertices.push_back(j - 1);

            vertices.push_back(i + 2);
            vertices.push_back(terrain[i + 2][j - 1]);
            vertices.push_back(j - 1);
        }
        // Repeat last vertex of the above row twice to avoid backfacing face.
        j += 2;
        for (int k = 0; k < 2; ++k) {
            vertices.push_back(i + 2);
            vertices.push_back(terrain[i + 2][j - 1]);
            vertices.push_back(j - 1);
        }
    }

    program_.use();

    glGenVertexArrays(1, &vao_);
    checkGlError();
    glBindVertexArray(vao_);
    checkGlError();

    GLuint vbo;
    glGenBuffers(1, &vbo);
    checkGlError();
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    checkGlError();
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof (GLfloat),
                 &vertices[0], GL_STATIC_DRAW);
    checkGlError();

    program_.enableVertexAttribArray("vert");
    program_.vertexAttribPointer("vert", 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    return vertices.size() / 3;
}
