#ifndef OPENGL_DODGER_SPIRIT_RENDERER_H_
#define OPENGL_DODGER_SPIRIT_RENDERER_H_

#include <GL/glew.h>
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

    // Renders a spirit with the specified MVP transformations.
    void render(const Spirit&, const glm::mat4& transformations) const;

  private:
    const Game& game_;
    const ShaderProgram& program_;
    const VertexArray vert_arr_;
    const int lod_;
};

#endif // OPENGL_DODGER_SPIRIT_RENDERER_H_
