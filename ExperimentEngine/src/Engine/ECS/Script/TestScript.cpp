#include "exppch.h"
#include "TestScript.h"

namespace Exp
{
    void TestScript::Start()
    {
        NativeScript::Start();

        EXP_LOG(Info, "TestScript Start!");
    }

    void TestScript::End()
    {
        NativeScript::End();

        EXP_LOG(Info, "TestScript End!");
    }

    void TestScript::OnUpdate(float deltaSeconds)
    {
        NativeScript::OnUpdate(deltaSeconds);

        glm::vec3 Rot = GetEntity().GetRotation();
        Rot += glm::vec3 { 0.f, 0.f, RotationPerSec } * deltaSeconds;
        GetEntity().SetRotation(Rot);
    }
}
