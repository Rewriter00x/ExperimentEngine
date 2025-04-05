#include "exppch.h"
#include "UUID.h"

namespace Exp
{
    UUID::UUID()
        : m_UUID(Rand::RandUInt64())
    {
    }

    UUID::UUID(uint64 uuid)
        : m_UUID(uuid)
    {
    }
}

std::size_t std::hash<Exp::UUID>::operator()(const Exp::UUID& uuid) const noexcept
{
    return hash<uint64>()(uuid);
}