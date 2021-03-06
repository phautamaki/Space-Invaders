//=============================================================================
#include "CMap.h"
#include <string>
#include "functions.h"
#include <fstream>
#include <iostream>
#include <sstream>

//=============================================================================
CMap::CMap(int MapNumber): mapNumber(MapNumber) {
	Surf_Tileset = NULL;
}

//=============================================================================
bool CMap::OnLoad(char* File) {
    TileList.clear();
	
    FILE* FileHandle;
	errno_t err = fopen_s(&FileHandle, File, "r");

    if(FileHandle == NULL || err != 0) {
        return false;
    }

    for(int Y = 0;Y < MAP_HEIGHT;Y++) {
        for(int X = 0;X < MAP_WIDTH;X++) {
            CTile tempTile;
			tempTile.X = X * TILE_SIZE + mapNumber*MAP_WIDTH*TILE_SIZE;
			tempTile.Y = Y * TILE_SIZE;

            fscanf_s(FileHandle, "%d:%d ", &tempTile.TileID, &tempTile.TypeID);
			tempTile.oldTypeID = tempTile.TypeID;

			if (tempTile.TypeID == TILE_TYPE_BLOCK_BREAKABLE) {
				tempTile.SetHP( 200 );
			}

            TileList.push_back(tempTile);
        }
        fscanf_s(FileHandle, "\n");
    }

    fclose(FileHandle);

	std::string filename = File;
	
	debug("Loaded map " + filename, 2);
    return true;
}

//-----------------------------------------------------------------------------
void CMap::OnRender(SDL_Surface* Surf_Display, int MapX, int MapY) {
	if(Surf_Tileset == NULL) return;

	int TilesetWidth  = Surf_Tileset->w / TILE_SIZE;
	int TilesetHeight = Surf_Tileset->h / TILE_SIZE;

	int ID = 0;

	for(int Y = 0;Y < MAP_HEIGHT;Y++) {
		for(int X = 0;X < MAP_WIDTH;X++) {
			if(TileList[ID].TypeID == TILE_TYPE_NONE) {
				ID++;
				continue;
			}

			int tX = MapX + (X * TILE_SIZE);
			int tY = MapY + (Y * TILE_SIZE);

			int TilesetX = (TileList[ID].TileID % TilesetWidth) * TILE_SIZE;
			int TilesetY = (TileList[ID].TileID / TilesetWidth) * TILE_SIZE;

			CSurface::OnDraw(Surf_Display, Surf_Tileset, tX, tY, TilesetX, TilesetY, TILE_SIZE, TILE_SIZE);

			ID++;
		}
	}
}

//=============================================================================
CTile* CMap::GetTile(int X, int Y) {
    int ID = 0;

    ID = X / TILE_SIZE;
    ID = ID + (MAP_WIDTH * (Y / TILE_SIZE));

    if(ID < 0 || (unsigned int)ID >= TileList.size()) {
    	return NULL;
    }

    return &TileList[ID];
}

//=============================================================================
