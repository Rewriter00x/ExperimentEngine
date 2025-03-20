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

		inline virtual uint32 GetWidth() const override { return m_WindowProps.Width; }
		inline virtual uint32 GetHeight() const override { return m_WindowProps.Height; }

		inline virtual void* GetNativeWindow() const override { return m_NativeWindow; }

	private:
		void InitEvents() const;

		GLFWwindow* m_NativeWindow = nullptr;

		WindowProps m_WindowProps;
	};
}

