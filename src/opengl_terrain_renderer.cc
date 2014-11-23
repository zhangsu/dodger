#include <glm/gtc/type_ptr.hpp>
#include "glerror.hh"
#include "opengl_terrain_renderer.hh"

using glm::mat4;

OpenGLTerrainRenderer::OpenGLTerrainRenderer(
    const Game& game,
    const ShaderProgram& program
) : game_(game),
    program_(program) {

    GLfloat vertices[] = {
       -1.0f, -1.0f, 0.0f,
       1.0f, -1.0f, 0.0f,
       0.0f,  1.0f, 0.0f,
    };

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
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    checkGlError();

    program_.enableVertexAttribArray("vert");
    program_.vertexAttribPointer("vert", 3, GL_FLOAT, GL_FALSE, 0, nullptr);
}

void OpenGLTerrainRenderer::render(const Terrain&, const mat4& mvp) const {
    program_.use();
    glBindVertexArray(vao_);
    checkGlError();

    program_.uniformMat4("mvp", glm::value_ptr(mvp));
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
