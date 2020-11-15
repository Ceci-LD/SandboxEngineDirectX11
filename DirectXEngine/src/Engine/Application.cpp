#include "Application.h"

#include <iostream>

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
	if (m_Window->GetKeyboard().KeyIsPressed('A'))
		std::cout << "A\n";

	std::pair<int, int> pos = m_Window->GetMouse().GetPos();
	std::cout << pos.first << ", " << pos.second << std::endl;
}

void Application::Render()
{
}
