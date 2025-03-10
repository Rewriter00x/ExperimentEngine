#pragma once

#include "Engine/System/Window.h"

struct GLFWwindow;

namespace Exp
{
	class ExpGLFWWindow : public Window
	{
	public:
		ExpGLFWWindow(const WindowProps& props);
		~ExpGLFWWindow();

		virtual void OnUpdate(float DeltaSeconds) override;

		virtual void SetVSync(bool enabled) override;

	private:
		GLFWwindow* m_NativeWindow = nullptr;

		WindowProps m_WindowProps;
	};
}

