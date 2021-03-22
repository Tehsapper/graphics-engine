#include <limitless/core/framebuffer.hpp>
#include <limitless/core/texture.hpp>

using namespace LimitlessEngine;

Framebuffer::Framebuffer(ContextEventObserver& context) noexcept : Framebuffer() {
    context.registerObserver(this);
}

Framebuffer::Framebuffer() noexcept {
    glGenFramebuffers(1, &id);
}

void Framebuffer::bind() noexcept {
    if (auto* state = ContextState::getState(glfwGetCurrentContext()); state) {
        if (state->framebuffer_id != id) {
            glBindFramebuffer(GL_FRAMEBUFFER, id);
            state->framebuffer_id = id;
        }
    }
}

Framebuffer::~Framebuffer() {
    if (id != 0) {
        if (auto* state = ContextState::getState(glfwGetCurrentContext()); state) {
            if (state->framebuffer_id == id) {
                state->framebuffer_id = 0;
            }
            glDeleteBuffers(1, &id);
        }
    }
    //TODO: unregister observer
}

void Framebuffer::onFramebufferChange(glm::uvec2 size) {
    for (auto& [type, attachment] : attachments) {
        attachment.texture->resize({ size, attachment.texture->getSize().z });
        *this << attachment;
    }
}

void Framebuffer::checkStatus() {
    bind();

    if (auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER); status != GL_FRAMEBUFFER_COMPLETE) {
        throw framebuffer_error{"Framebuffer is incomplete"};
    }
}

void Framebuffer::clear() noexcept {
    bind();

    GLenum bits = 0;

    if (hasAttachment(FramebufferAttachment::Color0)) {
        bits |= GL_COLOR_BUFFER_BIT;
    }

    if (hasAttachment(FramebufferAttachment::Depth)) {
        bits |= GL_DEPTH_BUFFER_BIT;
    }

    if (hasAttachment(FramebufferAttachment::Stencil)) {
        bits |= GL_STENCIL_BUFFER_BIT;
    }

    if (hasAttachment(FramebufferAttachment::DepthStencil)) {
        bits |= GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
    }

    glClear(bits);
}

bool Framebuffer::hasAttachment(FramebufferAttachment a) const noexcept {
    return attachments.find(a) != attachments.end();
}

void Framebuffer::drawBuffers(const std::vector<FramebufferAttachment>& a) noexcept {
    bind();

    glDrawBuffers(a.size(), reinterpret_cast<const GLenum*>(a.data()));
}

void Framebuffer::drawBuffer(FramebufferAttachment a) noexcept {
    bind();

    glDrawBuffers(1, reinterpret_cast<const GLenum*>(&a));
}

void Framebuffer::specifyLayer(FramebufferAttachment attachment, uint32_t layer) {
    try {
        auto& tex_attachment = attachments.at(attachment);

        if (tex_attachment.layer != layer) {
            bind();
            glFramebufferTextureLayer(GL_FRAMEBUFFER, static_cast<GLenum>(tex_attachment.attachment), tex_attachment.texture->getId(), 0, layer);
            tex_attachment.layer = layer;
        }
    } catch (...) {
        throw framebuffer_error{"No such framebuffer attachment"};
    }
}

Framebuffer& Framebuffer::operator<<(const TextureAttachment& attachment) noexcept {
    bind();

    switch (attachment.texture->getType()) {
        case Texture::Type::Tex2D:
            glFramebufferTexture2D(GL_FRAMEBUFFER, static_cast<GLenum>(attachment.attachment), static_cast<GLenum>(attachment.texture->getType()), attachment.texture->getId(), 0);
            break;
        case Texture::Type::CubeMap:
            glFramebufferTexture(GL_FRAMEBUFFER, static_cast<GLenum>(attachment.attachment), attachment.texture->getId(), 0);
            break;
        case Texture::Type::Tex2DArray:
        case Texture::Type::TexCubeMapArray:
        case Texture::Type::Tex3D:
            glFramebufferTextureLayer(GL_FRAMEBUFFER, static_cast<GLenum>(attachment.attachment), attachment.texture->getId(), 0, attachment.layer);
            break;
    }

    attachments.emplace(attachment.attachment, attachment);
    return *this;
}

const TextureAttachment& Framebuffer::get(FramebufferAttachment attachment) const {
    try {
        return attachments.at(attachment);
    } catch (...) {
        throw framebuffer_error{"No such framebuffer attachment"};
    }
}

void Framebuffer::unbind() noexcept {
    if (auto* state = ContextState::getState(glfwGetCurrentContext()); state) {
        if (state->framebuffer_id != 0) {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            state->framebuffer_id = 0;
        }
    }
}

void Framebuffer::bindDefault() noexcept {
    if (auto* state = ContextState::getState(glfwGetCurrentContext()); state) {
        if (state->framebuffer_id != 0) {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            state->framebuffer_id = 0;
        }
    }
}
