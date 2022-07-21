#pragma once

namespace Pearl 
{
	struct WindowSpec {
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowSpec(const std::string& title = "Pearl Engine",
			unsigned int width = 1280,
			unsigned int height = 720)
			: Title(title), Width(width), Height(height)
		{
		}
	};

	class Window 
	{
	public:
		virtual ~Window() {}
		//TODO: virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth()	 const = 0;
		virtual unsigned int GetHeight() const = 0;

		static Window* Init(const WindowSpec& windowSpec = WindowSpec());
	};
}




