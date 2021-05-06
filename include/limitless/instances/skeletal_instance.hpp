#pragma once

#include <limitless/instances/model_instance.hpp>
#include <limitless/models/skeletal_model.hpp>

namespace Limitless {
    class SkeletalInstance : public ModelInstance {
    private:
        std::vector<glm::mat4> bone_transform;
        std::shared_ptr<Buffer> bone_buffer;

        const Animation* animation {};
        bool paused {};

        void calculateBoundingBox() noexcept override;
        void initializeBuffer();

        const AnimationNode* findAnimationNode(const Bone& bone) const noexcept;
    public:
        SkeletalInstance(std::shared_ptr<AbstractModel> m, const glm::vec3& position);
        SkeletalInstance(Lighting* lighting, std::shared_ptr<AbstractModel> m, const glm::vec3& position);
        ~SkeletalInstance() override = default;

        SkeletalInstance(const SkeletalInstance&) = default;
        SkeletalInstance(SkeletalInstance&&) noexcept = default;

        SkeletalInstance& setPosition(const glm::vec3& position) noexcept override;
        SkeletalInstance& setRotation(const glm::quat& rotation) noexcept override;
        SkeletalInstance& setScale(const glm::vec3& scale) noexcept override;

        SkeletalInstance* clone() noexcept override;

        void update(Context& context, Camera& camera) override;

        SkeletalInstance& play(const std::string& name);
        SkeletalInstance& pause() noexcept;
        SkeletalInstance& resume() noexcept;
        SkeletalInstance& stop() noexcept;

        using AbstractInstance::draw;
        void draw(Context& ctx, const Assets& assets, ShaderPass shader_type, ms::Blending blending, const UniformSetter& uniform_setter) override;
    };
}

