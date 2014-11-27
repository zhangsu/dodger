#ifndef OPENGL_DODGER_TERRAIN_RENDERER_HH_
#define OPENGL_DODGER_TERRAIN_RENDERER_HH_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "game.hh"
#include "shader_program.hh"
#include "terrain.hh"
#include "texture.hh"
#include "vertex_array.hh"

// The renderer for terrains.
class TerrainRenderer {
  public:
    // Constructs a terrain renderer with the specified shader program.
    TerrainRenderer(const Game&, const ShaderProgram&);

    // Renders a terrain with the specified model-view and projection
    // transformations.
    void render(const Terrain&, const glm::mat4& mv, const glm::mat4& p) const;

  private:
    // Generates positions from a terrain and return the position count.
    void genVertices(const Terrain&) const;
    void addPosition(const Terrain&, std::vector<GLfloat>& positions,
                     int x, int z) const;
    void addNormal(const Terrain&, std::vector<GLfloat>& normals,
                   int x, int z) const;

    const Game& game_;
    const ShaderProgram& program_;
    const VertexArray vertex_array_;
    const Texture grass_texture_;
    const Texture rock_texture_;

    // These are lazily-initialized by render.
    mutable bool initialized;
    mutable size_t vertex_count_;
};

#endif // OPENGL_DODGER_TERRAIN_RENDERER_HH_
