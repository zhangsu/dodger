#include <glm/gtc/matrix_transform.hpp>
#include "renderer.hh"
#include "scene_node.hh"

using glm::mat4;
using glm::vec3;

SceneNode::SceneNode() : parent_(nullptr) {}

void SceneNode::addChild(SceneNode* child) {
    children_.push_back(child);
    child->set_parent(this);
}

void SceneNode::removeChild(SceneNode* child) {
    children_.remove(child);
}

void SceneNode::set_parent(SceneNode* parent) {
    parent_ = parent;
}

SceneNode* SceneNode::parent() const {
    return parent_;
}

mat4 SceneNode::cumulativeTransformation() const {
    mat4 transformation = transformation_;
    const SceneNode* node = this;
    while ((node = node->parent()) != nullptr)
        transformation = node->transformation_ * transformation;
    return transformation;
}

void SceneNode::render(const Renderer& renderer, mat4 transformations) const {
    mat4 stack = transformations * transformation_;
    for (auto child : children_)
        child->render(renderer, stack);
}

void SceneNode::rotate(float angle, const vec3& axis) {
    transformation_ = glm::rotate(transformation_, angle,  axis);
}

void SceneNode::scale(const vec3& amount) {
    transformation_ = glm::scale(transformation_, amount);
}

void SceneNode::translate(const vec3& amount) {
    transformation_ = glm::translate(transformation_, amount);
}
