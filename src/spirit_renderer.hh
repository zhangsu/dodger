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
    static const size_t MAX_PARTICLE_COUNT = 10000;

    // Update particle metrics and colors.
    void updateParticles(const Spirit&);
    // generate a new particle at the specified index.
    void genParticle(int index, glm::vec4 metric, glm::vec4 color);
    // Tests if a particle is dead at the specified index.
    bool isParticleDead(int index) const;
    // Returns a list of particle metrics, which are 4-tuples (x, y, z, scale).
    GLfloat* metrics() const;
    // Returns a list of particle colors.
    GLfloat* colors() const;

    float random() const;

    const Game& game_;
    const ShaderProgram program_;
    const ShaderProgram shadow_mapper_;
    const int lod_;

    VertexArray vertex_array_;

    GLfloat metrics_[MAX_PARTICLE_COUNT][4];
    GLfloat colors_[MAX_PARTICLE_COUNT][4];
};

#endif // DODGER_SPIRIT_RENDERER_HH_
