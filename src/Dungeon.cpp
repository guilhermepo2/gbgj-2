#include "Dungeon.h"
#include "Tile.h"

static Tile theDungeon[DUNGEON_DIMENSION][DUNGEON_DIMENSION];
static gueepo::math::vec2 CurrentPlayerStartPosition;

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

void Dungeon::InitializeFromJson(gueepo::json jsonFile) {
    gueepo::json levelData;
    jsonFile.GetArray("levelData", levelData);

    if(levelData.IsArray()) {
        if(levelData.GetArraySize() != DUNGEON_DIMENSION * DUNGEON_DIMENSION) {
            LOG_ERROR("json file has incorrect level data!!");
            return;
        }

        for(int i = 0; i < levelData.GetArraySize(); i++) {
            int temp;
            levelData.GetIntAt(i, temp);

            int x = i % DUNGEON_DIMENSION;

            // have to do this *quirky* thing because on the dungeon and rendering (0,0) is bottom left
            // but reading from the json (0, 0) is top left
            // this should fix things
            int y = (DUNGEON_DIMENSION - 1) - (i / DUNGEON_DIMENSION);

            theDungeon[x][y].x = x;
            theDungeon[x][y].y = y;
            switch(temp) {
                case 0: {
                    theDungeon[x][y].isPassable = true;
                } break;
                case 1: {
                    theDungeon[x][y].isPassable = false;
                } break;
                case 2: {
                    theDungeon[x][y].isPassable = true;
                    theDungeon[x][y].isPlayerPortal = true;
                    CurrentPlayerStartPosition.x = x;
                    CurrentPlayerStartPosition.y = y;
                }
            }

        }
    }
}

gueepo::math::vec2 Dungeon::GetCurrentPlayerStartPosition() {
    return CurrentPlayerStartPosition;
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
