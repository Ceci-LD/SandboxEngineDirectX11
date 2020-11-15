#include "SandboxApp.h"

int main(int argv, int** argc)
{
	Application* app = new SandboxApp();
	app->Run();
	delete app;
	return 0;
}