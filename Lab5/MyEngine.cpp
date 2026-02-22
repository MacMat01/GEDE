#include "pch.h"
#include "MyEngine.h"

using namespace Ogre;
using namespace OgreBites;

MyEngine::MyEngine()
	: ApplicationContext("SC-T-637-GEDE Lab 5"),
	root_(nullptr),
	scene_manager_(nullptr),
	roaming_camera_(nullptr),
	player_(nullptr)
{
}

void MyEngine::setup(void)
{
	ApplicationContext::setup();

	// Explicitly load resource locations from the Ogre SDK resources.cfg.
	// This avoids relying on the current working directory when launching from Visual Studio.
	{
		Ogre::ConfigFile cf;
		cf.load("C:/Users/macma/Documents/Ogre/OgreSdk/bin/resources.cfg");

		Ogre::String secName, typeName, archName;
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4996)
#endif
		Ogre::ConfigFile::SectionIterator secIt = cf.getSectionIterator();
#if defined(_MSC_VER)
#pragma warning(pop)
#endif
		while (secIt.hasMoreElements())
		{
			secName = secIt.peekNextKey();
			Ogre::ConfigFile::SettingsMultiMap* settings = secIt.getNext();
			for (auto it = settings->begin(); it != settings->end(); ++it)
			{
				typeName = it->first;
				archName = it->second;
				Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
			}
		}
	}

	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	addInputListener(this);
	setupSceneManager();
	setupCamera();
	populateScene();
}

bool MyEngine::keyPressed(const OgreBites::KeyboardEvent& evt)
{
	if (evt.keysym.sym == OgreBites::SDLK_ESCAPE)
	{
		getRoot()->queueEndRendering();
	}
	return true;
}

void MyEngine::setupSceneManager()
{
	// Get pointers to root and create scene manager
	root_ = getRoot();
	scene_manager_ = root_->createSceneManager();

	// Register scene with the RTSS
	RTShader::ShaderGenerator* shaderGenerator = RTShader::ShaderGenerator::getSingletonPtr();
	shaderGenerator->addSceneManager(scene_manager_);
}

void MyEngine::setupCamera()
{
	roaming_camera_ = new RoamingCamera(scene_manager_, getRenderWindow(), Vector3(0, 0, 50));
}

void MyEngine::populateScene()
{
	// Add Sinbad Model
	player_ = new PlayerAvatar(scene_manager_, "Sinbad.mesh");

	// Add Ground
	Plane plane(Ogre::Vector3::UNIT_Y, -5);
	MeshPtr groundMesh = MeshManager::getSingleton()
		.createPlane("MainGround", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			plane, 1000, 1000, 100, 100, true, 1, 50, 50, Ogre::Vector3::UNIT_Z);

	groundMesh->buildTangentVectors();

	Entity* groundEntity = scene_manager_->createEntity("LightPlaneEntity", "MainGround");
	scene_manager_->getRootSceneNode()->createChildSceneNode()->attachObject(groundEntity);
	groundEntity->setCastShadows(false);

	// Specify the material that is going to be used to render the floor tiles
	groundEntity->setMaterialName("BrickTiles");

	// Set Shadow Technique
	scene_manager_->setShadowTechnique(Ogre::ShadowTechnique::SHADOWTYPE_STENCIL_MODULATIVE);

	// Add Directional Light
	scene_manager_->setAmbientLight(Ogre::ColourValue(0.2f, 0.5f, 0.8f));
	Light* directionalLight = scene_manager_->createLight("DirectionalLight");
	directionalLight->setType(Ogre::Light::LT_DIRECTIONAL);
	directionalLight->setDiffuseColour(1, 1, 1);
	directionalLight->setSpecularColour(.4, .4, .4);
	directionalLight->setCastShadows(true); // Enable shadows for the directional light
	SceneNode* directionalLightNode = scene_manager_->getRootSceneNode()->createChildSceneNode();
	directionalLightNode->attachObject(directionalLight);
	directionalLightNode->setDirection(Ogre::Vector3(0, -1, -1));

	PickupManager::initialize(scene_manager_, player_->GetNode());
	PickupManager::addPickupObject("MyOgreCube.mesh");
}

bool MyEngine::frameStarted(const Ogre::FrameEvent& evt)
{
	// Main "game loop" of the application
	// Let parent handle this callback as well
	ApplicationContext::frameStarted(evt);
	// Store the time that has passed since last time we got the callback
	const Ogre::Real delta_time = evt.timeSinceLastFrame;
	// Check what keys of the keyboard are being pressed
	const Uint8* state = SDL_GetKeyboardState(nullptr);

	// Update any subsystems
	if (player_ != nullptr) player_->Update(delta_time, state);
	if (roaming_camera_ != nullptr) roaming_camera_->update(delta_time, state);

	// Update all the managed pickup objects
	PickupManager::Update(delta_time, state);

	return true;
}