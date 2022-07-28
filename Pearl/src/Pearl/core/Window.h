#pragma once

namespace Pearl
{
	struct WindowSpec;

	class Window
	{
	public:
		virtual ~Window();

		virtual void Init(const WindowSpec* windowSpec);
		
		virtual unsigned int GetWidth()	const;
		virtual unsigned int GetHeight() const;
	};
}
