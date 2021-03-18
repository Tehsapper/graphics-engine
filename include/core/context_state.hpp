#pragma once

#include <core/context_debug.hpp>
#include <core/buffer.hpp>
#include <unordered_map>
#include <glm/glm.hpp>
#include <mutex>
#include <map>

namespace LimitlessEngine {
    struct BindingPoint {
        Buffer::Type target;
        GLuint point;
    };
    bool operator<(const BindingPoint& a, const BindingPoint& b) noexcept;

    enum class Clear {
        Color = GL_COLOR_BUFFER_BIT,
        Depth = GL_DEPTH_BUFFER_BIT,
        Stencil = GL_STENCIL_BUFFER_BIT,
        ColorDepth = Color | Depth,
        ColorDepthStencil = ColorDepth | Stencil
    };

    enum class DepthFunc {
        Never = GL_NEVER,
        Less = GL_LESS,
        Equal = GL_EQUAL,
        Lequal = GL_LEQUAL,
        Greater = GL_GREATER,
        Notequal = GL_NOTEQUAL,
        Gequal = GL_GEQUAL,
        Always = GL_ALWAYS
    };

    enum class DepthMask {
        True = GL_TRUE,
        False = GL_FALSE
    };

    enum class Enable {
        DepthTest = GL_DEPTH_TEST,
        Blending = GL_BLEND,
        ProgramPointSize = GL_PROGRAM_POINT_SIZE
    };

    enum class BlendFactor {
        None,
        Zero = GL_ZERO,
        One = GL_ONE,
        DstColor = GL_DST_COLOR,
        SrcColor = GL_SRC_COLOR,
        SrcAlpha = GL_SRC_ALPHA,
        DstAplha = GL_DST_ALPHA,
        OneMinusSrcAlpha = GL_ONE_MINUS_SRC_ALPHA,
        OneMinusDstAlpha = GL_ONE_MINUS_DST_ALPHA,
        BlendColor = GL_CONSTANT_COLOR
    };

    enum class CullFace {
        Front = GL_FRONT,
        Back = GL_BACK,
        FrontBack = GL_FRONT_AND_BACK
    };

    enum class FrontFace {
        CW = GL_CW,
        CCW = GL_CCW
    };

    enum class PolygonMode {
        Point = GL_POINT,
        Line = GL_LINE,
        Fill = GL_FILL,
    };

    class Context;

    class ContextState {
    protected:
        std::unordered_map<Enable, bool> enable_map;
        glm::uvec2 viewport {};
        glm::vec4 clear_color {};

        DepthFunc depth_func {DepthFunc::Less};
        DepthMask depth_mask {DepthMask::True};
        CullFace cull_face {CullFace::Back};
        FrontFace front_face {FrontFace::CCW};
        CullFace polygon_face {CullFace::FrontBack};
        PolygonMode polygon_mode {PolygonMode::Fill};
        BlendFactor src_factor {BlendFactor::None}, dst_factor {BlendFactor::Zero};
        glm::vec4 blending_color {0.0f};

        GLuint shader_id {};
        GLuint vertex_array_id {};
        GLuint framebuffer_id {};

        // contains [target, last buffer id]
        std::unordered_map<Buffer::Type, GLuint> buffer_target;
        // contains [binding point, last_buffer_id]
        std::map<BindingPoint, GLuint> buffer_point;

        GLuint active_texture {};
        // contains [texture_image_unit, texture_id]
        std::map<GLuint, GLuint> texture_bound;
        // contains [texture_handle, resident]
        std::map<GLuint64, bool> texture_resident;

        ContextState() = default;
        void init() noexcept;

        static inline std::unordered_map<GLFWwindow*, ContextState*> state_map;
        static inline std::mutex mutex;
        void swapStateMap(Context& lhs, Context& rhs) noexcept;
        void unregisterState(GLFWwindow* window) noexcept;
        void registerState(GLFWwindow* window) noexcept;

        friend class StateBuffer;
        friend class NamedBuffer;
        friend class ShaderProgram;
        friend class VertexArray;
        friend class StateTexture;
        friend class NamedTexture;
        friend class BindlessTexture;
        friend class TextureBinder;
        friend class Framebuffer;
    public:
        virtual ~ContextState() = default;

        ContextState(const ContextState&) = delete;
        ContextState& operator=(const ContextState&) = delete;

        ContextState(ContextState&&) noexcept = default;
        ContextState& operator=(ContextState&&) noexcept = default;

        static bool hasState(GLFWwindow* window) noexcept;
        static ContextState* getState(GLFWwindow* window) noexcept;

        void setViewPort(glm::uvec2 viewport_size) noexcept;
        void clearColor(const glm::vec4& color) noexcept;
        void setDepthFunc(DepthFunc func) noexcept;
        void setDepthMask(DepthMask mask) noexcept;
        void setCullFace(CullFace mode) noexcept;
        void setFrontFace(FrontFace mode) noexcept;
        void setPolygonMode(CullFace face, PolygonMode mode) noexcept;
        void setBlendFunc(BlendFactor src, BlendFactor dst) noexcept;
        void setBlendColor(const glm::vec4& color) noexcept;
        void clear(Clear bits) noexcept;
        void disable(Enable func) noexcept;
        void enable(Enable func) noexcept;
    };
}
