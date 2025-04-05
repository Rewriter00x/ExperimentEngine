#pragma once

namespace Exp
{
    class UUID
    {
    public:
        UUID();
        UUID(uint64 uuid);

        operator uint64() const { return m_UUID; }

    private:
        uint64 m_UUID;
        
    };
}

template<>
struct std::hash<Exp::UUID>
{
    std::size_t operator()(const Exp::UUID& uuid) const noexcept;
};