#pragma once

namespace Exp
{
    class Module
    {
    public:
        virtual ~Module() = default;

        virtual void OnUpdate(float deltaSeconds) = 0;
        virtual void OnImGuiRender() = 0;
    
    };
}
