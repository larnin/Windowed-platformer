#pragma once
#include "Enums.h"
#include <Nazara/Math/Vector2.hpp>
#include <Nazara/Math/Rect.hpp>

struct Tile
{
	Tile() : frontID(0), backID(0) { }
	Tile(unsigned int _frontID, unsigned int _backID) : frontID(_frontID), backID(_backID) {}

	unsigned int frontID;
	unsigned int backID;
};

Nz::Rectui rectFromTileID(unsigned int id);
TileLayer layerFromTileID(unsigned int id);
TileColliderType colliderFromTileID(unsigned int id);
ColliderID colliderIDFromTileID(unsigned int id);