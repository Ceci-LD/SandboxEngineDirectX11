#include "Application.h"

Application* Application::s_Instance = nullptr;

Application::Application(WindowProps props)
{
	if (!s_Instance) s_Instance = this;
	m_Window = std::make_unique<Window>(props);
}

Application::~Application()
{
}

void Application::Run()
{
	while (true)
	{
		if (const auto ecode = m_Window->ProcessMessages())
		{
			return;
		}

		Update(0.0f);
		Render();
	}
}

void Application::Update(float dt)
{
}

void Application::Render()
{
}
