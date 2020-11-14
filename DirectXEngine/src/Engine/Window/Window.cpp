#include "Window.h"

Window* s_Window;

Window::Window(WindowProps props)
	: m_Props(props)
{
}

Window::~Window()
{
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//Window events
	switch (msg)
	{
		case WM_CREATE:
		{
			s_Window->OnCreate();
			break;
		}

		case WM_DESTROY:
		{
			//Request to terminate
			s_Window->OnDestroy();
			s_Window->SetRunning(false);
			PostQuitMessage(0);
			break;
		}

		default:
			return DefWindowProc(hwnd, msg, wparam, lparam);
	}

	return NULL;
}

bool Window::Init()
{
	WNDCLASSEX wc = {};
	wc.cbClsExtra = NULL;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbWndExtra = NULL;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = NULL;
	wc.lpszClassName = "WindowClass";
	wc.lpszMenuName = "";
	wc.style = NULL;
	wc.lpfnWndProc = WndProc;

	if (!RegisterClassEx(&wc))
	{
		return false;
	}

	if (!s_Window)
		s_Window = this;

	m_Hwnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, 
		"WindowClass", 
		m_Props.Title,
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 
		m_Props.Width, m_Props.Height,
		NULL, NULL, NULL, NULL);
	if (!m_Hwnd)
	{
		return false;
	}

	ShowWindow(m_Hwnd, SW_SHOW);
	UpdateWindow(m_Hwnd);

	m_IsRunning = true;
	return true;
}

bool Window::Release()
{
	if (!DestroyWindow(m_Hwnd))
	{
		return false;
	}

	return true;
}

void Window::PollEvents()
{
	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	s_Window->OnUpdate();

	//In order to not throttle the CPU
	Sleep(0);
}
