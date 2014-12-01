#ifndef DODGER_SPIRIT_HH_
#define DODGER_SPIRIT_HH_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "scene_node.hh"
#include "terrain.hh"

// A class that stores the properties of a terrain.
class Spirit : public SceneNode {
  public:
    static const size_t MAX_COUNT = 10;

    // Constructs a spherical sprit with the specified terrain on which the
    // spirit walks.
    explicit Spirit(const Terrain*, bool player = false);

    // Translate this spirit by x, z on the terrain.
    void translate(float x, float z);
    // Walks the spirit in a turtle graphics fashion.
    void walk();
    // Renders this spirit and its descendents for shadowing.
    virtual void renderShadow(Renderer&, glm::mat4 trans = glm::mat4()) const;
    // Renders this spirit and its descendents.
    virtual void render(Renderer&, glm::mat4 trans = glm::mat4()) const;
    // Renders the audio for this scene node and its descendents.
    virtual void renderAudio(AudioRenderer&,
                             glm::mat4 trans = glm::mat4()) const;

    // Return an integer identifier of this node. The id is guaranteed to be
    // incremental each time a new Spirit is generated.
    unsigned id() const;
    // Determines if this spirit is a player.
    unsigned is_player() const;

  private:
    // Updates this spirit's altitude based on the terrain height.
    void updateAltitude();

    const unsigned id_;
    const bool is_player_;
    const Terrain* const terrain_;
};

#endif // DODGER_SPIRIT_HH_
