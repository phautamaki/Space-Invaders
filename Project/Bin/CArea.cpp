//=============================================================================
#include "CArea.h"
#include <string>
#include "functions.h"

//=============================================================================
CArea CArea::AreaControl;

//=============================================================================
CArea::CArea() {
}

//=============================================================================
bool CArea::OnLoad(char* File) {
    OnCleanup();

    FILE* FileHandle = fopen(File, "r");

    if(FileHandle == NULL) {
        return false;
    }

    char TilesetFile[255];

	// Load tileset picture to a surface
    fscanf(FileHandle, "%s\n", TilesetFile);
    if((Surf_Tileset = CSurface::OnLoad(TilesetFile)) == false) {
        fclose(FileHandle);

        return false;
    }

	// Load maps to CAreas MapList
	int numberOfMaps;
	fscanf(FileHandle, "%d\n", &numberOfMaps);

	for(int X = 0;X < numberOfMaps;X++) {
		char MapFile[255];

		fscanf(FileHandle, "%s ", MapFile);

		// Load map's tileset into map's Tilelist
		CMap tempMap;
		if(tempMap.OnLoad(MapFile) == false) {
			fclose(FileHandle);

			return false;
		}

		tempMap.Surf_Tileset = Surf_Tileset;
		MapList.push_back(tempMap);
		fscanf(FileHandle, "\n");
	}

    fclose(FileHandle);

	std::string filename = File;
	debug("Loaded area " + filename, 2);
    return true;
}

//-----------------------------------------------------------------------------
void CArea::OnRender(SDL_Surface* Surf_Display, int CameraX, int CameraY) {
	int MapWidth  = MAP_WIDTH * TILE_SIZE;

	int FirstID = -CameraX / MapWidth;

	for(int i = 0;i < 2;i++) {
		int ID = FirstID + i;

		if(ID < 0 || ID >= MapList.size()) continue;

		int X = ID* MapWidth + CameraX;
		MapList[ID].OnRender(Surf_Display, X, 0); // Y-coordinate always same
	}
}

//-----------------------------------------------------------------------------
void CArea::OnCleanup() {
	if(Surf_Tileset) {
		SDL_FreeSurface(Surf_Tileset);
	}

	MapList.clear();
}

//=============================================================================
CMap* CArea::GetMap(int X, int Y) {
	int MapWidth  = MAP_WIDTH * TILE_SIZE;

    int ID = X / MapWidth;

    if(ID < 0 || ID >= MapList.size()) {
        return NULL;
    }

    return &MapList[ID];
}

//-----------------------------------------------------------------------------
CTile* CArea::GetTile(int X, int Y) {
	int MapWidth  = MAP_WIDTH * TILE_SIZE;
	int MapHeight = MAP_HEIGHT * TILE_SIZE;

	CMap* Map = GetMap(X, Y);

	if(Map == NULL) return NULL;

	X = X % MapWidth;
	Y = Y % MapHeight;

    return Map->GetTile(X, Y);
}

//=============================================================================
