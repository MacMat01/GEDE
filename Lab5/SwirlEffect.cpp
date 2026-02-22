#include "pch.h"
#include "SwirlEffect.h"

SwirlEffect::SwirlEffect(SceneNode* scene_node, Ogre::Vector3 cylindrical_velocity_vector)
{
	scene_node_ = scene_node;
	cylindrical_velocity_vector_ = cylindrical_velocity_vector;
	center_pos_ = scene_node_ ? scene_node_->getPosition() : Ogre::Vector3::ZERO;

	start_cylindrical_pos_ = Vector3 (0, 5, 0);
	current_cylindrical_pos_ = start_cylindrical_pos_;
	end_cylindrical_pos_ = Ogre::Vector3(5.0f, 10.0f, 0.0f);

	effect_running_ = false;
	effect_finished_ = false;
}

Ogre::Vector3 SwirlEffect::getCylindrical() const
{
	return current_cylindrical_pos_;
}

Ogre::Vector3 SwirlEffect::getCartesian() const
{
	const float height = current_cylindrical_pos_.x;
	const float radius = current_cylindrical_pos_.y;
	const float angle = current_cylindrical_pos_.z;

	return Ogre::Vector3(
		Ogre::Math::Cos(angle) * radius,
		height,
		Ogre::Math::Sin(angle) * radius);
}

void SwirlEffect::run()
{
	effect_running_ = true;
	effect_finished_ = false;
	start_cylindrical_pos_ = current_cylindrical_pos_;
}

void SwirlEffect::update(float delta_time) {
	if (effect_running_)
	{

		if (current_cylindrical_pos_.x >= end_cylindrical_pos_.x)
		{
			effect_running_ = false;
			effect_finished_ = true;
		}
		else
		{
			current_cylindrical_pos_ = current_cylindrical_pos_ + delta_time * cylindrical_velocity_vector_;
			scene_node_->setPosition(getCartesian() + center_pos_);
		}
	}
}

bool SwirlEffect::isRunning()
{
	return effect_running_;
}

bool SwirlEffect::isFinished()
{
	return effect_finished_;
}