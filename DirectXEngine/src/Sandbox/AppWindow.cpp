#include "AppWindow.h"

AppWindow::AppWindow(WindowProps props)
	: Window(props)
{
}

AppWindow::~AppWindow()
{
}

void AppWindow::OnCreate()
{
	Graphics::Get()->Init();
}

void AppWindow::OnUpdate()
{
}

void AppWindow::OnDestroy()
{
	Graphics::Get()->Release();
}
