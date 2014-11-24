#include <glm/gtc/type_ptr.hpp>
#include "glerror.hh"
#include "opengl_spirit_renderer.hh"

using glm::mat4;

OpenGLSpiritRenderer::OpenGLSpiritRenderer(
    const Game& game,
    const ShaderProgram& program
) : game_(game),
    program_(program),
    lod_(20) {

    GLfloat vertices[lod_ * 3];

    for(int i = 0; i < lod_; ++i) {
        vertices[i*3] = cos(i * 2 * M_PI / lod_);
        vertices[i*3 + 1] = sin(i * 2 * M_PI / lod_);
        vertices[i*3 + 2] = 0.0;
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    checkGlError();

    program_.enableVertexAttribArray("vert");
    program_.vertexAttribPointer("vert", 3, GL_FLOAT, GL_FALSE, 0, nullptr);
}

void OpenGLSpiritRenderer::render(const Spirit&, const mat4& mvp) const {
    program_.use();
    glBindVertexArray(vao_);
    checkGlError();

    program_.uniformMat4("mvp", glm::value_ptr(mvp));
    glDrawArrays(GL_LINE_LOOP, 0, lod_);
}
