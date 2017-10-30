#include <NDK/Application.hpp>
#include <Nazara/Renderer.hpp>
#include <NDK/Systems.hpp>
#include <NDK/Components.hpp>
#include <cmath>
#include <iostream>
#include <Nazara/Platform/Mouse.hpp>
#include <Nazara/Platform/Keyboard.hpp>

int main()
{
	Ndk::Application application;

	std::cout << Nz::VideoMode::GetDesktopMode().width << " " << Nz::VideoMode::GetDesktopMode().height << std::endl;

	Nz::RenderWindow& mainWindow = application.AddWindow<Nz::RenderWindow>(Nz::VideoMode(500, 500, 32), "Poop party",/* Nz::WindowStyle_Closable |*/ Nz::WindowStyle_Threaded);
	mainWindow.SetFramerateLimit(60);
	mainWindow.SetStayOnTop(true);

	Ndk::World& world = application.AddWorld();
	world.GetSystem<Ndk::RenderSystem>().SetGlobalUp(Nz::Vector3f::Down());

	Ndk::EntityHandle viewEntity = world.CreateEntity();
	Ndk::NodeComponent& nodeComponent = viewEntity->AddComponent<Ndk::NodeComponent>();

	Ndk::CameraComponent& viewer = viewEntity->AddComponent<Ndk::CameraComponent>();
	viewer.SetTarget(&mainWindow);
	viewer.SetProjectionType(Nz::ProjectionType_Orthogonal);

	Nz::TextSpriteRef textSprite = Nz::TextSprite::New();
	textSprite->Update(Nz::SimpleTextDrawer::Draw("Hello world !", 72));

	Ndk::EntityHandle text = world.CreateEntity();
	text->AddComponent<Ndk::NodeComponent>();
	Ndk::GraphicsComponent& graphicsComponent = text->AddComponent<Ndk::GraphicsComponent>();
	graphicsComponent.Attach(textSprite);

	auto & handler = mainWindow.GetEventHandler();

	Nz::Vector2i clickPosition(0, 0);
	Nz::Vector2f originalPos = nodeComponent.GetPosition() + Nz::Vector2f(mainWindow.GetPosition());

	handler.OnMouseButtonPressed.Connect([&clickPosition](const Nz::EventHandler*, const Nz::WindowEvent::MouseButtonEvent & e)
	{
		std::cout << "click" << std::endl;
		clickPosition = Nz::Vector2i(e.x, e.y);
	});

	handler.OnMouseMoved.Connect([&nodeComponent, &mainWindow, &clickPosition, originalPos](const Nz::EventHandler*, const Nz::WindowEvent::MouseMoveEvent & e)
	{
		std::cout << "move" << std::endl;
		if (!Nz::Mouse::IsButtonPressed(Nz::Mouse::Left))
			return;

		auto pos = Nz::Mouse::GetPosition();

		mainWindow.SetPosition(pos - clickPosition);
		nodeComponent.SetPosition(- originalPos + Nz::Vector2f(mainWindow.GetPosition()));
	});

	handler.OnKeyPressed.Connect([&mainWindow](const Nz::EventHandler*, const Nz::WindowEvent::KeyEvent & e)
	{
		if (e.code == Nz::Keyboard::Escape)
			mainWindow.Close();
	});

	while (application.Run())
	{
		mainWindow.Display();
	}

	return EXIT_SUCCESS;
}