#include <limitless/core/texture_builder.hpp>

#include <limitless/core/context_initializer.hpp>
#include <limitless/core/named_texture.hpp>
#include <limitless/core/bindless_texture.hpp>
#include <limitless/core/mutable_texture.hpp>
#include <limitless/core/immutable_texture.hpp>

using namespace Limitless;

ExtensionTexture* TextureBuilder::getSupportedTexture(Texture::Type target) {
    ExtensionTexture* extension_texture = ContextInitializer::isExtensionSupported("GL_ARB_direct_state_access") ?
                                          new NamedTexture(static_cast<GLenum>(target)) :
                                          new StateTexture();

    return ContextInitializer::isExtensionSupported("GL_ARB_bindless_texture") ?
           new BindlessTexture(extension_texture) :
           extension_texture;
}

TextureBuilder& TextureBuilder::setInternalFormat(Texture::InternalFormat _internal) {
    internal = _internal;
    return *this;
}

TextureBuilder& TextureBuilder::setFormat(Texture::Format _format) {
    format = _format;
    return *this;
}

TextureBuilder& TextureBuilder::setTarget(Texture::Type _target) {
    target = _target;
    return *this;
}

TextureBuilder& TextureBuilder::setDataType(Texture::DataType _data_type) {
    data_type = _data_type;
    return *this;
}

TextureBuilder& TextureBuilder::setData(const void* _data) {
    data = _data;
    return *this;
}

TextureBuilder& TextureBuilder::setData(const std::array<void *, 6>& _data) {
    data = _data;
    return *this;
}

TextureBuilder& TextureBuilder::setSize(glm::uvec2 _size) {
    size = _size;
    return *this;
}

TextureBuilder& TextureBuilder::setSize(glm::uvec3 _size) {
    size = _size;
    return *this;
}

TextureBuilder& TextureBuilder::setLevels(GLsizei _levels) {
    levels = _levels;
    return *this;
}

TextureBuilder& TextureBuilder::setMipMap(bool _mipmap) {
    mipmap = _mipmap;
    return *this;
}

TextureBuilder& TextureBuilder::setMinFilter(Texture::Filter filter) {
    min = filter;
    return *this;
}

TextureBuilder& TextureBuilder::setMagFilter(Texture::Filter filter) {
    mag = filter;
    return *this;
}

TextureBuilder& TextureBuilder::setWrapS(Texture::Wrap wrap) {
    wrap_s = wrap;
    return *this;
}

TextureBuilder& TextureBuilder::setWrapT(Texture::Wrap wrap) {
    wrap_t = wrap;
    return *this;
}

TextureBuilder& TextureBuilder::setWrapR(Texture::Wrap wrap) {
    wrap_r = wrap;
    return *this;
}

TextureBuilder& TextureBuilder::setBorder(bool _border) {
    border = _border;
    return *this;
}

TextureBuilder& TextureBuilder::setBorderColor(const glm::vec4& color) {
    border_color = color;
    return *this;
}

std::shared_ptr<Texture> TextureBuilder::buildMutable() {
    std::shared_ptr<Texture> texture;

    // if data contains std::array building a cubemap
    if (std::holds_alternative<std::array<void*, 6>>(data)) {
        texture = std::make_shared<MutableTexture>(std::unique_ptr<ExtensionTexture>(getSupportedTexture(target)),
                                                   target,
                                                   internal,
                                                   std::get<glm::uvec2>(size),
                                                   format,
                                                   data_type,
                                                   border,
                                                   mipmap,
                                                   border_color,
                                                   std::get<std::array<void*, 6>>(data),
                                                   min,
                                                   mag,
                                                   wrap_s,
                                                   wrap_t,
                                                   wrap_r);
    } else {
        // else building 2d texture
        if (std::holds_alternative<glm::uvec2>(size)) {
            texture = std::make_shared<MutableTexture>(std::unique_ptr<ExtensionTexture>(getSupportedTexture(target)),
                                                       target,
                                                       internal,
                                                       std::get<glm::uvec2>(size),
                                                       format,
                                                       data_type,
                                                       border,
                                                       mipmap,
                                                       border_color,
                                                       std::get<const void*>(data),
                                                       min,
                                                       mag,
                                                       wrap_s,
                                                       wrap_t,
                                                       wrap_r);
        // else building 2d texture array / 3d texture
        } else {
            texture = std::make_shared<MutableTexture>(std::unique_ptr<ExtensionTexture>(getSupportedTexture(target)),
                                                       target,
                                                       internal,
                                                       std::get<glm::uvec3>(size),
                                                       format,
                                                       data_type,
                                                       border,
                                                       mipmap,
                                                       border_color,
                                                       std::get<const void*>(data),
                                                       min,
                                                       mag,
                                                       wrap_s,
                                                       wrap_t,
                                                       wrap_r);
        }
    }

    return texture;
}

std::shared_ptr<Texture> TextureBuilder::build() {
    if (ContextInitializer::isExtensionSupported("GL_ARB_texture_storage")) {
        std::shared_ptr<Texture> texture;
        // if data contains std::array building a cubemap
        if (std::holds_alternative<std::array<void*, 6>>(data)) {
            texture = std::make_shared<ImmutableTexture>(std::unique_ptr<ExtensionTexture>(getSupportedTexture(target)),
                                                       target,
                                                       levels,
                                                       internal,
                                                       std::get<glm::uvec2>(size),
                                                       format,
                                                       data_type,
                                                       border,
                                                       mipmap,
                                                       border_color,
                                                       std::get<std::array<void*, 6>>(data),
                                                       min,
                                                       mag,
                                                       wrap_s,
                                                       wrap_t,
                                                       wrap_r);
        } else {
            // else building 2d texture
            if (std::holds_alternative<glm::uvec2>(size)) {
                texture = std::make_shared<ImmutableTexture>(std::unique_ptr<ExtensionTexture>(getSupportedTexture(target)),
                                                           target,
                                                           levels,
                                                           internal,
                                                           std::get<glm::uvec2>(size),
                                                           format,
                                                           data_type,
                                                           border,
                                                           mipmap,
                                                           border_color,
                                                           std::get<const void*>(data),
                                                           min,
                                                           mag,
                                                           wrap_s,
                                                           wrap_t,
                                                           wrap_r);
                // else building 2d texture array / 3d texture
            } else {
                texture = std::make_shared<ImmutableTexture>(std::unique_ptr<ExtensionTexture>(getSupportedTexture(target)),
                                                           target,
                                                           levels,
                                                           internal,
                                                           std::get<glm::uvec3>(size),
                                                           format,
                                                           data_type,
                                                           border,
                                                           mipmap,
                                                           border_color,
                                                           std::get<const void*>(data),
                                                           min,
                                                           mag,
                                                           wrap_s,
                                                           wrap_t,
                                                           wrap_r);
            }
        }

        return texture;
    } else {
        return buildMutable();
    }
}
