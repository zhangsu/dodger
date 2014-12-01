#include <vector>
#include "error.hh"
#include "sky_renderer.hh"

using std::vector;

SkyRenderer::SkyRenderer(const Game& game)
    : game_(game),
      program_("src/texture.vert", "src/texture.frag"),
      texture_("data/images/sky.png", true),
      vertex_array_(program_) {

    vector<GLfloat> positions = {
        // Front face.
        0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        // Right face.
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 1.0f,
        // Bottom face.
        0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f,
        // Left face.
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 0.0f,
        // Top face.
        0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 0.0f,
        // Back face.
        1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
    };
    float one_third = 1.0f / 3.0f;
    float two_third = 2.0f / 3.0f;
    vector<GLfloat> uvs = {
        // Front face.
        0.25f, one_third,
        0.50f, one_third,
        0.25f, two_third,
        0.50f, two_third,
        0.25f, two_third,
        0.50f, one_third,
        // Right face.
        0.50f, one_third,
        0.75f, one_third,
        0.50f, two_third,
        0.75f, two_third,
        0.50f, two_third,
        0.75f, one_third,
        // Bottom face.
        0.25f, 0,
        0.50f, 0,
        0.25f, one_third,
        0.50f, one_third,
        0.25f, one_third,
        0.50f, 0,
        // Left face.
        0.00f, one_third,
        0.25f, one_third,
        0.00f, two_third,
        0.25f, two_third,
        0.00f, two_third,
        0.25f, one_third,
        // Top face.
        0.25f, two_third,
        0.50f, two_third,
        0.25f, 1,
        0.50f, 1,
        0.25f, 1,
        0.50f, two_third,
        // Back face.
        0.75f, one_third,
        1.00f, one_third,
        0.75f, two_third,
        1.00f, two_third,
        0.75f, two_third,
        1.00f, one_third,
    };

    vertex_array_.addAttribute(positions, "position", 3);
    vertex_array_.addAttribute(uvs, "tex_uv", 2);
    vertex_array_.set_count(positions.size() / 3);
}

void SkyRenderer::render(const Sky&, const glm::mat4& mv,
                         const glm::mat4& p) const {
    vertex_array_.bind();
    program_.use();

    texture_.activateAndBind(GL_TEXTURE0);
    program_.uniform1i("sampler", 0);
    program_.uniformMat4("mvp", p * mv);

    glDrawArrays(GL_TRIANGLES, 0, vertex_array_.count());
    checkGlError();
}
