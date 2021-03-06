
#include <NDK/Components/NodeComponent.hpp>
#include <NDK/Components/GraphicsComponent.hpp>
#include <NDK/Components/PhysicsComponent2D.hpp>
#include <NDK/Components/CollisionComponent2D.hpp>
#include <NDK/Components/CameraComponent.hpp>
#include <NDK/Systems/PhysicsSystem2D.hpp>
#include <NDK/StateMachine.hpp>
#include <Nazara/Math/Rect.hpp>
#include <Nazara/Physics2D/Collider2D.hpp>
#include <Nazara/Graphics/Sprite.hpp>
#include "Components/CameraScreenScaleComponent.h"
#include "Components/PlayerCommandsComponent.h"
#include "Components/PlayerControlerComponent.h"
#include "Systems/PlayerControlerSystem.h"
#include "Env.h"
#include "Constants.h"
#include "TestState.h"

constexpr float gravity = 30;

Ndk::EntityHandle createCamera(Ndk::World & w);
Ndk::EntityHandle createStaticCube(Ndk::World & w, const Nz::Rectf & rect, const Nz::Color & color);
Ndk::EntityHandle createPlayer(Ndk::World & w, const Nz::Vector2f & pos);

TestState::TestState()
{
	createCamera(m_world);
	createStaticCube(m_world, Nz::Rectf(-10, 5, 20, 1), Nz::Color::White);
	createStaticCube(m_world, Nz::Rectf(-10,-3,  1, 8), Nz::Color::White);
	createStaticCube(m_world, Nz::Rectf( -7,-3,  1, 6), Nz::Color::White);
	createStaticCube(m_world, Nz::Rectf( -1,-3,  1, 6), Nz::Color::White);
	createStaticCube(m_world, Nz::Rectf(  2, 2,  3, 1), Nz::Color::White);
	createStaticCube(m_world, Nz::Rectf(  6, 0,  1, 1), Nz::Color::White);
	createStaticCube(m_world, Nz::Rectf(  2,-3,  1, 1), Nz::Color::White);

	createPlayer(m_world, Nz::Vector2f(0, 2));

	m_world.AddSystem<PlayerControlerSystem>().SetMaximumUpdateRate(60);

	m_world.GetSystem<Ndk::PhysicsSystem2D>().GetWorld().SetGravity(Nz::Vector2f(0, gravity));

	m_world.GetSystem<Ndk::PhysicsSystem2D>().SetMaximumUpdateRate(60);

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
	box->SetCollisionId(CollisionId::Ground);
	box->SetCollisionGroup(CollisionGroup::Ground);
	box->SetCategoryMask(static_cast<Nz::UInt32>(CategoryMaskFlags(CategoryMask::Ground)));
	box->SetCollisionMask(static_cast<Nz::UInt32>(CategoryMaskFlags(0xFFFF)));
	auto & collider = e->AddComponent<Ndk::CollisionComponent2D>(box);

	auto & render = e->AddComponent<Ndk::GraphicsComponent>();
	auto sprite = Nz::Sprite::New();
	sprite->SetColor(color);
	sprite->SetSize(rect.width, rect.height);
	sprite->SetOrigin(Nz::Vector2f(rect.width / 2, rect.height / 2));
	render.Attach(sprite);

	return e;
}

Ndk::EntityHandle createPlayer(Ndk::World & w, const Nz::Vector2f & pos)
{
	const Nz::Vector2f size( 1, 1 );
	const Nz::Color color(150, 219, 144);

	auto e = w.CreateEntity();
	auto & node = e->AddComponent<Ndk::NodeComponent>();

	auto box = Nz::BoxCollider2D::New(size);
	box->SetCollisionId(CollisionId::Player);
	box->SetCollisionGroup(CollisionGroup::Player);
	box->SetCategoryMask(static_cast<Nz::UInt32>(CategoryMaskFlags(CategoryMask::Player)));
	box->SetCollisionMask(static_cast<Nz::UInt32>(CategoryMaskFlags(CategoryMask::Ground)));
	auto & collider = e->AddComponent<Ndk::CollisionComponent2D>(box);

	auto & physics = e->AddComponent<Ndk::PhysicsComponent2D>();
	physics.SetPosition(pos);

	auto & render = e->AddComponent<Ndk::GraphicsComponent>();
	auto sprite = Nz::Sprite::New();
	sprite->SetColor(color);
	sprite->SetSize(size);
	sprite->SetOrigin(size / 2.0f);
	render.Attach(sprite);

	e->AddComponent<PlayerCommandsComponent>();
	auto & controler = e->AddComponent<PlayerControlerComponent>();
	controler.initialize();

	return e;
}
