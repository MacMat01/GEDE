#pragma once
#include "pch.h"
#include "PlayerAvatar.h"
#include "RoamingCamera.h"
#include "PickupManager.h"
#include "InputManager.h"

using namespace Ogre;
using namespace OgreBites;

class MyEngine : public ApplicationContext, public InputListener
{
public:
	MyEngine();
	virtual ~MyEngine() {};
	void setup();
	bool keyPressed(const OgreBites::KeyboardEvent& evt);
	void setupSceneManager(void);
	void setupCamera(void);
	void setupInputManager(void);
	void populateScene(void);

	bool frameStarted(const Ogre::FrameEvent& evt);

private:
	Root* root_;
	SceneManager* scene_manager_;
	PlayerAvatar* player_;
	RoamingCamera* roaming_camera_;
	InputManager* input_manager_;
};