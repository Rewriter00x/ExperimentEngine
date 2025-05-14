#pragma once

namespace Exp
{
    //c update
    struct PlayerInputComponent : public ComponentBase
    {
        //p draw, save
        float MoveSpeed = 10.f;
        
        virtual void Start() override;
        virtual void End() override;
        virtual void OnUpdate(float deltaSeconds) override;

        bool OnKeyPressed(const KeyPressedEvent& e);
    };
}
