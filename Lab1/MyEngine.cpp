#include "pch.h"
#include "MyEngine.h"

using namespace Ogre;
using namespace OgreBites;

MyEngine::MyEngine() : ApplicationContext("T-637-GEDE Lab 1")
{
}


void MyEngine::setup()
{
	// Setup
	ApplicationContext::setup();

	// Get pointers to root and create scene manager
	Root* root = getRoot();
	SceneManager* sceneManager = root->createSceneManager();

	// Register scene with the RTSS
	RTShader::ShaderGenerator* shaderGenerator = RTShader::ShaderGenerator::getSingletonPtr();
	shaderGenerator->addSceneManager(sceneManager);

	// Add light
	Light* light = sceneManager->createLight("MainLight");
	SceneNode* lightNode = sceneManager->getRootSceneNode()->createChildSceneNode();
	lightNode->attachObject(light);
	lightNode->setPosition(20, 80, 50);

	// Add camera
	SceneNode* cameraNode = sceneManager->getRootSceneNode()->createChildSceneNode();
	cameraNode->setPosition(0, 0, 15);
	cameraNode->lookAt(Vector3(0, 0, -1), Node::TS_PARENT);
	Camera* cam = sceneManager->createCamera("MainCamera");
	cam->setNearClipDistance(5);
	cam->setAutoAspectRatio(true);
	cameraNode->attachObject(cam);
	getRenderWindow()->addViewport(cam);

	// Add mesh
	Entity* ent = sceneManager->createEntity("Sinbad.mesh");
	SceneNode* sinbadNode = sceneManager->getRootSceneNode()->createChildSceneNode();
	sinbadNode->attachObject(ent);
}