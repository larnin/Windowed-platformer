#pragma once

#include "WindowData.h"
#include <Nazara/Core/Signal.hpp>
#include <Nazara/Math/Vector2.hpp>
#include <NDK/Application.hpp>
#include <memory>
#include <vector>

class WindowManager
{
	struct WindowInfos
	{
		WindowInfos(std::unique_ptr<WindowData> && _data, unsigned int _layer)
			: data(std::move(_data)), layer(_layer)
		{
		}

		WindowInfos(WindowInfos &&) = default;
		WindowInfos & operator=(WindowInfos &&) = default;

		std::unique_ptr<WindowData> data;
		unsigned int layer;

		NazaraSlot(Nz::EventHandler, OnMouseButtonPressed, mouseButtonPressedEvent);
	};

public:
	WindowManager(Ndk::Application & app, const Nz::Recti & screenRect);
	~WindowManager() = default;

	WindowData& addWindow(const Nz::Recti & geometry, unsigned int setIndex, unsigned int layer);
	void removeWindowsInLayer(unsigned int layer);

	void update(float elapsedTime);

	inline size_t windowCount() const { return m_windows.size(); }

private:
	void onMouseButtonPressed(const Nz::EventHandler* handler, const Nz::WindowEvent::MouseButtonEvent & event);
	void onMouseMouved();
	Nz::Vector2i moveWindow(WindowInfos & w, Nz::Vector2i offset);

	WindowInfos& getWindowFromHandler(const Nz::EventHandler* handler);

	Ndk::Application & m_app;
	std::vector<WindowInfos> m_windows;

	bool m_onMouseMove;
	const Nz::EventHandler* m_currentHandler;

	Nz::Recti m_screenRect;
	Nz::Vector2i m_clickPosition;
};

