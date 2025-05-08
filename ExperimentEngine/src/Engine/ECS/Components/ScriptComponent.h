#pragma once

#include "Engine/ECS/Script/NativeScript.h"
#include "Engine/ECS/Script/ScriptInfo.h"
#include "Engine/ECS/Script/ScriptUtils.h"

namespace Exp
{
    //c
    struct ScriptComponent
    {
        //p draw, save
        ScriptInfo SelectedScript;
        
        NativeScript* Script = nullptr;

        ScriptComponent() = default;

        void Create(Entity_ID entityID, World* world)
        {
            EXP_ASSERT(!Script);

            Script = CreateScriptByName(SelectedScript.Name, entityID, world);
            SelectedScript.Properties->InitScript(Script);
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
