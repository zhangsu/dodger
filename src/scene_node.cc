#include <glm/gtc/matrix_transform.hpp>
#include "renderer.hh"
#include "scene_node.hh"

using glm::mat4;
using glm::vec3;
using glm::vec4;

// Public methods.

SceneNode::SceneNode() : scaling_(1.0f, 1.0f, 1.0f), parent_(nullptr) {}

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

const vec3& SceneNode::scaling() const {
    return scaling_;
}

vec3 SceneNode::position(const SceneNode* node) const {
    return vec3(nodeTrans(node) * vec4(0, 0, 0, 1));
}

const mat4& SceneNode::trans() const {
    return trans_;
}

mat4 SceneNode::worldTrans() const {
    mat4 trans = this->trans();
    const SceneNode* node = this;
    while ((node = node->parent()) != nullptr)
        trans = node->trans() * trans;
    return trans;
}

mat4 SceneNode::nodeTrans(const SceneNode* node) const {
    mat4 world_trans = worldTrans();
    if (node == nullptr)
        return world_trans;
    else
        return glm::inverse(node->worldTrans()) * world_trans;
}

void SceneNode::resetTrans() {
    trans_ = mat4();
}

void SceneNode::set_trans(glm::mat4 trans) {
    trans_ = trans;
}

void SceneNode::render(const Renderer& renderer, mat4 trans) const {
    renderChildren(renderer, trans * this->trans());
}

void SceneNode::rotate(float angle, const vec3& axis) {
    trans_ = glm::rotate(trans_, angle, axis);
}

void SceneNode::scale(const vec3& amount) {
    trans_ = glm::scale(trans_, amount);
    scaling_ *= amount;
}

void SceneNode::translate(const vec3& amount) {
    trans_ = glm::translate(trans_, amount);
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
