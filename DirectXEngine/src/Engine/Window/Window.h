#pragma once
#include <Windows.h>
#include <memory>
#include <string>
#include <optional>

struct WindowProps
{
	const char* Title;
	unsigned int Width;
	unsigned int Height;
	WindowProps(const char* title = "asd",
		unsigned int width = 1280,
		unsigned int height = 720)
		: Title(title), Width(width), Height(height)
	{}
};

class Window
{
public:
	Window(WindowProps props);
	~Window();

	inline const HWND& GetHandle() const { return m_Hwnd; }

	inline const unsigned int GetWidth() const { return m_Props.Width; }
	inline const unsigned int GetHeight() const { return m_Props.Height; }

	std::optional<int> ProcessMessages();

private:
	WindowProps m_Props;

	HWND m_Hwnd;
};
