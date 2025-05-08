#pragma once

#include "NativeScript.h"

namespace Exp
{
    //sc
    class TestScript : public NativeScript
    {
    public:
        TestScript(Entity_ID entityID, World* world)
            : NativeScript(entityID, world) {}

        virtual void Start() override;
        virtual void End() override;
        virtual void OnUpdate(float deltaSeconds) override;
        
        //p draw, save
        float RotationPerSec = 45.f;
    
    };
}
