#include <NDK/Application.hpp>
#include <NDK/StateMachine.hpp>
#include "GameState.h"
#include "Animation2D/Animator2DComponent.h"
#include "Animation2D/Animator2DSystem.h"


#include <iostream>

void initializeSystemsAndComponents()
{
	Ndk::InitializeComponent<Animator2DComponent>("005A2D");
	Ndk::InitializeSystem<Animator2DSystem>();
}

int main1()
{
	Ndk::Application application;
	application.MakeExitOnLastWindowClosed(false);

	initializeSystemsAndComponents();

	Ndk::StateMachine fsm(std::make_shared<GameState>(application));

	while (application.Run())
	{
		if (!fsm.Update(application.GetUpdateTime()))
			application.Quit();
	}

	return 0;
}

#include <Nazara/Renderer.hpp>
#include <NDK/Systems.hpp>
#include <NDK/Components.hpp>
#include <cmath>
#include <Nazara/Platform/Mouse.hpp>
#include <Nazara/Platform/Keyboard.hpp>
#include "WindowData.h"

int main2()
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

	auto e = world.CreateEntity();
	Ndk::CameraComponent & second = e->AddComponent<Ndk::CameraComponent>();
	e->AddComponent<Ndk::NodeComponent>();

	Nz::TextSpriteRef textSprite = Nz::TextSprite::New();
	textSprite->Update(Nz::SimpleTextDrawer::Draw("Hello world !", 72));

	Ndk::EntityHandle text = world.CreateEntity();
	text->AddComponent<Ndk::NodeComponent>();
	Ndk::GraphicsComponent& graphicsComponent = text->AddComponent<Ndk::GraphicsComponent>();
	graphicsComponent.Attach(textSprite);

	auto & handler = mainWindow.GetEventHandler();

	Nz::Vector2i clickPosition(0, 0);
	Nz::Vector2f originalPos = Nz::Vector2f(nodeComponent.GetPosition().x) + Nz::Vector2f(mainWindow.GetPosition());

	handler.OnMouseButtonPressed.Connect([&clickPosition](const Nz::EventHandler*, const Nz::WindowEvent::MouseButtonEvent & e)
	{
		clickPosition = Nz::Vector2i(e.x, e.y);
	});

	handler.OnMouseMoved.Connect([&nodeComponent, &mainWindow, &clickPosition, originalPos](const Nz::EventHandler*, const Nz::WindowEvent::MouseMoveEvent & e)
	{
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

#include <NDK/World.hpp>

int main5()
{
	Ndk::Application application;

	Nz::RenderWindow& mainWindow = application.AddWindow<Nz::RenderWindow>(Nz::VideoMode(500, 500, 32), "Poop party", Nz::WindowStyle_Closable | Nz::WindowStyle_Threaded);
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

	handler.OnMouseButtonPressed.Connect([](const Nz::EventHandler* handler, const Nz::WindowEvent::MouseButtonEvent & e)
	{
		std::cout << handler << std::endl;
	});

	while (application.Run())
	{
		mainWindow.Display();
	}

	return EXIT_SUCCESS;
}

#include <NDK/Application.hpp>
#include <NDK/Systems.hpp>
#include <NDK/Components.hpp>
#include <Nazara/Graphics/TileMap.hpp>
#include <vector>
#include <iostream>

int main4()
{
	Ndk::Application application; 
	Nz::RenderWindow& mainWindow = application.AddWindow<Nz::RenderWindow>(Nz::VideoMode(500, 500, 32), "Poop party", Nz::WindowStyle_Closable | Nz::WindowStyle_Threaded);
	mainWindow.SetFramerateLimit(60);
	mainWindow.SetStayOnTop(true);

	Ndk::World& world = application.AddWorld();
	world.GetSystem<Ndk::RenderSystem>().SetGlobalUp(Nz::Vector3f::Down());
	world.GetSystem<Ndk::PhysicsSystem2D>().GetWorld().SetGravity(Nz::Vector2f(0, 20));

	Ndk::EntityHandle viewEntity = world.CreateEntity();
	Ndk::NodeComponent& nodeComponent = viewEntity->AddComponent<Ndk::NodeComponent>();
	Ndk::CameraComponent& viewer = viewEntity->AddComponent<Ndk::CameraComponent>();
	viewer.SetTarget(&mainWindow);
	viewer.SetProjectionType(Nz::ProjectionType_Orthogonal);

	auto entitySprite = Nz::Sprite::New();
	entitySprite->SetSize(50, 50);
	auto colliderSprite = Nz::Sprite::New();
	colliderSprite->SetSize(500, 100);
	colliderSprite->SetColor(Nz::Color::Red);

	auto entity = world.CreateEntity();
	entity->AddComponent<Ndk::NodeComponent>();
	auto& colliderEntity = entity->AddComponent<Ndk::CollisionComponent2D>(Nz::BoxCollider2D::New(Nz::Rectf(0, 0, 50, 50)));
	auto& physicEntity = entity->AddComponent<Ndk::PhysicsComponent2D>();
	physicEntity.SetPosition(Nz::Vector2f(200, 50));
	auto& graphicEntity = entity->AddComponent<Ndk::GraphicsComponent>();
	graphicEntity.Attach(entitySprite);
	
	auto wall = world.CreateEntity();
	auto& nodeWall = wall->AddComponent<Ndk::NodeComponent>();
	nodeWall.SetPosition(Nz::Vector2f(0, 300));
	wall->AddComponent<Ndk::CollisionComponent2D>(Nz::BoxCollider2D::New(Nz::Rectf(0, 0, 500, 100)));
	auto& graphicWall = wall->AddComponent<Ndk::GraphicsComponent>();
	graphicWall.Attach(colliderSprite);

	while (application.Run())
	{
		//std::cout << physicEntity.GetPosition().x << "\t" << physicEntity.GetPosition().y << "\t" << physicEntity.GetVelocity().x << "\t" << physicEntity.GetVelocity().y << std::endl;
		mainWindow.Display();
	}

	return EXIT_SUCCESS;
}

const float tileSize = 50.0f;
const float playerSize = 30.0f;
const float acceleration = 100.0f;
const float maxSpeed = 50.0f;
const float jumpPower = 50.0f;
const float gravity = 100.0f;

class PlayerE
{
public:
	PlayerE(Ndk::PhysicsComponent2D & physics)
		: m_physics(physics)
		, m_jumpPressed(false)
	{ }

	void update(float time)
	{
		bool jumping = false;
		if (Nz::Keyboard::IsKeyPressed(Nz::Keyboard::Up))
		{
			if (!m_jumpPressed)
			{
				jumping = true;
				m_jumpPressed = true;
			}
		}
		else m_jumpPressed = false;

		auto velocity = m_physics.GetVelocity();
		if (jumping)
			velocity.y = -jumpPower;

		float dir = Nz::Keyboard::IsKeyPressed(Nz::Keyboard::Right) - Nz::Keyboard::IsKeyPressed(Nz::Keyboard::Left);
		if (std::abs(dir) < 0.1f)
			dir = (velocity.x < 0) - (velocity.x > 0);
		dir *= acceleration * time;
		velocity.x += dir;
		velocity.x = Nz::Clamp(velocity.x, -maxSpeed, maxSpeed);
		if (std::abs(dir) > std::abs(velocity.x))
			velocity.x = 0;

		m_physics.SetVelocity(velocity);
	}

private:
	Ndk::PhysicsComponent2D & m_physics;
	bool m_jumpPressed;
};

int getMapValue(int* map, unsigned int x, unsigned int y, unsigned int w)
{
	return map[x + y * w];
}

Nz::CompoundCollider2DRef createColliders(int* map, unsigned int w, unsigned int h)
{
	std::vector<Nz::Collider2DRef> colliders;

	for (unsigned int i(0); i < w; i++)
	{
		for (unsigned int j(0); j < h; j++)
		{
			if (getMapValue(map, i, j, w) == 0)
				continue;

			colliders.push_back(Nz::BoxCollider2D::New(Nz::Rectf(i * tileSize, j * tileSize, tileSize, tileSize)));
		}
	}

	return Nz::CompoundCollider2D::New(colliders);
}

Nz::TileMapRef createRender(int* map, unsigned int w, unsigned int h)
{
	auto tilemap = Nz::TileMap::New(Nz::Vector2ui(w, h), Nz::Vector2f(tileSize, tileSize));
	auto mat = Nz::Material::New("Translucent2D");
	mat->SetDiffuseMap("i.png");
	tilemap->SetMaterial(0, mat);

	for (unsigned int i(0); i < w; i++)
	{
		for (unsigned int j(0); j < h; j++)
		{
			if (getMapValue(map, i, j, w) == 0)
				continue;

			tilemap->EnableTile(Nz::Vector2ui(i, j), Nz::Rectui(0, 0, 1, 1));
		}
	}
	return tilemap;
}

int main12()
{
	int map[] =
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	  0, 1, 1, 1, 1, 1, 1, 1, 0, 0,
	  0, 1, 1, 1, 1, 1, 1, 1, 0, 1,
	  1, 0, 1, 0, 1, 0, 1, 0, 1, 0,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	const unsigned int mapW = 10;
	const unsigned int mapH = 8;

	Ndk::Application application;
	Nz::RenderWindow& mainWindow = application.AddWindow<Nz::RenderWindow>(Nz::VideoMode(500, 500, 32), "Poop party", Nz::WindowStyle_Closable | Nz::WindowStyle_Threaded);
	mainWindow.SetFramerateLimit(60);

	Ndk::World world;
	world.GetSystem<Ndk::RenderSystem>().SetGlobalUp(Nz::Vector3f::Down());
	world.GetSystem<Ndk::PhysicsSystem2D>().GetWorld().SetGravity(Nz::Vector2f(0, gravity));

	Ndk::EntityHandle viewEntity = world.CreateEntity();
	Ndk::NodeComponent& nodeComponent = viewEntity->AddComponent<Ndk::NodeComponent>();
	Ndk::CameraComponent& viewer = viewEntity->AddComponent<Ndk::CameraComponent>();
	viewer.SetTarget(&mainWindow);
	viewer.SetProjectionType(Nz::ProjectionType_Orthogonal);

	auto entitySprite = Nz::Sprite::New();
	entitySprite->SetSize(playerSize, playerSize);

	auto entity = world.CreateEntity();
	entity->AddComponent<Ndk::NodeComponent>();
	auto& colliderEntity = entity->AddComponent<Ndk::CollisionComponent2D>();
	colliderEntity.SetGeom(Nz::BoxCollider2D::New(Nz::Rectf(0, 0, playerSize, playerSize)));
	auto& physicEntity = entity->AddComponent<Ndk::PhysicsComponent2D>();
	physicEntity.SetMassCenter(Nz::Vector2f(playerSize / 2.0f, playerSize / 2.0f));
	physicEntity.SetPosition(Nz::Vector2f(200, 50));
	auto& graphicEntity = entity->AddComponent<Ndk::GraphicsComponent>();
	graphicEntity.Attach(entitySprite);
	PlayerE p(physicEntity);

	auto wall = world.CreateEntity();
	auto& nodeWall = wall->AddComponent<Ndk::NodeComponent>();
	nodeWall.SetPosition(Nz::Vector2f(0, 0));
	auto & collisionWall = wall->AddComponent<Ndk::CollisionComponent2D>();
	collisionWall.SetGeom(createColliders(map, mapW, mapH));
	auto& graphicWall = wall->AddComponent<Ndk::GraphicsComponent>();
	graphicWall.Attach(createRender(map, mapW, mapH));

	while (application.Run())
	{
		mainWindow.Display();
		auto time = application.GetUpdateTime();
		p.update(time);
		world.Update(time);
	}

	return EXIT_SUCCESS;
}

#include "Event/Event.h"

struct EventInt
{
	EventInt(int _value) : value(_value) {}
	int value;
};

class A
{
public:
	A(const std::string & lbl) 
		: m_event(Event<EventInt>::connect([this](auto e) {foo(e); }))
		, m_label(lbl)
	{

	}

private:
	void foo(EventInt e)
	{
		std::cout << m_label << " " << e.value << std::endl;
	}
	EventHolder<EventInt> m_event;
	std::string m_label;
};

int main()
{
	A b("From b :");
	{
		A a("From a :");
		Event<EventInt>::send({ 1 });
	}
	std::cout << "----" << std::endl;
	Event<EventInt>::send({ 1 });
	std::getchar();
}