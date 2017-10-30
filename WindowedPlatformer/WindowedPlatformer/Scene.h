#pragma once

#include <NDK/World.hpp>

class Scene
{
public:
	Scene();
	~Scene() = default;
	Scene(const Scene &) = delete;
	Scene & operator=(const Scene &) = delete;

private:
	Ndk::World m_world;
};

