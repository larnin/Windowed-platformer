#pragma once

#include "Tile.h"
#include "Utility\Matrix.h"
#include <Nazara\Graphics\TileMap.hpp>
#include <Nazara\Physics2D\Collider2D.hpp>
#include <vector>

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

	inline void attachTilemap(Nz::TileMapRef tilemap) { m_tilemaps.push_back(tilemap); }
	inline void detachTilemap(Nz::TileMapRef tilemap) { m_tilemaps.erase(std::remove(m_tilemaps.begin(), m_tilemaps.end(), tilemap), m_tilemaps.end()); }
	inline void detachAllTilemap() { m_tilemaps.clear(); }
	inline void attachColliders(Nz::CompoundCollider2DRef colliders) { m_colliders = colliders; }

private:
	void updateRender();
	void updateRender(unsigned int x, unsigned int y);
	void updateCollisions();

	Matrix<Tile> m_tiles;

	std::vector<Nz::TileMapRef> m_tilemaps;
	Nz::CompoundCollider2DRef m_colliders;
};

