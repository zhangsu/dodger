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
    size_t init(const Terrain&) const;

    const Game& game_;
    const ShaderProgram& program_;

    // These are lazily-initialized by render.
    mutable GLuint vao_;
    mutable bool initialized;
    mutable size_t vertex_count_;
};

#endif // OPENGL_DODGER_TERRAIN_RENDERER_H_
