#pragma once

// let's try 9x9 dungeons
static const int DUNGEON_DIMENSION = 9;

class Tile;

class Dungeon {
public:
	static void Initialize();
	// todo: initialize from json

	static bool IsInBounds(int x, int y);
	static bool IsPassable(int x, int y);
	static Tile* GetTile(int x, int y);
};