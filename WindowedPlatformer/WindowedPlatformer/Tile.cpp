#include "Tile.h"

unsigned int idInRow = 18;

TileLayer layerFromTileID(unsigned int id)
{
	if (id == 0)
		return TileLayer::LAYER_BACK;
	id--;
	if (id % idInRow < 9) //ground
		return TileLayer::LAYER_BACK;
	if ((id >= 66 && id <= 69) || (id >= 84 && id <= 87)) //water
		return TileLayer::LAYER_BACK;
	if (id == 191 || id == 209) //spikes
		return TileLayer::LAYER_BACK;
	if (id %idInRow >= 17 || id == 159 || id == 177) //blocks & doors
		return TileLayer::LAYER_BACK;

	return TileLayer::LAYER_FRONT;
}

TileColliderType colliderFromTileID(unsigned int id)
{
	if (id == 0)
		return TileColliderType::EMPTY;
	id--;

	if (id % idInRow < 9) //ground
	{
		id %= 54;
		if (id <= 4 || id == 18 || (id >= 36 && id <= 42) || id == 23 || id == 24)
			return TileColliderType::FULL;
		if (id == 19 || id == 21)
			return TileColliderType::TRIANGLE_LEFT;
		if (id == 20 || id == 22)
			return TileColliderType::TRIANGLE_RIGHT;
		if (id <= 8)
			return TileColliderType::HALF_TOP;
		if (id == 25 || id == 43)
			return TileColliderType::TRIANGLE_DOWN_LEFT;
		if (id == 26 || id == 44)
			return TileColliderType::TRIANGLE_DOWN_RIGHT;
	}
	if (id == 191 || id == 209) //spikes
		return TileColliderType::HALF_BOTTOM;
	if (id == 190 || id == 208) //ladders
		return TileColliderType::FULL;
	if (id % idInRow < 12)
		return TileColliderType::EMPTY;
	if ((id >= 66 && id <= 69) || (id >= 84 && id <= 87)) //water
		return TileColliderType::FULL;
	if (id == 159 || id == 177) //open door
		return TileColliderType::FULL;
	if (id >= 192 && id <= 195) //unstable blocks
		return TileColliderType::FULL;
	if (id == 210 || id == 212) //buttons
		return TileColliderType::HALF_BOTTOM;
	if (id >= 174 && id <= 176) //rope
		return TileColliderType::VERTICAL_CENTER;
	if (id % idInRow < 16)
		return TileColliderType::EMPTY;
	if (id < 126)
		return TileColliderType::FULL;
	return TileColliderType::EMPTY;
}

ColliderID colliderIDFromTileID(unsigned int id)
{

}