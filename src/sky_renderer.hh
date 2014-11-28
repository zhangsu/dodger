#ifndef OPENGL_SKY_RENDERER_HH_
#define OPENGL_SKY_RENDERER_HH_

#include <glm/glm.hpp>
#include "game.hh"
#include "shader_program.hh"
#include "sky.hh"
#include "texture.hh"
#include "vertex_array.hh"

// The renderer for spirits.
class SkyRenderer {
  public:
    // Constructs a spirit renderer with the specified shader program.
    SkyRenderer(const Game&, const ShaderProgram&);

    // Renders a spirit with the specified model-view and projection
    // transformations.
    void render(const Sky&, const glm::mat4& mv, const glm::mat4& p) const;

  private:
    const Game& game_;
    const ShaderProgram& program_;
    const Texture texture_;

    VertexArray vertex_array_;
};

#endif // OPENGL_SKY_RENDERER_HH_
