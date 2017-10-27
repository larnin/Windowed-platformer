#include <NDK/Application.hpp>
#include <Nazara/Renderer.hpp>
#include <NDK/Systems.hpp>
#include <NDK/Components.hpp>
#include <cmath>
#include <iostream>
#include <Nazara/Platform/Mouse.hpp>

int main()
{
	Ndk::Application application;

	Nz::RenderWindow& mainWindow = application.AddWindow<Nz::RenderWindow>(Nz::VideoMode(500, 500, 32), "Poop party", Nz::WindowStyle_Closable | Nz::WindowStyle_Threaded);
	mainWindow.SetFramerateLimit(60);

	Ndk::World& world = application.AddWorld();
	world.GetSystem<Ndk::RenderSystem>().SetGlobalUp(Nz::Vector3f::Down());

	Ndk::EntityHandle viewEntity = world.CreateEntity();
	viewEntity->AddComponent<Ndk::NodeComponent>();

	Ndk::CameraComponent& viewer = viewEntity->AddComponent<Ndk::CameraComponent>();
	viewer.SetTarget(&mainWindow);
	viewer.SetProjectionType(Nz::ProjectionType_Orthogonal);

	Nz::TextSpriteRef textSprite = Nz::TextSprite::New();
	textSprite->Update(Nz::SimpleTextDrawer::Draw("Hello world !", 72));

	Ndk::EntityHandle text = world.CreateEntity();
	Ndk::NodeComponent& nodeComponent = text->AddComponent<Ndk::NodeComponent>();
	Ndk::GraphicsComponent& graphicsComponent = text->AddComponent<Ndk::GraphicsComponent>();
	graphicsComponent.Attach(textSprite);

	auto & handler = mainWindow.GetEventHandler();
	handler.OnMoved.Connect([&nodeComponent](const Nz::EventHandler*, const Nz::WindowEvent::PositionEvent & e)
	{
		Nz::Vector2f pos(100, 100);
		nodeComponent.SetPosition(pos.x - e.x, pos.y - e.y, 0);
	});
	/*handler.OnMouseMoved.Connect([&nodeComponent, &mainWindow](const Nz::EventHandler*, const Nz::WindowEvent::MouseMoveEvent & e)
	{
		if (!Nz::Mouse::IsButtonPressed(Nz::Mouse::Left))
			return;
		mainWindow.SetPosition(mainWindow.GetPosition() + Nz::Vector2i(e.deltaX, e.deltaY));
		nodeComponent.SetPosition(nodeComponent.GetPosition() - Nz::Vector3f(e.deltaX, e.deltaY, 0));
	});*/

	while (application.Run())
	{
		

		mainWindow.Display();
	}

	return EXIT_SUCCESS;
}