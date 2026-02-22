#include "pch.h"
#include "PlayerAvatar.h"

PlayerAvatar::PlayerAvatar(SceneManager* scene_manager, String mesh_file_name)
	: scene_manager_(scene_manager),
	entity_(nullptr),
	entity_node_(nullptr),
	animation_state_base_(nullptr),
	animation_state_top_(nullptr),
	sword_left_(nullptr),
	sword_right_(nullptr),
	swords_drawn_(false),
	sword_toggle_cooldown_(0.0f)
{
	entity_ = scene_manager_->createEntity(mesh_file_name);
	entity_->setCastShadows(true);
	entity_->getSubEntity(1)->setMaterialName("Sinbad/Body2");
	entity_node_ = scene_manager_->getRootSceneNode()->createChildSceneNode();
	entity_node_->attachObject(entity_);
	rotation_ = 0.0;
	rotation_speed_ = 5.0f;
	walking_speed_ = 3.0f;
	sword_toggle_cooldown_ = 0.0f;

	// Create swords and attach to sheaths on Sinbad's back
	sword_left_ = scene_manager_->createEntity("SwordL", "Sword.mesh");
	sword_right_ = scene_manager_->createEntity("SwordR", "Sword.mesh");
	entity_->attachObjectToBone("Sheath.L", sword_left_);
	entity_->attachObjectToBone("Sheath.R", sword_right_);
}

void PlayerAvatar::Update(Ogre::Real delta_time, const Uint8* state)
{
	float starting_rotation_ = rotation_;

	// Toggle swords on Q press (with cooldown to prevent rapid toggling)
	if (sword_toggle_cooldown_ > 0.0f)
		sword_toggle_cooldown_ -= delta_time;

	if (state[SDL_SCANCODE_Q] && sword_toggle_cooldown_ <= 0.0f) {
		ToggleSwords();
		sword_toggle_cooldown_ = 0.1f;
	}

	// Leave if the right mouse button is being pressed (different function)
	if ((SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_RMASK)) return;
	// Construct the movement
	Ogre::Vector3 translate_vector(0, 0, 0);
	Ogre::Vector3 dir(sin(rotation_), 0, cos(rotation_));

	if (state[SDL_SCANCODE_W]) {
		translate_vector = walking_speed_ * dir;
	}
	if (state[SDL_SCANCODE_S]) {
		translate_vector = walking_speed_ * -1 * dir;
	}
	if (state[SDL_SCANCODE_A]) {
		rotation_ += delta_time * rotation_speed_;
	}
	if (state[SDL_SCANCODE_D]) {
		rotation_ -= delta_time * rotation_speed_;
	}

	Move(translate_vector, rotation_, delta_time);

	const bool is_walking = translate_vector != Ogre::Vector3(0, 0, 0);
	const bool is_rotating = rotation_ != starting_rotation_;

	if (is_walking || is_rotating)
	{
		Move(translate_vector, rotation_, delta_time);
		SetRunAnimationLoop();
	}
	else
	{
		SetIdleAnimationLoop();
	}

	animation_state_base_->addTime(delta_time);     animation_state_top_->addTime(delta_time);
}

void PlayerAvatar::Move(Ogre::Vector3 translate_vector, float rotation, Ogre::Real delta_time)
{
	entity_node_->translate(translate_vector * delta_time * walking_speed_);
	entity_node_->resetOrientation();
	entity_node_->yaw(Ogre::Radian(rotation));
}

Ogre::Radian PlayerAvatar::GetRotation(const Ogre::Vector3& vec)
{
	Ogre::Vector3 v(vec);
	v.normalise();
	return ((v.x <= 0 ? -1.0f : 1.0f) * Ogre::Radian(acosf(v.z)));
}

void PlayerAvatar::SetIdleAnimationLoop()
{
	if (animation_state_top_ == nullptr)
	{
		animation_state_base_ = entity_->getAnimationState("IdleBase");
		animation_state_top_ = entity_->getAnimationState("IdleTop");
		StartAnimationLoop();

	}
	// Stop other animation if there is any already running
	else if (animation_state_top_->getAnimationName() != "IdleTop")
	{
		StopAnimationLoop();
		animation_state_base_ = entity_->getAnimationState("IdleBase");
		animation_state_top_ = entity_->getAnimationState("IdleTop");
		StartAnimationLoop();
	}
}

void PlayerAvatar::SetRunAnimationLoop()
{
	if (animation_state_top_ == nullptr)
	{
		animation_state_base_ = entity_->getAnimationState("RunBase");
		animation_state_top_ = entity_->getAnimationState("RunTop");
		StartAnimationLoop();
	}
	else if (animation_state_top_->getAnimationName() != "RunTop")
	{
		StopAnimationLoop();
		animation_state_base_ = entity_->getAnimationState("RunBase");
		animation_state_top_ = entity_->getAnimationState("RunTop");
		StartAnimationLoop();
	}
}

void PlayerAvatar::StopAnimationLoop(void) const
{
	if (animation_state_base_ != nullptr &&
		animation_state_base_->getEnabled())
	{
		animation_state_base_->setTimePosition(0.0f);
		animation_state_base_->setEnabled(false);
	}
	if (animation_state_top_ != nullptr &&
		animation_state_top_->getEnabled())
	{
		animation_state_top_->setTimePosition(0.0f);
		animation_state_top_->setEnabled(false);
	}
}

void PlayerAvatar::StartAnimationLoop(void) const
{
	if (animation_state_base_ != nullptr &&
		!animation_state_base_->getEnabled())
	{
		animation_state_base_->setLoop(true);
		animation_state_base_->setEnabled(true);
	}
	if (animation_state_top_ != nullptr &&
		!animation_state_top_->getEnabled())
	{
		animation_state_top_->setLoop(true);
		animation_state_top_->setEnabled(true);
	}
}

void PlayerAvatar::ToggleSwords()
{
	if (swords_drawn_)
	{
		// Move swords back to sheaths
		entity_->detachObjectFromBone(sword_left_);
		entity_->detachObjectFromBone(sword_right_);
		entity_->attachObjectToBone("Sheath.L", sword_left_);
		entity_->attachObjectToBone("Sheath.R", sword_right_);

		// Relax hands
		entity_->getAnimationState("HandsClosed")->setEnabled(false);
		entity_->getAnimationState("HandsRelaxed")->setEnabled(true);
	}
	else
	{
		// Move swords to hands
		entity_->detachObjectFromBone(sword_left_);
		entity_->detachObjectFromBone(sword_right_);
		entity_->attachObjectToBone("Handle.L", sword_left_);
		entity_->attachObjectToBone("Handle.R", sword_right_);

		// Close hands around hilts
		entity_->getAnimationState("HandsRelaxed")->setEnabled(false);
		entity_->getAnimationState("HandsClosed")->setEnabled(true);
	}

	swords_drawn_ = !swords_drawn_;
}