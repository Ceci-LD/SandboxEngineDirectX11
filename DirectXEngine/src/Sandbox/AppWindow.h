#pragma once

#include "Engine/Window/Window.h"

class AppWindow : public Window
{
public:
	AppWindow(WindowProps props = WindowProps());
	~AppWindow();

	virtual void OnCreate() override;
	virtual void OnUpdate() override;
	virtual void OnDestroy() override;

private:

};