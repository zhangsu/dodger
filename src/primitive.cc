#include "primitive.hh"

Primitive::Primitive() {}

Primitive::Primitive(Material material) : material_(material) {}

const Material& Primitive::material() const {
    return material_;
}
