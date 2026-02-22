#include "pch.h"
#include "MyEngine.h"

int main(int argc, char** argv)
{
	try
	{
		MyEngine engine;
		engine.initApp();
		engine.getRoot()->startRendering();
		engine.closeApp();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error occurred during execution: " << e.what() << '\n';
		return 1;
	}

	return 0;
}