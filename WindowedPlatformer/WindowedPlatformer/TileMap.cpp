#include "TileMap.h"

TileMap::TileMap(unsigned int width, unsigned int height, const Tile & t)
	: m_tiles(width, height, t)
	, m_colliders(nullptr)
{

}

void TileMap::setTile(unsigned int x, unsigned int y, const Tile & t)
{
	m_tiles(x, y) = t;
	for(auto map : m_backTilemaps)
		updateRender(map, x, y, TilemapLayer::BACK);
	for (auto map : m_frontTilemaps)
		updateRender(map, x, y, TilemapLayer::FRONT);

	updateCollisions();
}

void TileMap::setFrontTile(unsigned int x, unsigned int y, unsigned int frontID)
{
	m_tiles(x, y).frontID = frontID;
	for (auto map : m_frontTilemaps)
		updateRender(map, x, y, TilemapLayer::FRONT);
	updateCollisions();
}

void TileMap::setBackTile(unsigned int x, unsigned int y, unsigned int backID)
{
	m_tiles(x, y).backID = backID;
	for (auto map : m_backTilemaps)
		updateRender(map, x, y, TilemapLayer::BACK);
	updateCollisions();
}

Tile TileMap::getTile(unsigned int x, unsigned int y) const
{
	return m_tiles(x, y);
}

unsigned int TileMap::getFrontTile(unsigned int x, unsigned int y) const
{
	return m_tiles(x, y).frontID;
}

unsigned int TileMap::getBackTile(unsigned int x, unsigned int y) const
{
	return m_tiles(x, y).backID;
}

void TileMap::updateRender(Nz::TileMapRef map, TilemapLayer layer)
{
	map->DisableTiles();

	for(unsigned int i(0) ; i < m_tiles.width() ; i++)
		for (unsigned int j(0); j < m_tiles.height(); j++)
		{
			auto id = getTileID(i, j, layer);
			if (id == 0)
				continue;
			map->EnableTile(Nz::Vector2ui(i, j), rectFromTileID(id));
		}
}

void TileMap::updateRender(Nz::TileMapRef map, unsigned int x, unsigned int y, TilemapLayer layer)
{
	auto id = getTileID(x, y, layer);
	if (id == 0)
		map->DisableTile(Nz::Vector2ui(x, y));
	else map->EnableTile(Nz::Vector2ui(x, y), rectFromTileID(id));
}

void TileMap::updateCollisions()
{
	if (m_colliders == nullptr)
		return;

	std::vector<Nz::Collider2DRef> colliders;

	for (unsigned int i(0); i < m_tiles.width(); i++)
		for (unsigned int j(0); j < m_tiles.height(); j++)
		{
			auto tile = getTile(i, j);
			auto colliderType = colliderFromTileID(tile.frontID);
			if (colliderType != TileColliderType::EMPTY)
			{
				auto collider = createCollider(i, j, colliderType);
				collider->SetCollisionId((unsigned int)(colliderIDFromTileID(tile.frontID)));
				colliders.push_back(collider);
			}
			colliderType = colliderFromTileID(tile.backID);
			if (colliderType != TileColliderType::EMPTY)
			{
				auto collider = createCollider(i, j, colliderType);
				collider->SetCollisionId((unsigned int)(colliderIDFromTileID(tile.backID)));
				colliders.push_back(collider);
			}
		}
	m_colliders->SetGeom(Nz::CompoundCollider2D::New(colliders));
}

Nz::Collider2DRef TileMap::createCollider(float x, float y, TileColliderType type)
{
	switch (type)
	{
	case TileColliderType::FULL:
		return Nz::BoxCollider2D::New(Nz::Rectf(x, y, 1, 1));
	case TileColliderType::TRIANGLE_LEFT:
	{
		Nz::Vector2f p1[]{ { x + 1, y },{ x + 1, y + 1 },{ x, y + 1 } };
		return Nz::ConvexCollider2D::New(p1, 3);
	}
	case TileColliderType::TRIANGLE_RIGHT:
	{
		Nz::Vector2f p2[]{ { x, y },{ x + 1, y + 1 },{ x, y + 1 } };
		return Nz::ConvexCollider2D::New(p2, 3);
	}
	case TileColliderType::TRIANGLE_DOWN_LEFT:
	{
		Nz::Vector2f p3[]{ { x, y },{ x + 1, y },{ x + 1, y + 1 } };
		return Nz::ConvexCollider2D::New(p3, 3);
	}
	case TileColliderType::TRIANGLE_DOWN_RIGHT:
	{
		Nz::Vector2f p4[]{ { x, y },{ x + 1, y },{ x, y + 1 } };
		return Nz::ConvexCollider2D::New(p4, 3);
	}
	case TileColliderType::HALF_TOP:
		return Nz::BoxCollider2D::New(Nz::Rectf(x, y, 1, 0.5f));
	case TileColliderType::HALF_BOTTOM:
		return Nz::BoxCollider2D::New(Nz::Rectf(x, y + 0.5f, 1, 0.5f));
	case TileColliderType::VERTICAL_CENTER:
		return Nz::BoxCollider2D::New(Nz::Rectf(x + 0.25f, y, 0.5f, 1));
	}
	return Nz::NullCollider2D::New();
}

unsigned int TileMap::getTileID(unsigned int x, unsigned int y, TilemapLayer layer)
{
	if (layer == TilemapLayer::BACK)
		return getBackTile(x, y);
	return getFrontTile(x, y);
}
