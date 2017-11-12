#pragma once

enum class TileLayer
{
	LAYER_FRONT,
	LAYER_BACK
};

enum class TileColliderType
{
	EMPTY,
	FULL,
	TRIANGLE_LEFT,
	TRIANGLE_RIGHT,
	TRIANGLE_DOWN_LEFT,
	TRIANGLE_DOWN_RIGHT,
	HALF_TOP,
	HALF_BOTTOM,
	VERTICAL_CENTER,
};

enum class ColliderID
{
	PLAYER,
	GROUND,
	PLATFORM,
	WATER,
	LADDER,
	SPIKES,
	UNSTABLE_BLOCK,
	BUTTON,
	EXIT,
	//add enemies
};