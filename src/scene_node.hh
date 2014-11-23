#ifndef DODGER_SCENE_HH_
#define DODGER_SCENE_HH_

#include <list>
#include <string>
#include <glm/glm.hpp>

class Renderer;

// The base type for any node in a scene graph.
class SceneNode {
public:
    // Construct an empty scene node.
    SceneNode();

    // Render this scene node and its descendents.
    virtual void render(const Renderer&,
                        glm::mat4 transformations = glm::mat4()) const;
    // Add a child node to this node.
    virtual void addChild(SceneNode* child);
    // Removes a child node from this node.
    void removeChild(SceneNode* child);
    // Sets the parent of this node.
    void set_parent(SceneNode* parent);
    // Gets the parent of this node.
    SceneNode* parent() const;

    // Rotate this scene node by `angle` alone `axis`.
    void rotate(float angle, const glm::vec3& axis);
    // Scale this scene node by `amount`.
    void scale(const glm::vec3& amount);
    // Translate this scene node by `amount`.
    void translate(const glm::vec3& amount);

protected:
    glm::mat4 transformations_;

    std::list<SceneNode*> children_;
    SceneNode* parent_;
};

#endif // DODGER_SCENE_HH_
