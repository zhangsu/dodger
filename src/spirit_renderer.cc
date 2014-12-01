#include <cstdlib>
#include <ctime>
#include <cstring>
#include <vector>
#include <glm/gtc/type_ptr.hpp>
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
      shadow_mapper_("src/spirit_shadow_map.vert", "src/shadow_map.frag"),
      lod_(5),
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
    size_t stream_buf_len = MAX_PARTICLE_COUNT * 4 * sizeof (GLfloat);
    for (unsigned i = 0; i < MAX_SPIRIT_COUNT; ++i) {
        vertex_arrays_[i].set_program(program_);
        vertex_arrays_[i].addAttribute((GLfloat*) sphere_positions, len,
                                       "position", 3);
        vertex_arrays_[i].addStreamAttribute(stream_buf_len, "metric", 4);
        vertex_arrays_[i].addStreamAttribute(stream_buf_len, "color", 4);
        // Particle attributes cycles for each sphere, so the divisor should be
        // the number of triangles for the sphere minus 1.
        vertex_arrays_[i].setAttributeDivisor("metric", 1);
        vertex_arrays_[i].setAttributeDivisor("color", 1);
        vertex_arrays_[i].set_count(vertex_count);
    }
}

void SpiritRenderer::renderShadow(const Spirit& spirit, const mat4& mv,
                                  const mat4& p) {
    // Reduce the frame rate of particles by a half.
    static int frame = 0;
    frame++;
    if (frame % 2 == 0)
        updateParticles(spirit);

    vertex_arrays_[spirit.id()].bind();
    shadow_mapper_.use();
    // Render particles in spirit's parent frame to create a particle track.
    shadow_mapper_.uniformMat4("mvp", p * mv * glm::inverse(spirit.trans()));
    glDrawArraysInstanced(GL_TRIANGLES, 0, vertex_arrays_[spirit.id()].count(),
                          MAX_PARTICLE_COUNT);
    checkGlError();
}

#include <cstdio>
void SpiritRenderer::render(const Spirit& spirit, const mat4& mv,
                            const mat4& p) {
    vertex_arrays_[spirit.id()].bind();
    program_.use();
    // Render particles in spirit's parent frame to create a particle track.
    program_.uniformMat4("mvp", p * mv * glm::inverse(spirit.trans()));
    glDrawArraysInstanced(GL_TRIANGLES, 0, vertex_arrays_[spirit.id()].count(),
                          MAX_PARTICLE_COUNT);
    checkGlError();
}

void SpiritRenderer::updateParticles(const Spirit& spirit) {
    // Update existing particles and clean up dead particles.
    for (size_t i = 0; i < MAX_PARTICLE_COUNT; ++i) {
        if (!isParticleDead(spirit.id(), i)) {
            metric(spirit.id(), i)[0] += random() * 0.6f - 0.3f;
            metric(spirit.id(), i)[1] += random() * 0.6f - 0.3f;
            metric(spirit.id(), i)[2] += random() * 0.6f - 0.3f;
            // Become smaller.
            metric(spirit.id(), i)[3] *= 0.9;
            // Become more transparent.
            color(spirit.id(), i)[3] *= random() * 0.1f + 0.9f;
            if (isParticleDead(spirit.id(), i)) {
                memset(metric(spirit.id(), i), 0, 4 * sizeof (GLfloat));
                memset(color(spirit.id(), i), 0, 4 * sizeof (GLfloat));
            }
        }
    }

    // Generate particles at this spirit's location in parent frame.
    // Here w is used as the scale of the particle.
    vec4 metric = spirit.trans() * vec4(0.0f, 0.0f, 0.0f, 1.0f);
    metric.w = 0.1f;
    // Generate new particles.
    for (size_t i = 0, num_generated = 0;
         num_generated < MAX_PARTICLE_COUNT / 50 && i < MAX_PARTICLE_COUNT;
         ++i) {
        if (isParticleDead(spirit.id(), i)) {
            vec4 color(random() * 0.5f, random() * 0.5f, random() * 0.5f, 1.0f);
            if (spirit.is_player())
                color.b += 0.5;
            else
                color.r += 0.5;
            genParticle(spirit.id(), i, metric, color);
            num_generated++;
        }
    }
    size_t stream_buffer_len = 4 * MAX_PARTICLE_COUNT * sizeof (GLfloat);
    vertex_arrays_[spirit.id()].stream("metric", metrics(spirit.id()),
                                       stream_buffer_len);
    vertex_arrays_[spirit.id()].stream("color", colors(spirit.id()),
                                       stream_buffer_len);
}

void SpiritRenderer::genParticle(unsigned spirit_id, int index,
                                 const vec4& metric, const vec4& color) {
    memcpy(this->metric(spirit_id, index), glm::value_ptr(metric),
           4 * sizeof (GLfloat));
    memcpy(this->color(spirit_id, index), glm::value_ptr(color),
           4 * sizeof (GLfloat));
}

bool SpiritRenderer::isParticleDead(unsigned spirit_id, int index) {
    // It's dead, Jim! Too transparent.
    return color(spirit_id, index)[3] < 0.1f;
}

GLfloat* SpiritRenderer::metric(unsigned spirit_id, int metric_index) {
    return metrics_[spirit_id][metric_index];
}

GLfloat* SpiritRenderer::color(unsigned spirit_id, int color_index) {
    return colors_[spirit_id][color_index];
}

GLfloat* SpiritRenderer::metrics(int spirit_id) const {
    return (GLfloat*) metrics_[spirit_id];
}

GLfloat* SpiritRenderer::colors(int spirit_id) const {
    return (GLfloat*) colors_[spirit_id];
}

float SpiritRenderer::random() const {
    return ((float) rand()) / RAND_MAX;
}
