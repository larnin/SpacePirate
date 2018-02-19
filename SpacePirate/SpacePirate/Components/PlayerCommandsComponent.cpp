#include <Nazara/Math/Vector2.hpp>
#include "PlayerCommandsComponent.h"
#include "Env.h"

constexpr Nz::Keyboard::Key dashKey = Nz::Keyboard::N;
constexpr Nz::Keyboard::Key jumpKey = Nz::Keyboard::Space;

Ndk::ComponentIndex PlayerCommandsComponent::componentIndex;

PlayerCommandsComponent::PlayerCommandsComponent()
	: m_keyPressedHolder(Event<KeyPressedEvent>::connect([this](auto e) {onKeyPressed(e); }))
	, m_dashing(false)
	, m_jumping(false)
{

}

PlayerCommandsComponent::PlayerCommandsComponent(const PlayerCommandsComponent & c)
	: m_keyPressedHolder(Event<KeyPressedEvent>::connect([this](auto e) {onKeyPressed(e); }))
	, m_dashing(c.m_dashing)
	, m_jumping(c.m_jumping)
{

}

PlayerCommandsComponent::PlayerCommandsComponent(PlayerCommandsComponent && c)
	: m_keyPressedHolder(Event<KeyPressedEvent>::connect([this](auto e) {onKeyPressed(e); }))
	, m_dashing(std::move(c.m_dashing))
	, m_jumping(std::move(c.m_jumping))
{

}

PlayerCommands PlayerCommandsComponent::evaluateControles()
{
	PlayerCommands commands;

	if (!Env::instance().window().HasFocus())
		return PlayerCommands();

	Nz::Vector2f dir(0, 0);

	if (Nz::Keyboard::IsKeyPressed(Nz::Keyboard::Left) || Nz::Keyboard::IsKeyPressed(Nz::Keyboard::Q))
		dir.x--;
	if (Nz::Keyboard::IsKeyPressed(Nz::Keyboard::Right) || Nz::Keyboard::IsKeyPressed(Nz::Keyboard::D))
		dir.x++;
	if (Nz::Keyboard::IsKeyPressed(Nz::Keyboard::Up) || Nz::Keyboard::IsKeyPressed(Nz::Keyboard::Z))
		dir.y--;
	if (Nz::Keyboard::IsKeyPressed(Nz::Keyboard::Down) || Nz::Keyboard::IsKeyPressed(Nz::Keyboard::S))
		dir.y++;
	commands.direction = dir;

	commands.jumping = Nz::Keyboard::IsKeyPressed(jumpKey);
	commands.dashing = m_dashing;
	commands.startJump = m_jumping;

	m_dashing = false;
	m_jumping = false;

	return commands;
}

void PlayerCommandsComponent::onKeyPressed(const KeyPressedEvent & e)
{
	if (e.value.repeated)
		return;

	if (e.value.code == jumpKey)
		m_jumping = true;
	else if (e.value.code == dashKey)
		m_dashing = true;
}
