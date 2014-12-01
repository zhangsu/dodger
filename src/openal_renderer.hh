#ifndef DODGER_OPENAL_RENDERER_HH_
#define DODGER_OPENAL_RENDERER_HH_

#include <seal.h>
#include "audio_renderer.hh"
#include "spirit.hh"

// An interface for audio renderers.
class OpenALRenderer : public AudioRenderer {
  public:
    // Constructs a renderer with the specified game.
    OpenALRenderer(const Game&);
    virtual ~OpenALRenderer();

    using AudioRenderer::render;
    // Renders audio for a spirit node with the specified model transformations.
    virtual void render(const Spirit&, glm::mat4 model_trans);

  private:
    seal_src_t particle_noises[Spirit::MAX_COUNT];
    seal_buf_t particle_noise_buf;
};

#endif // DODGER_OPENAL_RENDERER_HH_

