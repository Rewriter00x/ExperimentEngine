#include "GameApplication.h"

Exp::Application* CreateApplication(const std::string& name)
{
	return new GameApplication(name);
}

GameApplication::GameApplication(const std::string& name)
	: Application(name)
{

}
