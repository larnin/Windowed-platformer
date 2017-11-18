#include "TileMap.h"

TileMap::TileMap(unsigned int width, unsigned int height, const Tile & t)
	: m_tiles(width, height, t)
{

}

void TileMap::setTile(unsigned int x, unsigned int y, const Tile & t)
{
	m_tiles(x, y) = t;
	updateRender(x, y);
	updateCollisions();
}

void TileMap::setFrontTile(unsigned int x, unsigned int y, unsigned int frontID)
{
	m_tiles(x, y).frontID = frontID;
	updateRender(x, y);
	updateCollisions();
}

void TileMap::setBackTile(unsigned int x, unsigned int y, unsigned int backID)
{
	m_tiles(x, y).backID = backID;
	updateRender(x, y);
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

void TileMap::updateRender()
{

}

void TileMap::updateRender(unsigned int x, unsigned int y)
{
}

void TileMap::updateCollisions()
{

}
