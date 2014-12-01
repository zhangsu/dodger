#ifndef DODGER_TERRAIN_RENDERER_HH_
#define DODGER_TERRAIN_RENDERER_HH_

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
    TerrainRenderer(const Game&, int shadow_map_texture_index,
                    const bool& do_lighting, const bool& drawing_shadow,
                    const bool& drawing_fog);

    // Renders a terrain for shadowing.
    void renderShadow(const Terrain&, const glm::mat4& mv,
                      const glm::mat4& p) const;
    // Renders a terrain with the specified model-view and projection
    // transformations.
    void render(const Terrain&, const glm::mat4& mv, const glm::mat4& p,
                const glm::mat4& shadow_mvp) const;

  private:
    // Generates positions from a terrain and return the position count.
    void genVertices(const Terrain&) const;
    void addPosition(const Terrain&, std::vector<GLfloat>& positions,
                     int x, int z) const;
    void addNormal(const Terrain&, std::vector<GLfloat>& normals,
                   int x, int z) const;

    const Game& game_;
    const ShaderProgram program_;
    const ShaderProgram shadow_mapper_;

    const int shadow_map_texture_index_;
    const Texture grass_texture_;
    const Texture rock_texture_;

    const bool& do_lighting_;
    const bool& drawing_shadow_;
    const bool& drawing_fog_;

    // These are lazily-initialized by render.
    mutable bool initialized;
    mutable VertexArray vertex_array_;
};

#endif // DODGER_TERRAIN_RENDERER_HH_
