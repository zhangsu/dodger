#include <vector>
#include "glerror.hh"
#include "sky_renderer.hh"

using std::vector;

SkyRenderer::SkyRenderer(const Game& game, const ShaderProgram& program)
    : game_(game),
      program_(program),
      texture_("data/images/sky.png"),
      vertex_array_(program) {

    vector<GLfloat> positions = {
        0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,

        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f,

        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 0.0f,

        0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,

        0.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,
    };

    vertex_array_.addAttribute(positions, "position", 3);
    vertex_array_.set_count(positions.size() / 3);
}

void SkyRenderer::render(const Sky&, const glm::mat4& mv,
                         const glm::mat4& p) const {
    vertex_array_.bind();
    program_.use();

    texture_.activateAndBind(GL_TEXTURE0);
    program_.uniform1i("sampler", 0);
    program_.uniformMat4("mvp", p * mv);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, vertex_array_.count());
    checkGlError();
}
