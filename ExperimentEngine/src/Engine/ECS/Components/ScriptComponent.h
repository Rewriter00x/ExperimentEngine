#pragma once

#include "Engine/ECS/Script/NativeScript.h"
#include "Engine/ECS/Script/ScriptName.h"
#include "Engine/ECS/Script/ScriptUtils.h"

namespace Exp
{
    //c
    struct ScriptComponent
    {
        //p draw, save
        ScriptName SelectedScript;
        
        NativeScript* Script = nullptr;

        ScriptComponent() = default;

        void Create(Entity_ID entityID, World* world)
        {
            EXP_ASSERT(!Script);

            Script = CreateScriptByName(SelectedScript, entityID, world);
            Script->Start();
        }

        void Destroy()
        {
            EXP_ASSERT(Script);

            Script->End();
            delete Script;
            Script = nullptr;
        }

        void Update(float deltaSeconds) const
        {
            EXP_ASSERT(Script);
            
            Script->OnUpdate(deltaSeconds);
        }
    };
}
