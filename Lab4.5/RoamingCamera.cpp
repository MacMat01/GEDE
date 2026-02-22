#include "pch.h"
#include "RoamingCamera.h"
RoamingCamera::RoamingCamera(SceneManager* scene_manager, RenderWindow*
	render_window, Vector3 position, Vector3 lookat_position)
{
	scene_manager_ = scene_manager;
	// Creating and attaching the camera
	camera_yaw_node_ = scene_manager->getRootSceneNode()->createChildSceneNode("camera_yaw_node_");
	camera_pitch_node_ = camera_yaw_node_->createChildSceneNode("camera_pitch_node_");

	camera_ = scene_manager->createCamera("RoamingCamera");
	camera_pitch_node_->attachObject(camera_);

	camera_yaw_node_->setPosition(position);
	camera_pitch_node_->lookAt(lookat_position, Ogre::Node::TransformSpace::TS_WORLD);


	Ogre::ColourValue fadeColour(0.9, 0.9, 0.9);

	// Configuring the camera
	camera_->setNearClipDistance(1);
	Ogre::Viewport* vp = render_window->addViewport(camera_);
	vp->setBackgroundColour(fadeColour);
	camera_->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp -> getActualHeight()));

	// Add fog that starts 50 units away from the camera, ends at 300 units, and increases in a linear fashion
	scene_manager_->setFog(Ogre::FOG_LINEAR, fadeColour, 0, 50, 300);

	// Setting motion parameters
	movement_speed_ = 7.0f;
	rotation_speed_ = 0.8f;
}

RoamingCamera::~RoamingCamera()
{
}

void RoamingCamera::update(Ogre::Real delta_time, const Uint8* keyboard_state)
{
	int x = 0, y = 0;
	// Leave if right mouse button is not being pressed
	// ...but also retrieve and store mouse movement
	if (!(SDL_GetRelativeMouseState(&x, &y) & SDL_BUTTON_RMASK)) return;
	// Construct displacement vector
	Ogre::Vector3 vec = Ogre::Vector3(0, 0, 0);
	if (keyboard_state[SDL_SCANCODE_W]) {
		vec = Ogre::Vector3(0, 0, -1);
	}
	if (keyboard_state[SDL_SCANCODE_S]) {
		vec += Ogre::Vector3(0, 0, 1);
	}
	if (keyboard_state[SDL_SCANCODE_A]) {
		vec += Ogre::Vector3(-1, 0, 0);
	}
	if (keyboard_state[SDL_SCANCODE_D]) {
		vec += Ogre::Vector3(1, 0, 0);
	}
	if (keyboard_state[SDL_SCANCODE_Q]) {
		vec += Ogre::Vector3(0, 1, 0);
	}
	if (keyboard_state[SDL_SCANCODE_E]) {
		vec += Ogre::Vector3(0, -1, 0);
	}// Construct view rotation
	float rotX = x * delta_time * -1 * rotation_speed_;
	float rotY = y * delta_time * -1 * rotation_speed_;

	// Update camera with new displacement and rotation
	camera_yaw_node_->yaw(Ogre::Radian(rotX));
	camera_pitch_node_->pitch(Ogre::Radian(rotY));

	Ogre::Vector3 direction = (camera_yaw_node_->getOrientation() * camera_pitch_node_->getOrientation()) * vec;

	camera_yaw_node_->translate(delta_time * movement_speed_ * direction);
}