#pragma once

#include <memory>

namespace LimitlessEngine {
    template<typename T> class Distribution;
    class ByteBuffer;

    class DistributionSerializer {
    public:
        template<typename T>
        ByteBuffer serialize(Distribution<T>& distr);

        template<typename T>
        std::unique_ptr<Distribution<T>> deserialize(ByteBuffer& buffer);
    };

    template<typename T>
    ByteBuffer& operator<<(ByteBuffer& buffer, Distribution<T>& distr);

    template<typename T>
    ByteBuffer& operator>>(ByteBuffer& buffer, std::unique_ptr<Distribution<T>>& distr);
}
