#include "../stdafx.h"

#include "WorldTile.h"
#include "GrassTile.h"
#include "WorldMap.h"
#include "Kingdom.h"

#define S2D (Sandbox2D::GetSingleton())

GrassTile::GrassTile(const Pixel position) : WorldTile(position)
{
}

GrassTile::~GrassTile()
{
}

void GrassTile::tick(const double deltaTime)
{
}

void GrassTile::draw(Graphics* g)
{
	g->setColor(_color);
	const auto tileRect = Rect<int>{
		_position,
		{ _position.x + TILE_SIZE, _position.y + TILE_SIZE }
	};
	g->drawRect(true, tileRect);

	g->setColor(_owner == nullptr ? MakeRGBA(0, 0, 0) : _owner->getColor());
	g->drawRect(false, tileRect);
}