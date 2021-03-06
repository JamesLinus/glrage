#include "Texture.hpp"

namespace glrage {

Texture::Texture(GLenum target) : m_target(target) {
    glGenTextures(1, &m_id);
}

Texture::~Texture() {
    glDeleteTextures(1, &m_id);
}

void Texture::bind() {
    glBindTexture(m_target, m_id);
}

}