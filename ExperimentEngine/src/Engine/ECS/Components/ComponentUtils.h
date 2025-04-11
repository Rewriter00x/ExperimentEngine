#pragma once

namespace Exp
{
    template<typename T>
    const char* GetComponentName();

    template<typename T>
    void DrawComponent(Entity& e);
    
    template<typename T>
    void SerializeComponent(const Entity& e);
    
    template<typename T>
    void DeserializeComponent(Entity& e);
}