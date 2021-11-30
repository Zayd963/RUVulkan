#include <iostream>
#include <SDL.h>
#undef main
#include <vulkan.h>
#include "core/Engine.h"
int main(int argc, char* argv[])
{
	Engine engine;
	if (engine.Init())
	{
		engine.Run();
	}
	engine.Shutdown();
	return 0;
}