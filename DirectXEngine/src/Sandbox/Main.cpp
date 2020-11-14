#include "AppWindow.h"

int main()
{
	AppWindow* app = new AppWindow();
	if (app->Init())
	{
		while (app->IsRunning())
		{
			app->PollEvents();
		}
	}

	return 0;
}