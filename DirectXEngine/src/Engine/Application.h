#pragma once

#include "Window/Window.h"

class Application
{
public:
	Application(WindowProps props = WindowProps());
	virtual ~Application();

	void Run();

	void Update(float dt);
	void Render();

	static Application& Get() { return *s_Instance; }

private:
	std::unique_ptr<Window> m_Window;

private:
	static Application* s_Instance;
};
