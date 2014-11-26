#include <vector>
#include <glm/gtc/type_ptr.hpp>
#include "glerror.hh"
#include "spirit_renderer.hh"

using std::vector;
using glm::mat4;

SpiritRenderer::SpiritRenderer(
    const Game& game,
    const ShaderProgram& program
) : game_(game),
    program_(program),
    vert_arr_(program),
    lod_(20) {

    vector<GLfloat> positions(lod_ * 3);

    for(int i = 0; i < lod_; ++i) {
        positions[i*3] = cos(i * 2 * M_PI / lod_);
        positions[i*3 + 1] = sin(i * 2 * M_PI / lod_);
        positions[i*3 + 2] = 0.0;
    }

    vert_arr_.addBuffer(positions, "position", 3);
}

void SpiritRenderer::render(const Spirit&, const mat4& mvp) const {
    program_.use();
    vert_arr_.bind();

    program_.uniformMat4("mvp", glm::value_ptr(mvp));
    glDrawArrays(GL_LINE_LOOP, 0, lod_);
    checkGlError();
}
