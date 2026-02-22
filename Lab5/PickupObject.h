#pragma once
#include "pch.h"
#include "IPickupObject.h"
#include "IPickupEffect.h"
#include "SwirlEffect.h"

using namespace Ogre;
using namespace OgreBites;

// PICKUP OBJECT A simple game object that can show a nice effect if being picked up
class PickupObject : public IPickupObject
{
public:
	// CONSTRUCTOR
	// scene_manager: Pointer to the main scene manager
	// mesh_file_name: The asset file name of the mesh that will represent this object visually
	// position: Where the object will be placed in world coordinates
	// scale: How much the size of the object is scaled in the x, y and z dimensions
	PickupObject(SceneManager* scene_manager, const char* mesh_file_name, Vector3
		position = Vector3(0, 0, 0), Vector3 scale = Vector3(1, 1, 1));
	~PickupObject();

	SceneNode* getSceneNode() const;		                  // Returns the scene node that holds this object
	Entity* getEntity() const;                                // Returns the Entity attached to this object
	IPickupEffect* getPickupEffect() const;                   // Returns the pickup effect attached to this object
	bool isPickedUp();                                        // True if the pickup effect has been run

	void runPickupEffect();                                   // Display a special motion effect on the game object
	bool collidesWith(SceneNode* other_node, float distance); // True if the passed node is within distance of this object
	void update(float delta_time) const;                      // Updates object, including any running motion effects

private:
	SceneManager* scene_manager_;      // The main scene manager

	SceneNode* entity_node_;           // The scene node that holds the model
	Entity* entity_;                   // The game object model
	
	bool picked_up_;                   // True if the pickup effect has been run
	IPickupEffect* pickup_effect_;     // An interesting pickup motion effect 
};