#include "TileMap.h"

TileMap::TileMap(unsigned int width, unsigned int height, const Tile & t)
	: m_tiles(width, height, t)
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

}

unsigned int TileMap::getTileID(unsigned int x, unsigned int y, TilemapLayer layer)
{
	if (layer == TilemapLayer::BACK)
		return getBackTile(x, y);
	return getFrontTile(x, y);
}
