#include "WindowManager.h"

WindowManager::WindowManager(Ndk::Application & app, const Nz::Recti & screenRect)
	: m_app(app)
	, m_onMouseMove(false)
	, m_currentHandler(nullptr)
	, m_screenRect(screenRect)
{

}

void WindowManager::addWindow(const Nz::Recti & geometry, Ndk::CameraComponent & camera, unsigned int setIndex, unsigned int layer)
{
	m_windows.emplace_back(WindowData(m_app, geometry, camera, setIndex), layer);
	auto & w = m_windows.back().data;

	mouseButtonPressedEvent.Connect(w.eventHandler().OnMouseButtonPressed, this, &WindowManager::onMouseButtonPressed);
	mouseButtonReleasedEvent.Connect(w.eventHandler().OnMouseButtonReleased, this, &WindowManager::onMouseButtonReleased);
	mouseMouvedEvent.Connect(w.eventHandler().OnMouseMoved, this, &WindowManager::onMouseMouved);
}

void WindowManager::removeWindowsInLayer(unsigned int layer)
{
	if (m_onMouseMove)
	{
		for (auto & item : m_windows)
		{
			if (item.layer != layer)
				continue;
			if (&item.data.eventHandler() == m_currentHandler)
			{
				m_currentHandler = nullptr;
				m_onMouseMove = false;
			}
		}
	}
	m_windows.erase(std::remove_if(m_windows.begin(), m_windows.end(), [layer](const auto & item) {return item.layer == layer; }), m_windows.end());
}

void WindowManager::update(float elapsedTime)
{
	for (auto & w : m_windows)
		w.data.update(elapsedTime);
}

void WindowManager::onMouseButtonPressed(const Nz::EventHandler * handler, const Nz::WindowEvent::MouseButtonEvent & event)
{
	if (event.button == Nz::Mouse::Button::Left)
		m_onMouseMove = true;
}

void WindowManager::onMouseButtonReleased(const Nz::EventHandler * handler, const Nz::WindowEvent::MouseButtonEvent & event)
{
	if (event.button == Nz::Mouse::Button::Left)
		m_onMouseMove = false;
}

void WindowManager::onMouseMouved(const Nz::EventHandler * handler, const Nz::WindowEvent::MouseMoveEvent & event)
{
	if (!m_onMouseMove || m_currentHandler == nullptr)
		return;

	auto & w = getWindowFromHandler(m_currentHandler);
	auto delta = moveWindow(w, Nz::Vector2i(event.deltaX, event.deltaY));
	w.data.move(delta);
}

WindowManager::WindowInfos & WindowManager::getWindowFromHandler(const Nz::EventHandler * handler)
{
	for (auto & w : m_windows)
	{
		if (&w.data.eventHandler() == handler)
			return w;
	}

	NazaraAssert(false, "The handler is not from one listed window");
	return m_windows.front();
}


Nz::Vector2i WindowManager::moveWindow(WindowManager::WindowInfos & w, Nz::Vector2i offset)
{
	auto rect = w.data.getGeometry();
	int distX = offset.x;
	for (auto & w2 : m_windows)
	{
		if (&w == &w2 || w2.layer != w.layer)
			continue;
		auto destRect = Nz::Recti(rect).Translate(Nz::Vector2i(distX, 0));
		auto rect2 = w2.data.getGeometry();
		if (!destRect.Intersect(rect2))
			continue;
		if (distX < 0)
			distX = rect2.x + rect2.width - rect.x;
		else distX = rect2.x - rect.x - rect.width;
	}

	int distY = offset.y;
	for (auto & w2 : m_windows)
	{
		if (&w == &w2 || w2.layer != w.layer)
			continue;
		auto destRect = Nz::Recti(rect).Translate(Nz::Vector2i(distX, distY));
		auto rect2 = w2.data.getGeometry();
		if (!destRect.Intersect(rect2))
			continue;
		if (distY < 0)
			distY = rect2.y + rect2.height - rect.y;
		else distY = rect2.y - rect.y - rect.height;
	}

	return Nz::Vector2i(distX, distY);
}


