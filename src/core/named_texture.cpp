#include <core/named_texture.hpp>
#include <core/context_state.hpp>
#include <algorithm>

using namespace LimitlessEngine;

NamedTexture::NamedTexture(GLenum _target) noexcept : target{_target} {
    glCreateTextures(target, 1, &id);
}

NamedTexture::~NamedTexture() {
    if (id != 0) {
        if (auto* state = ContextState::getState(glfwGetCurrentContext()); state) {
            auto& target_map = state->texture_bound;

            std::for_each(target_map.begin(), target_map.end(), [&] (auto& state) {
                auto& [s_unit, s_id] = state;
                if (s_id == id) s_id = 0;
            });

            glDeleteTextures(1, &id);
        }
    }
}

void NamedTexture::texStorage2D([[maybe_unused]] GLenum _target, GLsizei levels, GLenum internal_format, glm::uvec2 size) const noexcept {
    glTextureStorage2D(id, levels, internal_format, size.x, size.y);
}

void NamedTexture::texStorage3D([[maybe_unused]] GLenum _target, GLsizei levels, GLenum internal_format, glm::uvec3 size) const noexcept {
    glTextureStorage3D(id, levels, internal_format, size.x, size.y, size.z);
}

void NamedTexture::texStorage2DMultisample([[maybe_unused]] GLenum _target, uint8_t samples, GLenum internal_format, glm::uvec2 size) const noexcept {
    glTextureStorage2DMultisample(id, samples, internal_format, size.x, size.y, GL_FALSE);
}

void NamedTexture::texImage2D(GLenum _target, GLsizei levels, GLenum internal_format, GLenum format, GLenum type, glm::uvec2 size, const void* data) const noexcept {
    texSubImage2D(target, levels, 0, 0, size, format, type, data);
}

void NamedTexture::texImage3D(GLenum _target, GLsizei levels, GLenum internal_format, GLenum format, GLenum type, glm::uvec3 size, const void *data) const noexcept {
    glTextureImage3DEXT(id, _target, levels, internal_format, size.x, size.y, size.z, 0, format, type, data);
}

void NamedTexture::texImage2DMultiSample(GLenum _target, uint8_t samples, GLenum internal_format, glm::uvec3 size) const noexcept {
    glTextureImage2DMultisampleNV(id, _target, samples, internal_format, size.x, size.y, GL_FALSE);
}

void NamedTexture::texSubImage2D([[maybe_unused]] GLenum _target, GLint level, GLint xoffset, GLint yoffset, glm::uvec2 size, GLenum format, GLenum type, const void* data) const noexcept {
    glTextureSubImage2D(id, level, xoffset, yoffset, size.x, size.y, format, type, data);
}

void NamedTexture::texSubImage3D([[maybe_unused]] GLenum _target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, glm::uvec3 size, GLenum format, GLenum type, const void* data) const noexcept {
    glTextureSubImage3D(id, level, xoffset, yoffset, zoffset, size.x, size.y, size.z, format, type, data);
}

void NamedTexture::bind([[maybe_unused]] GLenum _target, GLuint index) const noexcept {
    if (auto* state = ContextState::getState(glfwGetCurrentContext()); state) {
        if (state->texture_bound[index] != id) {
            glBindTextureUnit(index, id);
            state->texture_bound[index] = id;
        }
    }
}

void NamedTexture::generateMipMap([[maybe_unused]] GLenum _target) const noexcept {
    glGenerateTextureMipmap(id);
}

void NamedTexture::texParameter([[maybe_unused]] GLenum _target, GLenum name, GLint param) const noexcept {
    glTextureParameteri(id, name, param);
}

void NamedTexture::texParameter([[maybe_unused]] GLenum _target, GLenum name, GLfloat param) const noexcept {
    glTextureParameterf(id, name, param);
}

void NamedTexture::texParameter([[maybe_unused]] GLenum _target, GLenum name, GLint* params) const noexcept {
    glTextureParameteriv(id, name, params);
}

void NamedTexture::texParameter([[maybe_unused]] GLenum _target, GLenum name, GLfloat* params) const noexcept {
    glTextureParameterfv(id, name, params);
}

NamedTexture* NamedTexture::clone() const {
    return new NamedTexture(target);
}

void NamedTexture::accept(TextureVisitor& visitor) noexcept {
    visitor.visit(*this);
}

NamedTexture::NamedTexture(NamedTexture&& rhs) noexcept {
    std::swap(id, rhs.id);
    std::swap(target, rhs.target);
}

NamedTexture& NamedTexture::operator=(NamedTexture&& rhs) noexcept {
    std::swap(id, rhs.id);
    std::swap(target, rhs.target);
    return *this;
}
