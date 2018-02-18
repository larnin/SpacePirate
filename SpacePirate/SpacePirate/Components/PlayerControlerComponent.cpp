
#include "PlayerControlerComponent.h"
#include "Constants.h"
#include <NDK/World.hpp>
#include <NDK/Systems/PhysicsSystem2D.hpp>
#include <algorithm>
#include <limits>

Ndk::ComponentIndex PlayerControlerComponent::componentIndex;

constexpr float maxSpeed = 5;
constexpr float maxFallSpeed = 10;
constexpr float accelerationMultiplier = 20;
constexpr float airAccelerationMultiplier = 10;
constexpr float decelerationMultiplier = 5;
constexpr float deadZoneCommands = 0.2f;
constexpr float jumpPower = 5;
constexpr float jumpMaxDelay = 0.1f;
constexpr float jumpMaxTime = 0.5f;

PlayerControlerComponent::PlayerControlerComponent()
	: m_grounded(true)
	, m_jumpingTime(0)
	, m_jumped(false)
{
}

void PlayerControlerComponent::update(const PlayerCommands & commands, float elapsedTime)
{
	auto & physics = GetEntity()->GetComponent<Ndk::PhysicsComponent2D>();

	physics.SetRotation(0);
	physics.SetAngularVelocity(0);

	checkGrounded(physics.GetPosition());
	move(commands.direction, physics, elapsedTime);
	jump(commands.startJump, commands.jumping, physics, elapsedTime);
	
}

void PlayerControlerComponent::move(const Nz::Vector2f & dir, Ndk::PhysicsComponent2D & physics, float elapsedTime)
{
	auto speed = physics.GetVelocity();
	speed.y = std::min(speed.y, maxFallSpeed);

	float acceleration = m_grounded ? accelerationMultiplier : airAccelerationMultiplier;
	if (std::abs(dir.x) < deadZoneCommands)
		acceleration = 0;
	else acceleration *= dir.x;

	if (acceleration != 0)
	{
		speed.x += acceleration * elapsedTime;
		if (speed.x < -maxSpeed)
			speed.x = -maxSpeed;
		if (speed.x > maxSpeed)
			speed.x = maxSpeed;
	}
	else
	{
		acceleration = decelerationMultiplier * (speed.x > 0 ? -1 : 1) * elapsedTime;
		if (std::abs(acceleration) > std::abs(speed.x))
			acceleration = -speed.x;
		speed.x += acceleration;
	}
	physics.SetVelocity(speed);
}

void PlayerControlerComponent::jump(bool startJumping, bool jumping, Ndk::PhysicsComponent2D & physics, float elapsedTime)
{
	if (jumping)
		m_jumpingTime += elapsedTime;
	else m_jumped = false;

	if (startJumping)
	{
		m_jumpingTime = 0;
		if (m_grounded)
			m_jumped = true;
		else m_jumped = false;
	}
	if (!m_jumped && jumping && m_grounded && m_jumpingTime < jumpMaxDelay)
	{
		m_jumped = true;
		m_jumpingTime = 0;
	}

	if (m_jumpingTime < jumpMaxTime && jumping && m_jumped)
	{
		auto speed = physics.GetVelocity();
		if (speed.y > 0.01f)
			m_jumpingTime = jumpMaxTime;
		speed.y = -jumpPower;
		physics.SetVelocity(speed);
	}
}

void PlayerControlerComponent::checkGrounded(const Nz::Vector2f & pos)
{
	const float rayDistance = 0.6f;
	
	std::vector<Nz::PhysWorld2D::RaycastHit> hits;

	auto & world = GetEntity()->GetWorld()->GetSystem<Ndk::PhysicsSystem2D>().GetWorld();
	m_grounded = world.RaycastQueryFirst(pos, pos + Nz::Vector2f(0, rayDistance), 0, CollisionGroup::Player, static_cast<Nz::UInt32>(CategoryMaskFlags(CategoryMask::Ground)), std::numeric_limits<unsigned int>::max());
}
