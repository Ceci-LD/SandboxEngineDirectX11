#include "Window.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (msg)
	{
		case WM_CREATE:
			break;
		case WM_CLOSE:
			DestroyWindow(hWnd);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return 0;
}

Window::Window(WindowProps props)
	: m_Props(props)
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = 0;
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.hIconSm = nullptr;
	wc.lpszClassName = "WindowClassName";
	wc.lpszMenuName = "WindowMenuName";
	wc.lpfnWndProc = WndProc;

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Window Registration Failed!", "Window",
			MB_ICONEXCLAMATION | MB_OK);
	}

	// create window & get hWnd
	m_Hwnd = CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW,
		"WindowClassName", m_Props.Title,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT,
		m_Props.Width, m_Props.Height,
		nullptr, nullptr, nullptr, nullptr
	);

	if (m_Hwnd == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Window",
			MB_ICONEXCLAMATION | MB_OK);
	}

	//Show and Update window
	ShowWindow(m_Hwnd, SW_SHOWDEFAULT);
	UpdateWindow(m_Hwnd);
}

std::optional<int> Window::ProcessMessages()
{
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return (int)msg.wParam;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return {};
}

Window::~Window()
{
	DestroyWindow(m_Hwnd);
}

