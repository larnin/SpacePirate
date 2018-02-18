
#include <NDK/Components/NodeComponent.hpp>
#include <NDK/Components/GraphicsComponent.hpp>
#include <NDK/Components/PhysicsComponent2D.hpp>
#include <NDK/Components/CollisionComponent2D.hpp>
#include <NDK/Components/CameraComponent.hpp>
#include <NDK/StateMachine.hpp>
#include <Nazara/Math/Rect.hpp>
#include <Nazara/Physics2D/Collider2D.hpp>
#include <Nazara/Graphics/Sprite.hpp>
#include "Components/CameraScreenScaleComponent.h"
#include "Env.h"
#include "Constants.h"
#include "TestState.h"

Ndk::EntityHandle createCamera(Ndk::World & w);
Ndk::EntityHandle createStaticCube(Ndk::World & w, const Nz::Rectf & rect, const Nz::Color & color);

TestState::TestState()
{
	createCamera(m_world);
	createStaticCube(m_world, Nz::Rectf(-1, -0.5, 2, 1), Nz::Color::White);

}

void TestState::Enter(Ndk::StateMachine & fsm)
{
}

void TestState::Leave(Ndk::StateMachine & fsm)
{
}

bool TestState::Update(Ndk::StateMachine & fsm, float elapsedTime)
{
	m_world.Update(elapsedTime);

	return true;
}

Ndk::EntityHandle createCamera(Ndk::World & w)
{
	auto e = w.CreateEntity();
	auto & camera = e->AddComponent<Ndk::CameraComponent>();
	camera.SetProjectionType(Nz::ProjectionType_Orthogonal);
	auto & window = Env::instance().window();
	camera.SetSize(Nz::Vector2f(window.GetSize().x, window.GetSize().y));
	camera.SetTarget(&window);

	auto & node = e->AddComponent<Ndk::NodeComponent>();
	auto parent = w.CreateEntity();
	auto & parentNode = parent->AddComponent<Ndk::NodeComponent>();
	parentNode.SetPosition(-10.0f*camera.GetForward());
	node.SetParent(parentNode);

	auto & scaling = e->AddComponent<CameraScreenScaleComponent>(Constants::pixelsByUnit);
	scaling.updateScreenScale();

	return parent;
}

Ndk::EntityHandle createStaticCube(Ndk::World & w, const Nz::Rectf & rect, const Nz::Color & color)
{
	auto e = w.CreateEntity();
	auto & node = e->AddComponent<Ndk::NodeComponent>();
	node.SetPosition(Nz::Vector2f(rect.x + rect.width / 2, rect.y + rect.height / 2));

	auto box = Nz::BoxCollider2D::New(Nz::Vector2f(rect.width, rect.height));
	auto & collider = e->AddComponent<Ndk::CollisionComponent2D>(box);

	auto & render = e->AddComponent<Ndk::GraphicsComponent>();
	auto sprite = Nz::Sprite::New();
	sprite->SetColor(color);
	sprite->SetSize(rect.width, rect.height);
	sprite->SetOrigin(Nz::Vector2f(rect.width / 2, rect.height / 2));
	render.Attach(sprite);

	return e;
}
