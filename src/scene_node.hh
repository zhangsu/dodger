#ifndef DODGER_SCENE_HH_
#define DODGER_SCENE_HH_

#include <list>
#include <string>
#include <glm/glm.hpp>

class Renderer;
class AudioRenderer;

// The base type for any node in a scene graph.
class SceneNode {
  public:
    // Construct an empty scene node.
    SceneNode();
    virtual ~SceneNode();

    // Renders this scene node and its descendents for shadowing.
    virtual void renderShadow(Renderer&, glm::mat4 trans = glm::mat4()) const;
    // Renders this scene node and its descendents.
    virtual void render(Renderer&, glm::mat4 trans = glm::mat4()) const;
    // Renders the audio for this scene node and its descendents.
    virtual void renderAudio(AudioRenderer&,
                             glm::mat4 trans = glm::mat4()) const;
    // Adds a child node to this node.
    virtual void attach(SceneNode* child);
    // Removes a child node from this node.
    void detach(SceneNode* child);
    // Gets the parent of this node.
    SceneNode* parent() const;
    // Gets the scaling of this node.
    const glm::vec3& scaling() const;
    // Gets the position of this node in another node's frame, or in world
    // frame if the node is nullptr.
    glm::vec3 position(const SceneNode* node = nullptr) const;
    // Gets the transformation matrix of this node.
    const glm::mat4& trans() const;
    // Gets the cumulative transformation matrix from root to this node.
    glm::mat4 worldTrans() const;
    // Gets the transformation matrix in another node's frame, or in world
    // frame if the node is nullptr.
    glm::mat4 nodeTrans(const SceneNode* = nullptr) const;

    // Resets the transformation for this node.
    void resetTrans();
    // Sets the transformation of this node.
    void set_trans(glm::mat4 trans);
    // Rotate this scene node by `angle` alone `axis`.
    void rotate(float angle, const glm::vec3& axis);
    // Scale this scene node by `amount`.
    void scale(const glm::vec3& amount);
    // Translate this scene node by `amount`.
    void translate(const glm::vec3& amount);
    // Rotate this scene node by `angle` alone an axis specified by x, y, z.
    void rotate(float angle, float x, float y, float z);
    // Scale this scene node by x, y, z.
    void scale(float x, float y, float z);
    // Translate this scene node by x, y, z.
    void translate(float x, float y, float z);

  protected:
    // Renders the children with the specified cumulative matrix stack for
    // shadowing.
    void renderChildrenShadow(Renderer& renderer, glm::mat4 stack) const;
    // Renders the children with the specified cumulative matrix stack.
    void renderChildren(Renderer& renderer, glm::mat4 stack) const;
    // Renders audio for the children with the specified cumulative matrix
    // stack.
    void renderChildrenAudio(AudioRenderer& renderer, glm::mat4 stack) const;

    glm::mat4 trans_;
    glm::vec3 scaling_;

    std::list<SceneNode*> children_;
    SceneNode* parent_;
};

#endif // DODGER_SCENE_HH_
