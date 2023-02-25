#pragma once

class Tile {
public:
	int x;
	int y;
	bool isPassable;

	// monster
	// treasure?
	// exit?

	Tile();
	Tile(int _x, int _y, bool _passable);
	bool operator==(const Tile& rhs) const;
};