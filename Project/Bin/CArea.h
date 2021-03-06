//=============================================================================
#ifndef _CAREA_H_
    #define _CAREA_H_

#include "CMap.h"

//=============================================================================
class CArea {
    public:
        static CArea            AreaControl;

    public:
        std::vector<CMap>       MapList;

    private:
        SDL_Surface*			Surf_Tileset;

    public:
        CArea();

        bool    OnLoad(char* File);

        void    OnRender(SDL_Surface* Surf_Display, int CameraX, int CameraY);

        void    OnCleanup();

    public:
        CMap*	GetMap(int X, int Y);
		CTile*	GetTile(int X, int Y);

		CTile*	GetNextHorizontalTile(int X, int Y);

		void	RestoreBrokenTiles();
};

//=============================================================================

#endif

