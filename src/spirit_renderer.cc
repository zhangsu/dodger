#include <vector>
#include "glerror.hh"
#include "spirit_renderer.hh"

using std::vector;
using glm::mat4;

SpiritRenderer::SpiritRenderer(
    const Game& game,
    const ShaderProgram& program
) : game_(game),
    program_(program),
    lod_(20),
    vertex_array_(program) {

    vector<GLfloat> positions(lod_ * 3);

    for(int i = 0; i < lod_; ++i) {
        positions[i*3] = cos(i * 2 * M_PI / lod_);
        positions[i*3 + 1] = sin(i * 2 * M_PI / lod_);
        positions[i*3 + 2] = 0.0;
    }

    vertex_array_.addAttribute(positions, "position", 3);
    vertex_array_.set_count(lod_);
}

void SpiritRenderer::render(const Spirit&, const mat4& mv,
                            const mat4& p) const {
    vertex_array_.bind();

    program_.use();
    program_.uniformMat4("mvp", p * mv);
    glDrawArrays(GL_LINE_LOOP, 0, vertex_array_.count());
    checkGlError();
}
