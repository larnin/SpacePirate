#pragma once

#include <NDK/Component.hpp>
#include <NDK/Components/PhysicsComponent2D.hpp>
#include "PlayerCommandsComponent.h"

class PlayerControlerComponent : public Ndk::Component<PlayerControlerComponent>
{
	enum class Facing
	{
		Left,
		Right,
	};

	enum class WallState
	{
		None,
		Left,
		Right,
	};

public:
	PlayerControlerComponent();
	void initialize();

	static Ndk::ComponentIndex componentIndex;

private:
	void update(const PlayerCommands & commands, Ndk::PhysicsComponent2D & physics, float elapsedTime);
	void move(const Nz::Vector2f & dir, Ndk::PhysicsComponent2D & physics, float elapsedTime);
	void jump(bool startJumping, bool jumping, Ndk::PhysicsComponent2D & physics, float elapsedTime);
	void startJump();
	void checkGrounded(const Nz::Vector2f & pos);
	void startDash(Ndk::PhysicsComponent2D & physics);
	void dashing(Ndk::PhysicsComponent2D & physics, float elapsedTime);
	void updateDashStatus(float elapsedTime);

	void updateVelocityFunction(const Nz::Vector2f& gravity, float damping, float elapsedTime);

	Facing m_facing;
	WallState m_wallState;
	float m_grounded;
	float m_jumpingTime;
	bool m_lastFrameZero;
	Nz::Vector2f m_jumpDirection;
	bool m_jumped;
	bool m_dashing;
	float m_dashTime;
	bool m_canDash;
	float m_delayAfterDash;
	float m_wallRideTime;
};