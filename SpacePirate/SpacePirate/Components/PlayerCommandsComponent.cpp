#include <Nazara/Math/Vector2.hpp>
#include "PlayerCommandsComponent.h"
#include "Env.h"

Ndk::ComponentIndex PlayerCommandsComponent::componentIndex;

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

	commands.jumping = Nz::Keyboard::IsKeyPressed(Nz::Keyboard::Space);
	commands.dashing = Nz::Keyboard::IsKeyPressed(Nz::Keyboard::Num0);

	return commands;
}
