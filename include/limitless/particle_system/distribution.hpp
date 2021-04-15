#pragma once

#include <glm/glm.hpp>
#include <random>

namespace std {
    template<> struct is_floating_point<glm::vec2> : public true_type {};
    template<> struct is_floating_point<glm::vec3> : public true_type {};
    template<> struct is_floating_point<glm::vec4> : public true_type {};

    template<>
    class uniform_real_distribution<glm::vec2> {
    private:
        glm::vec2 min, max;
    public:
        uniform_real_distribution(const glm::vec2& min, const glm::vec2& max) noexcept
                : min(min), max(max) {}

        template<typename _UniformRandomNumberGenerator>
        glm::vec2 operator()(_UniformRandomNumberGenerator& __urng) {
            __detail::_Adaptor<_UniformRandomNumberGenerator, float> __aurng(__urng);
            return { __aurng() * (max.x - min.x) + min.x,
                     __aurng() * (max.y - min.y) + min.y };
        }
    };

    template<>
    class uniform_real_distribution<glm::vec3> {
    private:
        glm::vec3 min, max;
    public:
        uniform_real_distribution(const glm::vec3& min, const glm::vec3& max) noexcept
                : min(min), max(max) {}

        template<typename _UniformRandomNumberGenerator>
        glm::vec3 operator()(_UniformRandomNumberGenerator& __urng) {
            __detail::_Adaptor<_UniformRandomNumberGenerator, float> __aurng(__urng);
            return { __aurng() * (max.x - min.x) + min.x,
                     __aurng() * (max.y - min.y) + min.y,
                     __aurng() * (max.z - min.z) + min.z };
        }
    };

    template<>
    class uniform_real_distribution<glm::vec4> {
    private:
        glm::vec4 min, max;
    public:
        uniform_real_distribution(const glm::vec4& min, const glm::vec4& max)
                : min(min), max(max) {}

        template<typename _UniformRandomNumberGenerator>
        glm::vec4 operator()(_UniformRandomNumberGenerator& __urng) {
            __detail::_Adaptor<_UniformRandomNumberGenerator, float> __aurng(__urng);
            return { __aurng() * (max.x - min.x) + min.x,
                     __aurng() * (max.y - min.y) + min.y,
                     __aurng() * (max.z - min.z) + min.z,
                     __aurng() * (max.w - min.w) + min.w };
        }
    };
}

namespace LimitlessEngine {
    enum class DistributionType { Const, Range, Curve };

    template<typename T>
    class Distribution {
    protected:
        DistributionType type;
        explicit Distribution(DistributionType type) noexcept : type{type} { }
    public:
        virtual ~Distribution() = default;

        [[nodiscard]] virtual T get() = 0;
        [[nodiscard]] virtual Distribution<T>* clone() = 0;
        [[nodiscard]] const auto& getType() const noexcept { return type; }
    };

    template<typename T>
    class ConstDistribution : public Distribution<T> {
    private:
        T value;
    public:
        explicit ConstDistribution(const T& value) noexcept : Distribution<T>(DistributionType::Const), value{value} {}
        ~ConstDistribution() override = default;

        T get() override { return value; }

        [[nodiscard]] Distribution<T>* clone() override {
            return new ConstDistribution<T>(*this);
        }
    };

    template<typename T, typename E = void>
    class uniform_distribution { static_assert("kek i shrek bratya na vek"); };

    template<typename T>
    class uniform_distribution<T, typename std::enable_if_t<std::is_integral_v<T>>> {
        std::uniform_int_distribution<T> distribution;
    public:
        uniform_distribution(T min, T max) : distribution{min, max} {}
        void set(const T& min, const T& max) { distribution = std::uniform_int_distribution{min, max}; }
        template<typename Gen> auto operator()(Gen&& gen) { return distribution(std::forward<Gen>(gen)); }
    };

    template<typename T>
    class uniform_distribution<T, typename std::enable_if_t<std::is_floating_point_v<T>>> {
        std::uniform_real_distribution<T> distribution;
    public:
        uniform_distribution(T min, T max) : distribution{min, max} {}
        void set(const T& min, const T& max) { distribution = std::uniform_real_distribution{min, max}; }
        template<typename Gen> auto operator()(Gen&& gen) { return distribution(std::forward<Gen>(gen)); }
    };

    template<typename T>
    class RangeDistribution : public Distribution<T> {
    private:
        T min, max;

        std::default_random_engine generator;
        uniform_distribution<T> distribution;
    public:
        RangeDistribution(const T& min, const T& max) noexcept
            : Distribution<T>(DistributionType::Range)
            , min(min)
            , max(max)
            , distribution(min, max) {}
        ~RangeDistribution() override = default;

        [[nodiscard]] const T& getMin() const noexcept { return min; }
        [[nodiscard]] const T& getMax() const noexcept { return max; }

        void setMin(const T& _min) noexcept {
            min = _min;
            distribution.set(min, max);
        }

        void setMax(const T& _max) noexcept {
            max = _max;
            distribution.set(min, max);
        }

        T get() override {
            return distribution(generator);
        }

        [[nodiscard]] Distribution<T>* clone() override {
            return new RangeDistribution<T>(*this);
        }
    };

    //TODO:
    template<typename T>
    class CurveDistribution : public Distribution<T> {
    private:

    public:

    };
}
