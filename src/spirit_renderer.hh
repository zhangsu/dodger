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
    SpiritRenderer(const Game&, const ShaderProgram&);

    // Renders a spirit with the specified model-view and projection
    // transformations.
    void render(const Spirit&, const glm::mat4& mv, const glm::mat4& p) const;

  private:
    const Game& game_;
    const ShaderProgram& program_;
    const int lod_;

    VertexArray vertex_array_;
};

#endif // DODGER_SPIRIT_RENDERER_HH_
