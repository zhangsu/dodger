#include <glm/gtc/matrix_transform.hpp>
#include "renderer.hh"
#include "scene_node.hh"

using glm::mat4;
using glm::vec3;
using glm::vec4;

// Public methods.

SceneNode::SceneNode() : parent_(nullptr) {}

SceneNode::~SceneNode() {
    for (auto child : children_)
        delete child;
}

void SceneNode::attach(SceneNode* child) {
    children_.push_back(child);
    child->parent_ = this;
}

void SceneNode::detach(SceneNode* child) {
    children_.remove(child);
    child->parent_ = nullptr;
}

SceneNode* SceneNode::parent() const {
    return parent_;
}

vec3 SceneNode::position(const SceneNode* node) const {
    return vec3(nodeTransformation(node) * vec4(0, 0, 0, 1));
}

mat4 SceneNode::transformation() const {
    return transformation_;
}

mat4 SceneNode::worldTransformation() const {
    mat4 trans = transformation();
    const SceneNode* node = this;
    while ((node = node->parent()) != nullptr)
        trans = node->transformation() * trans;
    return trans;
}

mat4 SceneNode::nodeTransformation(const SceneNode* node) const {
    mat4 world_trans = worldTransformation();
    if (node == nullptr)
        return world_trans;
    else
        return glm::inverse(node->worldTransformation()) * world_trans;
}

void SceneNode::resetTransformation() {
    transformation_ = mat4();
}

void SceneNode::set_transformation(glm::mat4 transformation) {
    transformation_ = transformation;
}

void SceneNode::render(const Renderer& renderer, mat4 trans) const {
    renderChildren(renderer, trans * transformation());
}

void SceneNode::rotate(float angle, const vec3& axis) {
    transformation_ = glm::rotate(transformation_, angle, axis);
}

void SceneNode::scale(const vec3& amount) {
    transformation_ = glm::scale(transformation_, amount);
}

void SceneNode::translate(const vec3& amount) {
    transformation_ = glm::translate(transformation_, amount);
}

void SceneNode::rotate(float angle, float x, float y, float z) {
    rotate(angle, vec3(x, y, z));
}

void SceneNode::scale(float x, float y, float z) {
    scale(vec3(x, y, z));
}

void SceneNode::translate(float x, float y, float z) {
    translate(vec3(x, y, z));
}

// Protected methods.

void SceneNode::renderChildren(const Renderer& renderer, mat4 stack) const {
    for (auto child : children_)
        child->render(renderer, stack);
}
