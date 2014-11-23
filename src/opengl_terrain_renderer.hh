#ifndef OPENGL_DODGER_TERRAIN_RENDERER_H_
#define OPENGL_DODGER_TERRAIN_RENDERER_H_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "game.hh"
#include "shader_program.hh"
#include "terrain.hh"

// The renderer for terrains.
class OpenGLTerrainRenderer {
  public:
    // Constructs a terrain renderer with the specified shader program.
    OpenGLTerrainRenderer(const Game&, const ShaderProgram&);

    // Renders this terrain with the specified MVP transformations.
    void render(const Terrain&, const glm::mat4& transformations) const;

  private:
    const Game& game_;
    const ShaderProgram& program_;

    GLuint vao_;
};

#endif // OPENGL_DODGER_TERRAIN_RENDERER_H_
