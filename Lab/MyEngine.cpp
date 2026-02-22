#include "pch.h"
#include "MyEngine.h"

using namespace Ogre;
using namespace OgreBites;

MyEngine::MyEngine() : ApplicationContext("T-637-GEDE Lab")
{
}

void MyEngine::setup()
{
	ApplicationContext::setup();
	addInputListener(this);
}
