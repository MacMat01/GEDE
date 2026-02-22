#pragma once
using namespace Ogre;
using namespace OgreBites;

class PlayerAvatar
{
	public:
	PlayerAvatar(SceneManager* scene_manager, String mesh_file_name);

	void Update(Ogre::Real delta_time);

	void Move(Ogre::Vector3 translate_vector, Ogre::Real delta_time);

	Ogre::Radian GetRotation(const Ogre::Vector3& vec);

	void SetIdleAnimationLoop();
	void SetRunAnimationLoop();

	void StopAnimationLoop(void) const;

	void StartAnimationLoop(void) const;

	SceneNode* GetNode() const { return entity_node_; }

	private:
		SceneManager* scene_manager_;
		Entity* entity_;
		SceneNode* entity_node_;
		AnimationState* animation_state_base_;
		AnimationState* animation_state_top_;
};

