#pragma once

#include "WindowData.h"
#include <Nazara/Core/Signal.hpp>
#include <Nazara/Math/Vector2.hpp>
#include <NDK/Application.hpp>
#include <vector>

class WindowManager
{
	struct WindowInfos
	{
		WindowInfos(WindowData && _data, unsigned int _layer)
			: data(std::move(_data)), layer(_layer)
		{
		}

		WindowInfos(WindowInfos && w) = default;
		WindowInfos & operator=(WindowInfos &&) = default;

		WindowData data;
		unsigned int layer;
	};

public:
	WindowManager(Ndk::Application & app, const Nz::Recti & screenRect);
	~WindowManager() = default;

	void addWindow(const Nz::Recti & geometry, Ndk::CameraComponent & camera, unsigned int setIndex, unsigned int layer);
	void removeWindowsInLayer(unsigned int layer);

	void update(float elapsedTime);

	inline size_t windowCount() const { return m_windows.size(); }

private:
	void onMouseButtonPressed(const Nz::EventHandler* handler, const Nz::WindowEvent::MouseButtonEvent & event);
	void onMouseButtonReleased(const Nz::EventHandler* handler, const Nz::WindowEvent::MouseButtonEvent & event);
	void onMouseMouved(const Nz::EventHandler* handler, const Nz::WindowEvent::MouseMoveEvent & event);
	Nz::Vector2i moveWindow(WindowInfos & w, Nz::Vector2i offset);

	WindowInfos& getWindowFromHandler(const Nz::EventHandler* handler);

	NazaraSlot(Nz::EventHandler, OnMouseButtonPressed, mouseButtonPressedEvent);
	NazaraSlot(Nz::EventHandler, OnMouseButtonReleased, mouseButtonReleasedEvent);
	NazaraSlot(Nz::EventHandler, OnMouseMoved, mouseMouvedEvent);

	Ndk::Application & m_app;
	std::vector<WindowInfos> m_windows;

	bool m_onMouseMove;
	Nz::EventHandler* m_currentHandler;

	Nz::Recti m_screenRect;
};

