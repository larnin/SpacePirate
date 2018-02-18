#pragma once

#include <NDK/Component.hpp>
#include <NDK/Components/PhysicsComponent2D.hpp>
#include "PlayerCommandsComponent.h"

class PlayerControlerComponent : public Ndk::Component<PlayerControlerComponent>
{
public:
	PlayerControlerComponent();

	void update(const PlayerCommands & commands, float elapsedTime);

	static Ndk::ComponentIndex componentIndex;

private:
	void move(const Nz::Vector2f & dir, Ndk::PhysicsComponent2D & physics, float elapsedTime);
	void jump(bool startJumping, bool jumping, Ndk::PhysicsComponent2D & physics, float elapsedTime);
	void checkGrounded(const Nz::Vector2f & pos);

	float m_grounded;
	float m_jumpingTime;
	bool m_jumped;
};