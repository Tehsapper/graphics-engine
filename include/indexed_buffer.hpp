#pragma once

#include <context_debug.hpp>

#include <string>
#include <stdexcept>
#include <unordered_map>
#include <map>
#include <memory>

namespace GraphicsEngine {
    class Buffer;

    struct buffer_not_found : public std::runtime_error {
        explicit buffer_not_found(const std::string& name) : runtime_error(name) {}
    };

    struct multiple_indexed_buffers : public std::runtime_error {
        explicit multiple_indexed_buffers(const std::string& name) : runtime_error(name) {}
    };

    class IndexedBuffer {
    public:
        enum class Type { UniformBuffer = GL_UNIFORM_BLOCK, ShaderStorage = GL_SHADER_STORAGE_BLOCK };
        using Identifier = std::pair<Type, std::string>;
    private:
        static inline std::unordered_multimap<std::string, std::shared_ptr<Buffer>> buffers;
        static inline std::unordered_map<IndexedBuffer::Type, GLint> current_bind;
        static inline std::map<Identifier, GLuint> bound;
    public:
        static GLuint getBindingPoint(Type type, std::string_view name) noexcept;

        static void add(std::string_view name, std::shared_ptr<Buffer> buffer);
        static std::shared_ptr<Buffer> get(std::string_view name);
    };

    struct IndexedBufferData {
        IndexedBuffer::Type target;
        std::string name;
        GLuint block_index;
        GLuint bound_point;
        bool index_connected {false};

        IndexedBufferData(IndexedBuffer::Type _target, std::string _name, GLuint _block_index, GLuint _bound_point)
                : target{_target}, name{std::move(_name)}, block_index{_block_index}, bound_point{_bound_point} {}
    };
}