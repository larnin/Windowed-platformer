#pragma once

#include "Tile.h"
#include "Utility/Matrix.h"
#include <Nazara/Graphics/TileMap.hpp>
#include <Nazara/Physics2D/Collider2D.hpp>
#include <NDK/Components/CollisionComponent2D.hpp>
#include <vector>

class TileMap
{
	enum class TilemapLayer
	{
		BACK,
		FRONT,
	};

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

	inline void attachBackTilemap(Nz::TileMapRef tilemap) { m_backTilemaps.push_back(tilemap);  updateRender(tilemap, TilemapLayer::BACK); }
	inline void detachBackTilemap(Nz::TileMapRef tilemap) { m_backTilemaps.erase(std::remove(m_backTilemaps.begin(), m_backTilemaps.end(), tilemap), m_backTilemaps.end()); }
	inline void attachFrontTilemap(Nz::TileMapRef tilemap) { m_frontTilemaps.push_back(tilemap);  updateRender(tilemap, TilemapLayer::FRONT); }
	inline void detachFrontTilemap(Nz::TileMapRef tilemap) { m_frontTilemaps.erase(std::remove(m_frontTilemaps.begin(), m_frontTilemaps.end(), tilemap), m_frontTilemaps.end()); }
	inline void detachAllTilemap() { m_backTilemaps.clear(); m_frontTilemaps.clear(); }
	inline void attachColliders(Ndk::CollisionComponent2D & colliders) { m_colliders = &colliders; }

private:
	void updateRender(Nz::TileMapRef map, TilemapLayer layer);
	void updateRender(Nz::TileMapRef map, unsigned int x, unsigned int y, TilemapLayer layer);
	void updateCollisions();
	Nz::Collider2DRef createCollider(float x, float y, TileColliderType type);

	unsigned int getTileID(unsigned int x, unsigned int y, TilemapLayer layer);

	Matrix<Tile> m_tiles;

	std::vector<Nz::TileMapRef> m_backTilemaps;
	std::vector<Nz::TileMapRef> m_frontTilemaps;
	Ndk::CollisionComponent2D* m_colliders;
};

