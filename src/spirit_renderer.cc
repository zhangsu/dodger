#include <glm/gtc/type_ptr.hpp>
#include "glerror.hh"
#include "spirit_renderer.hh"

using glm::mat4;

SpiritRenderer::SpiritRenderer(
    const Game& game,
    const ShaderProgram& program
) : game_(game),
    program_(program),
    lod_(20) {

    GLfloat positions[lod_ * 3];

    for(int i = 0; i < lod_; ++i) {
        positions[i*3] = cos(i * 2 * M_PI / lod_);
        positions[i*3 + 1] = sin(i * 2 * M_PI / lod_);
        positions[i*3 + 2] = 0.0;
    }

    program_.use();

    glGenVertexArrays(1, &vert_arr_);
    checkGlError();
    glBindVertexArray(vert_arr_);
    checkGlError();

    GLuint buf;
    glGenBuffers(1, &buf);
    checkGlError();
    glBindBuffer(GL_ARRAY_BUFFER, buf);
    checkGlError();
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
    checkGlError();

    program_.enableVertexAttribArray("position");
    program_.vertexAttribPointer("position", 3, GL_FLOAT, GL_FALSE, 0, nullptr);
}

void SpiritRenderer::render(const Spirit&, const mat4& mvp) const {
    program_.use();
    glBindVertexArray(vert_arr_);
    checkGlError();

    program_.uniformMat4("mvp", glm::value_ptr(mvp));
    glDrawArrays(GL_LINE_LOOP, 0, lod_);
    checkGlError();
}
