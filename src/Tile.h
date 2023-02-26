#pragma once

class Tile {
public:
	int x;
	int y;
	bool isPassable;
	bool isPlayerPortal; // this is where the player starts and has to go back to...

	// monster
	// treasure?
	// exit?

	Tile();
	Tile(int _x, int _y, bool _passable);
	bool operator==(const Tile& rhs) const;
};