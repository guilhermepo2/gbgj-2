#include "Dungeon.h"
#include "Tile.h"

static Tile theDungeon[DUNGEON_DIMENSION][DUNGEON_DIMENSION];

void Dungeon::Initialize() {
	for (int y = 0; y < DUNGEON_DIMENSION; y++) {
		for (int x = 0; x < DUNGEON_DIMENSION; x++) {
			theDungeon[x][y].x = x;
			theDungeon[x][y].y = y;

			if (y == 0 || x == 0 || x == DUNGEON_DIMENSION - 1 || y == DUNGEON_DIMENSION - 1) {
				theDungeon[x][y].isPassable = false;
			}
			else {
				theDungeon[x][y].isPassable = true;
			}
		}
	}
}

bool Dungeon::IsInBounds(int x, int y) {
	return x >= 0 && y >= 0 && x < DUNGEON_DIMENSION&& y < DUNGEON_DIMENSION;
}

bool Dungeon::IsPassable(int x, int y) {
	if (IsInBounds(x, y)) {
		return theDungeon[x][y].isPassable;
	}

	return false;
}

Tile* Dungeon::GetTile(int x, int y) {
	Tile* t = nullptr;
	
	if (IsInBounds(x, y)) {
		t = &theDungeon[x][y];
	}

	return t;
}
