#pragma once

#include <Nazara/Renderer/RenderWindow.hpp>
#include <Nazara/Math/Rect.hpp>
#include <Nazara/Core/Signal.hpp>
#include <NDK/Components/CameraComponent.hpp>

struct WindowMoveEvent
{
	WindowMoveEvent(int _x, int _y, int _dx, int _dy)
		: x(_x)
		, y(_y)
		, dx(_dx)
		, dy(_dy)
	{ }

	int x;
	int y;
	int dx;
	int dy;
};

class WindowData
{
public:
	WindowData(const Nz::Recti & m_geometry, Ndk::CameraComponent & camera);
	~WindowData();
	WindowData(const WindowData &) = delete;
	WindowData & operator=(const WindowData &) = delete;
	WindowData(WindowData &&) = default;
	WindowData & operator=(WindowData &&) = default;

private:
	void onWindowMouseMoved(const Nz::EventHandler*, const Nz::WindowEvent::MouseMoveEvent & e);
	void onWindowMouseButtonPressed(const Nz::EventHandler*, const Nz::WindowEvent::MouseButtonEvent & e);

	void updateCameraPosition();

	Nz::RenderWindow m_window;
	Ndk::CameraComponent & m_camera;

	NazaraSlot(Nz::EventHandler, OnMouseMoved, mouseMouved);
	NazaraSlot(Nz::EventHandler, OnMouseButtonPressed, mouseButtonPressed);
	NazaraSignal(OnWindowMoved, const WindowData *, const WindowMoveEvent &);

	NazaraSlot(Nz::EventHandler, OnKeyPressed, keyPressed);
	NazaraSlot(Nz::EventHandler, OnKeyReleased, keyReleased);
	NazaraSignal(OnKeyPressed, const WindowData*, const Nz::WindowEvent::KeyEvent&);
	NazaraSignal(OnKeyReleased, const WindowData*, const Nz::WindowEvent::KeyEvent&);
};

