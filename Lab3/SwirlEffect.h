#pragma once
#include "pch.h"
#include "IPickupEffect.h"

using namespace Ogre;
using namespace OgreBites;

// SWIRL EFFECT
// A rotational motion effect that moves a scene node along a trajectory around its
// center defined by a velocity vector specified in cylindrical coordinates
class SwirlEffect : public IPickupEffect { // TODO: have SwirlEffect class implement IPickupEffect
public:

	// CONSTRUCTOR
	// scene_nnode: The node that gets moved
	// cylindrical_velocity_vector: Defines the motion in cylindrical coordinates
	//     in the form (up speed, out speed, angular speed)
	SwirlEffect(SceneNode* scene_node, Ogre::Vector3 cylindrical_velocity_vector);

	// Gets the current position relative to the original node position
	Ogre::Vector3 getCylindrical() const;	// "Native" cylindrical coordinates (x=height, y=radius, z=angle)
	Ogre::Vector3 getCartesian() const;		// Converted to Cartesian coordinates (x, y, z)

	void run() override;					// Starts the motion along the trajectory
	void update(float delta_time) override;	// Steps the motion along the trajectory
	bool isRunning() override;				// True if motion is ongoing
	bool isFinished() override;				// False if motion has run its course

private:
	SceneNode* scene_node_;		// The node to be moved
	Ogre::Vector3 center_pos_;	// The original node position (motion is relative to this)

	Ogre::Vector3 cylindrical_velocity_vector_;	// What gets added to position every second
	Ogre::Vector3 start_cylindrical_pos_;		// Position at beginning of motion
	Ogre::Vector3 current_cylindrical_pos_;		// Current position during motion
	Ogre::Vector3 end_cylindrical_pos_;			// Position that triggers the finish of effect

	bool effect_running_;
	bool effect_finished_;
};

