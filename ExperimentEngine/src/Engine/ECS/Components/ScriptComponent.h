#pragma once

#include "Engine/ECS/Script/NativeScript.h"
#include "Engine/ECS/Script/ScriptInfo.h"
#include "Engine/ECS/Script/ScriptUtils.h"

namespace Exp
{
    //c update
    struct ScriptComponent : public ComponentBase
    {
        //p draw, save
        ScriptInfo SelectedScript;
        
        NativeScript* Script = nullptr;

        ScriptComponent() = default;

        virtual void Start() override
        {
            EXP_ASSERT(!Script);

            Script = CreateScriptByName(SelectedScript.Name, GetEntity().GetID(), &GetWorld());
            SelectedScript.Properties->InitScript(Script);
            Script->Start();
        }

        virtual void End() override
        {
            EXP_ASSERT(Script);

            Script->End();
            delete Script;
            Script = nullptr;
        }

        virtual void OnUpdate(float deltaSeconds) override
        {
            EXP_ASSERT(Script);
            
            Script->OnUpdate(deltaSeconds);
        }
    };
}
