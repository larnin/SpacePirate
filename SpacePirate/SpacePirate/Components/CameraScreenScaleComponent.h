#pragma once

#include <NDK/Component.hpp>
#include "Event/WindowEventArgs.h"
#include "Event/Event.h"

class CameraScreenScaleComponent : public Ndk::Component<CameraScreenScaleComponent>
{
public :
	CameraScreenScaleComponent(float scale);
	CameraScreenScaleComponent(const CameraScreenScaleComponent & c);
	CameraScreenScaleComponent(CameraScreenScaleComponent && c);

	void updateScreenScale();

	static Ndk::ComponentIndex componentIndex;

private:
	void onResized(const ResizedEvent & e);

	EventHolder<ResizedEvent> m_resizeHolder;

	float m_scale;
};