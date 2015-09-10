#pragma once

#include "Object.hpp"

namespace glrage {

class Sampler : public Object {
public:
    Sampler();
    ~Sampler();
    void bind();
    void bind(GLuint unit);
    void parameteri(GLenum pname, GLint param);
    void parameterf(GLenum pname, GLfloat param);
};

}