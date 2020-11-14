#pragma once

#include <Windows.h>

//Helper class
struct WindowProps
{
	const char* Title;
	unsigned int Width, Height;
	WindowProps(const char* title = "DirectX Sandbox",
		unsigned int width = 1280,
		unsigned int height = 720)
		: Title(title), Width(width), Height(height)
	{}
};

//Low level Window class
class Window
{
public:
	Window(WindowProps props);
	~Window();

	bool Init();
	bool Release();
	void PollEvents();

	inline const bool IsRunning() const { return m_IsRunning; }
	inline void SetRunning(bool enable) { m_IsRunning = enable; }

	//Events
	virtual void OnCreate() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnDestroy() = 0;

private:
	HWND m_Hwnd;
	bool m_IsRunning = false;

	WindowProps m_Props;
};
