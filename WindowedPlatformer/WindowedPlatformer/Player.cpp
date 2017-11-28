#include "Player.h"
#include "Enums.h"
#include <NDK/Components/GraphicsComponent.hpp>
#include <NDK/Components/CollisionComponent2D.hpp>
#include <Nazara/Graphics/Sprite.hpp>
#include <Nazara/Platform/Keyboard.hpp>
#include <Nazara/Physics2D/PhysWorld2D.hpp>
#include <Nazara/Math/Algorithm.hpp>
#include <iostream>

const float width = 0.6f;
const float height = 0.8f;
const float acceleration = 3.0f;
const float maxSpeed = 5.0f;
const float jumpPower = 5.0f;

Player::Player()
	: m_physics(nullptr) 
	, m_animator(createAnimator())
	, m_grounded(false)
	, m_jumpPressed(false)
{
	
}

void Player::attachGraphicEntity(Ndk::EntityHandle graphicEntity, unsigned int setLayer, float scale)
{
	auto & animatorComponent = graphicEntity->AddComponent<Animator2DComponent>(m_animator);
	auto & nodeComponent = graphicEntity->AddComponent<Ndk::NodeComponent>();
	nodeComponent.SetScale(scale / 53);
	auto sprite = Nz::Sprite::New(Nz::Material::New("Translucent2D"));
	sprite->SetTexture("Res/Img/Set" + std::to_string(setLayer) + "/Player.png");
	auto & graphicComponent = graphicEntity->AddComponent<Ndk::GraphicsComponent>();
	graphicComponent.Attach(sprite);
	animatorComponent.attach(sprite);
	m_renders.emplace_back(&nodeComponent, &animatorComponent, scale);
}

void Player::attachPhysicEntity(Ndk::EntityHandle physicEntity, const Nz::Vector2f & pos)
{
	auto& nodeComponent = physicEntity->AddComponent<Ndk::NodeComponent>();
	nodeComponent.SetPosition(pos);
	auto collider = Nz::BoxCollider2D::New(Nz::Rectf(-width / 2, height, width, height));
	collider->SetCollisionId((unsigned int)(ColliderID::PLAYER));
	physicEntity->AddComponent<Ndk::CollisionComponent2D>(collider);
	m_physics = &physicEntity->AddComponent<Ndk::PhysicsComponent2D>();
	m_physics->SetPosition(pos);
	
}

void Player::update(float elapsedTime)
{
	if (m_physics == nullptr)
		return;

	bool jumping = false;
	if (Nz::Keyboard::IsKeyPressed(Nz::Keyboard::Up))
	{
		if (!m_jumpPressed && m_grounded)
		{
			jumping = true;
			m_jumpPressed = true;
		}
	}
	else m_jumpPressed = false;

	auto velocity = m_physics->GetVelocity();
	if (jumping)
		velocity.y = -jumpPower;

	float dir = Nz::Keyboard::IsKeyPressed(Nz::Keyboard::Right) - Nz::Keyboard::IsKeyPressed(Nz::Keyboard::Left);
	if (std::abs(dir) < 0.1f)
		dir = (velocity.x < 0) - (velocity.x > 0);
	dir *= acceleration * elapsedTime;
	velocity.x += dir;
	velocity.x = Nz::Clamp(velocity.x, -maxSpeed, maxSpeed);
	if (std::abs(dir) > std::abs(velocity.x))
		velocity.x = 0;

	m_physics->SetVelocity(velocity);

	auto pos = m_physics->GetPosition();
	for (auto & it : m_renders)
		it.node->SetPosition(Nz::Vector3f(pos.x * it.scale, pos.y * it.scale, it.node->GetPosition().z));
	//std::cout << m_physics->GetPosition().x << " " << m_physics->GetPosition().y << std::endl;
}

void Player::createCallbacks(Nz::PhysWorld2D & physWorld)
{
	Nz::PhysWorld2D::Callback groundCallbacks;
	groundCallbacks.startCallback = [this](Nz::PhysWorld2D& world, Nz::RigidBody2D& bodyA, Nz::RigidBody2D& bodyB, void*)
	{
		onTouchGround();
		return true;
	};

	groundCallbacks.endCallback = [this](Nz::PhysWorld2D& world, Nz::RigidBody2D& bodyA, Nz::RigidBody2D& bodyB, void*)
	{
		onExitGround();
	};

	physWorld.RegisterCallbacks((unsigned int)(ColliderID::PLAYER), (unsigned int)(ColliderID::GROUND), groundCallbacks);
}

void Player::setProperty(const std::string & key, int value)
{
	for (auto & it : m_renders)
	{
		it.animator->setProperty(key, value);
	}
}

Animator2DRef Player::createAnimator()
{
	auto anim = Animation2D::New("Idle");
	anim->addFrame(Frame(100, Nz::Rectui(246, 0, 45, 53)));
	return Animator2D::New(anim);
}

void Player::onTouchGround()
{
	m_grounded = true;
	auto velocity = m_physics->GetVelocity();
	velocity.y = 0;
	m_physics->SetVelocity(velocity);
}

void Player::onExitGround()
{
	m_grounded = false;
}