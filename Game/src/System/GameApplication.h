#pragma once

#include "ExperimentEngine.h"

class GameApplication : public Exp::Application
{
public:
	GameApplication(const std::string& name = "");
	~GameApplication() = default;

};

