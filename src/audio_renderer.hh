#ifndef DODGER_AUDIO_RENDERER_HH_
#define DODGER_AUDIO_RENDERER_HH_

#include "game.hh"
#include "scene_node.hh"
#include "spirit.hh"

// An interface for audio renderers.
class AudioRenderer {
  public:
    // Constructs an audio renderer with the specified game.
    explicit AudioRenderer(const Game&);
    virtual ~AudioRenderer();

    // Renders everything.
    virtual void render();
    // Renders audio for a generic scene node with the specified model
    // transformations.
    virtual void render(const SceneNode&, glm::mat4 model_trans);
    // Renders audio for a spirit node with the specified model transformations.
    virtual void render(const Spirit&, glm::mat4 model_trans) = 0;

  protected:
    const Game& game_;
};

#endif // DODGER_AUDIO_RENDERER_HH_
