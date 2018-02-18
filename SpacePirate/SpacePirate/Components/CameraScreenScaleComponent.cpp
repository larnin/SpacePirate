#include <NDK/Components/CameraComponent.hpp>
#include <NDK/Components/NodeComponent.hpp>

#include "CameraScreenScaleComponent.h"
#include "Env.h"

Ndk::ComponentIndex CameraScreenScaleComponent::componentIndex;

CameraScreenScaleComponent::CameraScreenScaleComponent(float scale)
	: m_resizeHolder(Event<ResizedEvent>::connect([this](auto e) {onResized(e); }))
	, m_scale(scale)
{
	
}

CameraScreenScaleComponent::CameraScreenScaleComponent(const CameraScreenScaleComponent & c)
	: m_resizeHolder(Event<ResizedEvent>::connect([this](auto e) {onResized(e); }))
	, m_scale(c.m_scale)
{

}

CameraScreenScaleComponent::CameraScreenScaleComponent(CameraScreenScaleComponent && c)
	: m_resizeHolder(Event<ResizedEvent>::connect([this](auto e) {onResized(e); }))
	, m_scale(std::move(c.m_scale))
{

}

void CameraScreenScaleComponent::updateScreenScale()
{
	auto size = Env::instance().window().GetSize();
	onResized(ResizedEvent{ Nz::WindowEvent::SizeEvent{ size.y, size.x } });
}

void CameraScreenScaleComponent::onResized(const ResizedEvent & e)
{
	auto & cam = GetEntity()->GetComponent<Ndk::CameraComponent>();
	auto & node = GetEntity()->GetComponent<Ndk::NodeComponent>();
	auto size = Env::instance().window().GetSize();

	cam.SetSize(Nz::Vector2f(size.x, size.y) / m_scale);
	node.SetPosition(Nz::Vector2f(-(size.x / 2.0f), -(size.y / 2.0f)) / m_scale, Nz::CoordSys_Local);
}
