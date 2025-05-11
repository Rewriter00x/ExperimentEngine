#pragma onle

#include "ExperimentEngine.h"

namespace Exp
{
    class RuntimeModule : public Module
    {
    public:
        RuntimeModule();

        virtual void OnUpdate(float deltaSeconds) override;
        virtual void OnImGuiRender() override;
        
    private:
        Shared<World> m_ActiveWorld;
        
    };
}
