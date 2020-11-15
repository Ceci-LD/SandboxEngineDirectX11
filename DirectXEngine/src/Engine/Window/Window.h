#pragma once
#include <Windows.h>
#include <memory>
#include <string>
#include <optional>

#include "Keyboard/Keyboard.h"
#include "Mouse/Mouse.h"

#include "Engine/GraphicsEngine/Graphics.h"

struct WindowProps
{
	const char* Title;
	unsigned int Width;
	unsigned int Height;
	HINSTANCE HInst;
	WindowProps(const char* title = "DirectX Engine",
		unsigned int width = 1280,
		unsigned int height = 720)
		: Title(title), Width(width), Height(height), HInst(GetModuleHandle(nullptr))
	{}
	~WindowProps() { UnregisterClass("WindowClassName", HInst); }
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

	const Keyboard& GetKeyboard() { return m_Keyboard; }
	const Mouse& GetMouse() { return m_Mouse; }
	Graphics& Gfx();

private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	WindowProps m_Props;

	HWND m_Hwnd;
	Keyboard m_Keyboard;
	Mouse m_Mouse;
	std::unique_ptr<Graphics> m_Graphics;
};
