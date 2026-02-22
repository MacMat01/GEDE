#pragma once
using namespace Ogre;
using namespace OgreBites;

class PlayerAvatar
{
	public:
	PlayerAvatar(SceneManager* scene_manager, String mesh_file_name);

	void Update(Ogre::Real delta_time, const Uint8* state);

	void Move(Ogre::Vector3 translate_vector, float rotation, Ogre::Real delta_time);

	Ogre::Radian GetRotation(const Ogre::Vector3& vec);

	void SetIdleAnimationLoop();
	void SetRunAnimationLoop();

	void StopAnimationLoop(void) const;

	void StartAnimationLoop(void) const;

	void ToggleSwords();

	SceneNode* GetNode() const { return entity_node_; }

	private:
		SceneManager* scene_manager_;
		Entity* entity_;
		SceneNode* entity_node_;
		AnimationState* animation_state_base_;
		AnimationState* animation_state_top_;
		float rotation_;
		float rotation_speed_;
		float walking_speed_;

		Entity* sword_left_;
		Entity* sword_right_;
		bool swords_drawn_;
};

