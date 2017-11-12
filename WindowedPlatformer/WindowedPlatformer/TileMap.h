#pragma once

#include "Tile.h"
#include "Utility\Matrix.h"
#include <Nazara\Graphics\TileMap.hpp>
#include <Nazara\Physics2D\Collider2D.hpp>

class TileMap
{
public:
	

	TileMap(unsigned int width, unsigned int height, const Tile & t = Tile());
	~TileMap() = default;

	void setTile(unsigned int x, unsigned int y, const Tile & t);
	void setFrontTile(unsigned int x, unsigned int y, unsigned int frontID);
	void setBackTile(unsigned int x, unsigned int y, unsigned int backID);
	Tile getTile(unsigned int x, unsigned int y) const;
	unsigned int getFrontTile(unsigned int x, unsigned int y) const;
	unsigned int getBackTile(unsigned int x, unsigned int y) const;

	inline unsigned int width() const { return m_tiles.width(); }
	inline unsigned int height() const { return m_tiles.height(); }

	inline void attachTilemap(Nz::TileMapRef tilemap) { m_tilemap = tilemap; }
	inline void attachColliders(Nz::CompoundCollider2DRef colliders) { m_colliders = colliders; }

private:
	void updateRender();
	void updateCollisions();

	Matrix<Tile> m_tiles;

	Nz::TileMapRef m_tilemap;
	Nz::CompoundCollider2DRef m_colliders;
};

