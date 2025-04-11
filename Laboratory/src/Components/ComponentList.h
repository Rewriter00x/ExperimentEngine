#pragma once

#include "Engine/ECS/Components/SpriteComponent.h"

namespace Exp
{
    template<typename... Components>
    struct ComponentList {};
    
    // BEGIN TODO REFLECTOR
    using AllComponents = ComponentList<SpriteComponent>;
    // END TODO REFLECTOR
}
