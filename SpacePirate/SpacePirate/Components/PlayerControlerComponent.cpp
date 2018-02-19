
#include "PlayerControlerComponent.h"
#include "Constants.h"
#include <NDK/World.hpp>
#include <NDK/Systems/PhysicsSystem2D.hpp>
#include <algorithm>
#include <limits>

Ndk::ComponentIndex PlayerControlerComponent::componentIndex;

constexpr float maxSpeed = 5;
constexpr float maxFallSpeed = 10;
constexpr float accelerationMultiplier = 100;
constexpr float airAccelerationMultiplier = 80;
constexpr float decelerationMultiplier = 50;
constexpr float deadZoneCommands = 0.2f;
constexpr float jumpPower = 8;
constexpr float jumpMaxDelay = 0.1f;
constexpr float jumpMaxTime = 0.3f;
constexpr float groundCheckWidth = 0.4f;
constexpr float delayBetweenDash = 0.2f;
constexpr float dashSpeed = 15;
constexpr float dashDuration = 0.3f;
constexpr float wallFallSpeed = 0.2f;

PlayerControlerComponent::PlayerControlerComponent()
	: m_facing(Facing::Left)
	, m_wallState(WallState::None)
	, m_grounded(true)
	, m_jumpingTime(0)
	, m_jumped(false)
	, m_dashing(false)
	, m_dashTime(0)
	, m_canDash(true)
	, m_delayAfterDash(0)
{

}

void PlayerControlerComponent::update(const PlayerCommands & commands, float elapsedTime)
{
	auto & physics = GetEntity()->GetComponent<Ndk::PhysicsComponent2D>();

	physics.SetRotation(0);
	physics.SetAngularVelocity(0);

	checkGrounded(physics.GetPosition());
	updateDashStatus(elapsedTime);
	if (commands.dashing)
		startDash(physics);
	if (m_dashing)
		dashing(physics, elapsedTime);
	else move(commands.direction, physics, elapsedTime);
	jump(commands.startJump, commands.jumping, physics, elapsedTime);
}

void PlayerControlerComponent::move(const Nz::Vector2f & dir, Ndk::PhysicsComponent2D & physics, float elapsedTime)
{
	auto speed = physics.GetVelocity();
	speed.y = std::min(speed.y, m_wallState == WallState::None ? maxFallSpeed : wallFallSpeed);

	float acceleration = m_grounded ? accelerationMultiplier : airAccelerationMultiplier;
	if (std::abs(dir.x) < deadZoneCommands)
		acceleration = 0;
	else
	{
		m_facing = dir.x > 0 ? Facing::Right : Facing::Left;
		acceleration *= dir.x;
	}

	if (m_wallState != WallState::None)
		m_facing = m_wallState == WallState::Right ? Facing::Left : Facing::Right;

	if (acceleration != 0)
	{
		float accelerationValue = acceleration * elapsedTime;
		if (speed.x > 0)
		{
			if (speed.x > maxSpeed)
				speed.x -= decelerationMultiplier * elapsedTime;
			else  speed.x = std::min(maxSpeed, speed.x + accelerationValue);
		}
		else
		{
			if (-speed.x > maxSpeed)
				speed.x += decelerationMultiplier * elapsedTime;
			else  speed.x = std::max(-maxSpeed, speed.x + accelerationValue);
		}
	}
	else
	{
		acceleration = decelerationMultiplier * (speed.x > 0 ? -1 : 1) * elapsedTime;
		if (std::abs(acceleration) > std::abs(speed.x))
			acceleration = -speed.x;
		speed.x += acceleration;
	}

	if (speed.x < -maxSpeed)
		speed.x += decelerationMultiplier * elapsedTime;
	if (speed.x > maxSpeed)
		speed.x -= decelerationMultiplier * elapsedTime;

	physics.SetVelocity(speed);
}

void PlayerControlerComponent::jump(bool startJumping, bool jumping, Ndk::PhysicsComponent2D & physics, float elapsedTime)
{
	if (m_dashing)
		m_jumpingTime = jumpMaxTime;

	if (jumping)
		m_jumpingTime += elapsedTime;
	else m_jumped = false;

	if (startJumping)
	{
		m_jumpingTime = 0;
		if (m_grounded)
		{
			m_jumped = true;
			m_jumpDirection = Nz::Vector2f(0, -1);
		}
		else if (m_wallState != WallState::None)
		{
			m_jumped = true;
			m_jumpDirection = Nz::Vector2f(m_wallState == WallState::Left ? 0.707f : -0.707f, -0.707f); //45° diagonal
		}
		else m_jumped = false;
	}
	if (!m_jumped && jumping && (m_grounded || m_wallState != WallState::None) && m_jumpingTime < jumpMaxDelay)
	{
		if (m_grounded)
		{
			m_jumped = true;
			m_jumpDirection = Nz::Vector2f(0, -1);
		}
		else if (m_wallState != WallState::None)
		{
			m_jumped = true;
			m_jumpDirection = Nz::Vector2f(m_wallState == WallState::Left ? 0.707f : -0.707f, -0.707f); //45° diagonal
		}
		m_jumpingTime = 0;
	}

	if (m_jumpingTime < jumpMaxTime && jumping && m_jumped)
	{
		auto speed = physics.GetVelocity();
		if (speed.y > 0.01f)
			m_jumpingTime = jumpMaxTime;
		speed.y = jumpPower * m_jumpDirection.y;

		if (speed.x < m_jumpDirection.x * jumpPower && m_jumpDirection.x > 0)
			speed.x += m_jumpDirection.x * jumpPower;
		if (speed.x > m_jumpDirection.x * jumpPower && m_jumpDirection.x < 0)
			speed.x += m_jumpDirection.x * jumpPower;

		physics.SetVelocity(speed);
	}
}

void PlayerControlerComponent::checkGrounded(const Nz::Vector2f & pos)
{
	const float rayDistance = 0.6f;
	const float wallRayDistance = 0.55f;
	
	std::vector<Nz::PhysWorld2D::RaycastHit> hits;

	auto & world = GetEntity()->GetWorld()->GetSystem<Ndk::PhysicsSystem2D>().GetWorld();
	m_grounded = world.RaycastQueryFirst(pos, pos + Nz::Vector2f(0, rayDistance), 0, CollisionGroup::Player
		, static_cast<Nz::UInt32>(CategoryMaskFlags(CategoryMask::Ground)), std::numeric_limits<unsigned int>::max());
	if(!m_grounded)
		m_grounded = world.RaycastQueryFirst(pos + Nz::Vector2f(groundCheckWidth, 0), pos + Nz::Vector2f(groundCheckWidth, rayDistance), 0, CollisionGroup::Player
			, static_cast<Nz::UInt32>(CategoryMaskFlags(CategoryMask::Ground)), std::numeric_limits<unsigned int>::max());	
	if (!m_grounded)
		m_grounded = world.RaycastQueryFirst(pos + Nz::Vector2f(-groundCheckWidth, 0), pos + Nz::Vector2f(-groundCheckWidth, rayDistance), 0, CollisionGroup::Player
			, static_cast<Nz::UInt32>(CategoryMaskFlags(CategoryMask::Ground)), std::numeric_limits<unsigned int>::max());

	bool left = world.RaycastQueryFirst(pos, pos + Nz::Vector2f(-wallRayDistance, 0), 0, CollisionGroup::Player
		, static_cast<Nz::UInt32>(CategoryMaskFlags(CategoryMask::Ground)), std::numeric_limits<unsigned int>::max());
	bool right = world.RaycastQueryFirst(pos, pos + Nz::Vector2f(wallRayDistance, 0), 0, CollisionGroup::Player
		, static_cast<Nz::UInt32>(CategoryMaskFlags(CategoryMask::Ground)), std::numeric_limits<unsigned int>::max());
	if (left)
		m_wallState = WallState::Left;
	else if (right)
		m_wallState = WallState::Right;
	else m_wallState = WallState::None;
}

void PlayerControlerComponent::startDash(Ndk::PhysicsComponent2D & physics)
{
	if (!m_canDash)
		return;

	m_dashing = true;
	m_canDash = false;
	m_dashTime = 0;
	m_delayAfterDash = 0;

	float direction = (m_facing == Facing::Left ? -1 : 1) * dashSpeed;
	physics.SetVelocity(Nz::Vector2f(direction * dashSpeed, 0));
}

void PlayerControlerComponent::dashing(Ndk::PhysicsComponent2D & physics, float elapsedTime)
{
	float direction = (m_facing == Facing::Left ? -1 : 1) * dashSpeed;

	auto speed = physics.GetVelocity();
	if (std::abs(speed.x) < 0.1f)
		m_dashing = false;

	physics.SetVelocity(Nz::Vector2f(direction, 0));

	m_dashTime += elapsedTime;

	if (m_dashTime >= dashDuration)
		m_dashing = false;
}

void PlayerControlerComponent::updateDashStatus(float elapsedTime)
{
	if (m_dashing)
		m_delayAfterDash = 0;
	else m_delayAfterDash += elapsedTime;
	if ((m_grounded || m_wallState != WallState::None) && m_delayAfterDash > delayBetweenDash)
		m_canDash = true;
}
