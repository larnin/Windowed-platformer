#include "Player.h"
#include "Enums.h"
#include <NDK/Components/GraphicsComponent.hpp>
#include <NDK/Components/CollisionComponent2D.hpp>
#include <Nazara/Graphics/Sprite.hpp>
#include <Nazara/Platform/Keyboard.hpp>

const float width = 0.6f;
const float height = 0.8f;
const float speed = 3.0f;

Player::Player()
	: m_physics(nullptr) 
	, m_animator(createAnimator())
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

	/*Nz::Vector2f dir(Nz::Keyboard::IsKeyPressed(Nz::Keyboard::Right) - Nz::Keyboard::IsKeyPressed(Nz::Keyboard::Left)
		, Nz::Keyboard::IsKeyPressed(Nz::Keyboard::Down) - Nz::Keyboard::IsKeyPressed(Nz::Keyboard::Up));
	m_physics->SetVelocity(dir * speed);*/

	//todo controles & co

	auto pos = m_physics->GetPosition();
	for (auto & it : m_renders)
		it.node->SetPosition(Nz::Vector3f(pos.x * it.scale, pos.y * it.scale, it.node->GetPosition().z));
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