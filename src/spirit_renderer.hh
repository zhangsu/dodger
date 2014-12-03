#ifndef DODGER_SPIRIT_RENDERER_HH_
#define DODGER_SPIRIT_RENDERER_HH_

#include <glm/glm.hpp>
#include "game.hh"
#include "shader_program.hh"
#include "spirit.hh"
#include "vertex_array.hh"

// The renderer for spirits.
class SpiritRenderer {
  public:
    // Constructs a spirit renderer with the specified shader program.
    SpiritRenderer(const Game&);

    // Renders a spirit with the specified model-view and projection
    // transformations for shadowing.
    void renderShadow(const Spirit&, const glm::mat4& mv, const glm::mat4& p);
    // Renders a spirit with the specified model-view and projection
    // transformations.
    void render(const Spirit&, const glm::mat4& mv, const glm::mat4& p);

  private:
    static const size_t MAX_PARTICLE_COUNT = 1000;

    // Update particle metrics and colors.
    void updateParticles(const Spirit&);
    // generate a new particle at the specified index.
    void genParticle(unsigned spirit_id, int index, const glm::vec4& metric,
                     const glm::vec4& color);
    // Tests if a particle is dead at the specified index.
    inline bool isParticleDead(unsigned spirit_id, int index);
    // Return the specific metric for a particle of a specific spirit.
    inline GLfloat* metric(unsigned spirit_id, int metric_index);
    // Return the specific color for a particle of a specific spirit.
    inline GLfloat* color(unsigned spirit_id, int color_index);
    // Returns a list of particle metrics, which are 4-tuples (x, y, z, scale).
    inline GLfloat* metrics(int spirit_id) const;
    // Returns a list of particle colors.
    inline GLfloat* colors(int spirit_id) const;
    // Generates a random number berween 0 and 1.
    inline float random() const;

    static const int LOD = 5;

    const Game& game_;
    const ShaderProgram program_;
    const ShaderProgram shadow_mapper_;

    VertexArray vertex_arrays_[Spirit::MAX_COUNT];

    GLfloat metrics_[Spirit::MAX_COUNT][MAX_PARTICLE_COUNT][4];
    GLfloat colors_[Spirit::MAX_COUNT][MAX_PARTICLE_COUNT][4];
};

#endif // DODGER_SPIRIT_RENDERER_HH_
