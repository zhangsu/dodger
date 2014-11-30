#include <cstdlib>
#include <ctime>
#include <cstring>
#include <vector>
#include "glerror.hh"
#include "spirit_renderer.hh"

using std::vector;
using glm::mat4;
using glm::vec3;
using glm::vec4;

namespace {

void copy_vec(GLfloat dest[3], GLfloat src[3]) {
    memcpy(dest, src, sizeof (GLfloat) * 3);
}

}

SpiritRenderer::SpiritRenderer(const Game& game)
    : game_(game),
      program_("src/spirit.vert", "src/spirit.frag"),
      lod_(5),
      vertex_array_(program_),
      metrics_(),
      colors_() {
    srand(time(nullptr));

    GLfloat positions[lod_ + 1][lod_][3];

    for (int i = 0; i < lod_ + 1; ++i) {
        for (int j = 0; j < lod_; ++j) {
            positions[i][j][0] = cos(j * 2 * M_PI / lod_)
                               * sin(i * M_PI / lod_);
            positions[i][j][1] = sin(j * 2 * M_PI / lod_)
                               * sin(i * M_PI / lod_);
            positions[i][j][2] = cos(i * M_PI / lod_);
        }
    }

    GLfloat sphere_positions[lod_][lod_ * 2 * 3][3];
    memset(sphere_positions, 0, sizeof (sphere_positions));
    for (int i = 0; i < lod_; ++i) {
        for (int j = 0; j < lod_; ++j) {
            copy_vec(sphere_positions[i][j * 2 * 3],
                     positions[i][j]);
            copy_vec(sphere_positions[i][j * 2 * 3 + 1],
                     positions[i + 1][j]);
            copy_vec(sphere_positions[i][j * 2 * 3 + 2],
                     positions[i][j + 1]);

            copy_vec(sphere_positions[i][(j * 2 + 1) * 3],
                     positions[i][j + 1]);
            copy_vec(sphere_positions[i][(j * 2 + 1) * 3 + 1],
                     positions[i + 1][j]);
            copy_vec(sphere_positions[i][(j * 2 + 1) * 3 + 2],
                     positions[i + 1][j + 1]);
        }
    }

    size_t len = sizeof (sphere_positions);
    size_t vertex_count = len / 3 / sizeof (GLfloat);
    size_t attr_len = MAX_PARTICLE_COUNT * 4 * sizeof (GLfloat);
    vertex_array_.addAttribute((GLfloat*) sphere_positions, len, "position", 3);
    vertex_array_.addStreamAttribute(attr_len, "metric", 4);
    vertex_array_.addStreamAttribute(attr_len, "color", 4);
    // Particle attributes cycles for each sphere, so the divisor should be
    // the number of triangles for the sphere minus 1.
    vertex_array_.setAttributeDivisor("metric", 1);
    vertex_array_.setAttributeDivisor("color", 1);
    vertex_array_.set_count(vertex_count);
}

void SpiritRenderer::render(const Spirit& spirit, const mat4& mv,
                            const mat4& p) {
    static int frame = 0;
    frame++;
    if (frame % 2 == 0)
        updateParticles(spirit);

    vertex_array_.bind();

    program_.use();
    // Render particles in spirit's parent frame to create a particle track.
    program_.uniformMat4("mvp", p * mv * glm::inverse(spirit.trans()));
    size_t len = 4 * MAX_PARTICLE_COUNT * sizeof (GLfloat);
    vertex_array_.stream("metric", metrics(), len);
    vertex_array_.stream("color", colors(), len);
    glDrawArraysInstanced(GL_TRIANGLES, 0, vertex_array_.count(),
                          MAX_PARTICLE_COUNT);
    checkGlError();
}
#include <cstdio>
void SpiritRenderer::updateParticles(const Spirit& spirit) {
    // Update existing particles and clean up dead particles.
    for (size_t i = 0; i < MAX_PARTICLE_COUNT; ++i) {
        if (!isParticleDead(i)) {
            metrics_[i][0] += random() * 0.6f - 0.3f;
            metrics_[i][1] += random() * 0.6f - 0.3f;
            metrics_[i][2] += random() * 0.6f - 0.3f;
            // Become smaller.
            metrics_[i][3] *= 0.9;
            // Become more transparent.
            colors_[i][3] *= random() * 0.1f + 0.9f;
            if (isParticleDead(i)) {
                memset(metrics_[i], 0, 4 * sizeof (GLfloat));
                memset(colors_[i], 0, 4 * sizeof (GLfloat));
            }
        }
    }

    vec4 spirit_pos = spirit.trans() * vec4(0.0f, 0.0f, 0.0f, 1.0f);
    // Generate new particles.
    for (size_t i = 0; i < MAX_PARTICLE_COUNT; ++i) {
        if (isParticleDead(i)) {
            // Generate particles at this spirit's location in parent frame.
            vec4 metric = spirit_pos;
            // Here w is used as the scale of the particle.
            metric.w = 0.1f;
            genParticle(i, metric,
                        vec4(random() * 0.5f,
                             random() * 0.5f,
                             random() * 0.5f + 0.5f,
                             1.0f));
        }
    }
}

void SpiritRenderer::genParticle(int index, vec4 metric, vec4 color) {
    for (int i = 0; i < 4; ++i)
        metrics_[index][i] = metric[i];
    for (int i = 0; i < 4; ++i)
        colors_[index][i] = color[i];
}

bool SpiritRenderer::isParticleDead(int index) const {
    // It's dead, Jim! Too transparent.
    return colors_[index][3] < 0.1f;
}

GLfloat* SpiritRenderer::metrics() const {
    return (GLfloat*) metrics_;
}

GLfloat* SpiritRenderer::colors() const {
    return (GLfloat*) colors_;
}

float SpiritRenderer::random() const {
    return ((float) rand()) / RAND_MAX;
}
