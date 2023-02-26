#include "Tile.h"

Tile::Tile() : x(0), y(0), isPassable(false) {}
Tile::Tile(int _x, int _y, bool _passable) : x(_x), y(_y), isPassable(_passable), isPlayerPortal(false) {}

bool Tile::operator==(const Tile& rhs) const {
	return this->x == rhs.x &&
		this->y == rhs.y &&
		this->isPassable == rhs.isPassable;
}

