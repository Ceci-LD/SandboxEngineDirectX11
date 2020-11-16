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

		m_Window->Gfx().ClearBuffer(0.2f, 0.2f, 0.2f);
		m_Window->Gfx().DrawTriangle(0.0f,
			m_Window->GetMouse().GetPosX() / 640.0f - 1.0f,
			-m_Window->GetMouse().GetPosY() / 360.0f + 1.0f
		); 
		m_Window->Gfx().EndFrame();
	}
}

void Application::Update(float dt)
{
}

void Application::Render()
{
}
