#include "exppch.h"
#include "Random.h"

#include <random>

namespace Exp::Rand
{
    static std::random_device s_RandomDevice;
    static std::mt19937 s_Engine32(s_RandomDevice());
    static std::mt19937_64 s_Engine64(s_RandomDevice());
    
    template<typename T, std::enable_if_t<std::is_integral_v<T>, bool> = true>
    static std::uniform_int_distribution<T> s_UniformIntDistribution;

    template<typename T, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
    static std::uniform_real_distribution<T> s_UniformRealDistribution(T(0), std::nextafter(T(1), std::numeric_limits<T>::max()));
    
    int32 RandInt32()
    {
        return s_UniformIntDistribution<int32_t>(s_Engine32);
    }

    uint32 RandUInt32()
    {
        return s_UniformIntDistribution<uint32_t>(s_Engine32);
    }

    int64 RandInt64()
    {
        return s_UniformIntDistribution<int64_t>(s_Engine64);
    }

    uint64 RandUInt64()
    {
        return s_UniformIntDistribution<uint64_t>(s_Engine64);
    }

    float RandFloat()
    {
        return s_UniformRealDistribution<float>(s_Engine32);
    }

    double RandDouble()
    {
        return s_UniformRealDistribution<double>(s_Engine64);
    }
}
