#pragma once

namespace Exp
{
	struct WindowProps
	{
		std::string Title = "Experiment Engine";
		uint32 Width = 1920;
		uint32 Height = 1080;
	};

	class Window
	{
	public:
		Window(const WindowProps& props);
		virtual ~Window() = default;

		virtual void OnUpdate(float DeltaSeconds) = 0;

		virtual void SetVSync(bool enabled) = 0;

		static Unique<Window> Create(const WindowProps& props = {});
	};
}

