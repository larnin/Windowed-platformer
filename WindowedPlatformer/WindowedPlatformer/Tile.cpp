#include "Tile.h"

unsigned int idInRow = 9;
unsigned int tileSize = 128;

namespace
{
	Nz::Vector2ui posFromTileID(unsigned int id)
	{
		if (id > 0)
			id--;
		return Nz::Vector2ui(id % idInRow, id / idInRow);
	}
}

Nz::Rectui rectFromTileID(unsigned int id)
{
	auto pos = posFromTileID(id);
	return Nz::Rectui(pos.x * tileSize, pos.y * tileSize, tileSize, tileSize);
}

TileLayer layerFromTileID(unsigned int id)
{
	auto pos = posFromTileID(id);

	if (pos.y <= 2)
		return TileLayer::LAYER_BACK;
	if (pos.y == 3 && pos.x <= 5)
		return TileLayer::LAYER_FRONT;
	if (pos.y == 9 && pos.x <= 1)
		return TileLayer::LAYER_FRONT;
	if(pos.y <= 10)
		return TileLayer::LAYER_BACK;
	if (pos.y == 11 && pos.x >= 6)
		return TileLayer::LAYER_BACK;
	return TileLayer::LAYER_FRONT;
}

TileColliderType colliderFromTileID(unsigned int id)
{
	if (id = 0)
		return TileColliderType::EMPTY;

	auto pos = posFromTileID(id);

	if (pos.y == 0 && pos.x >= 5)
		return TileColliderType::HALF_TOP;
	if (pos.y == 1 && (pos.x == 1 || pos.x == 3))
		return TileColliderType::TRIANGLE_LEFT;
	if (pos.y == 1 && (pos.x == 2 || pos.x == 4))
		return TileColliderType::TRIANGLE_RIGHT;
	if (pos.x == 7 && (pos.y == 1 || pos.y == 2))
		return TileColliderType::TRIANGLE_DOWN_LEFT;
	if (pos.x == 8 && (pos.y == 1 || pos.y == 2))
		return TileColliderType::TRIANGLE_RIGHT;
	if (pos.y == 3 && pos.x <= 6)
		return TileColliderType::EMPTY;
	if (pos.y <= 5)
		return TileColliderType::FULL;
	if (pos.y == 9 && pos.x <= 1)
		return TileColliderType::HALF_BOTTOM;
	if (pos.y == 9 && pos.x >= 6)
		return TileColliderType::VERTICAL_CENTER;
	if (pos.y <= 9)
		return TileColliderType::EMPTY;
	if (pos.y == 10 && pos.x <= 3)
		return TileColliderType::FULL;
	if (pos.y == 10 && (pos.x == 4 || pos.x == 6))
		return TileColliderType::HALF_BOTTOM;
	if (pos.y == 11 && pos.x >= 6)
		return TileColliderType::FULL;
	return TileColliderType::EMPTY;
}

ColliderID colliderIDFromTileID(unsigned int id)
{
	if (id == 0)
		return ColliderID::NONE;

	auto pos = posFromTileID(id);

	if (pos.y == 0 && pos.x >= 5)
		return ColliderID::PLATFORM;
	if (pos.y <= 2)
		return ColliderID::GROUND;
	if (pos.y == 3 && pos.x >= 7)
		return ColliderID::WATER;
	if (pos.y == 3)
		return ColliderID::NONE;
	if (pos.y <= 5)
		return ColliderID::GROUND;
	if (pos.y == 9 && pos.x <= 1)
		return ColliderID::SPIKES;
	if (pos.y == 9 && pos.x >= 6)
		return ColliderID::LADDER;
	if (pos.y == 10 && pos.x <= 3)
		return ColliderID::UNSTABLE_BLOCK;
	if (pos.y == 10 && (pos.x == 4 || pos.x == 6))
		return ColliderID::BUTTON;
	if (pos.y == 11 && (pos.x == 6 || pos.x == 7))
		return ColliderID::LADDER;
	return ColliderID::NONE;
}