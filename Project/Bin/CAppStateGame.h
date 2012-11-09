//=============================================================================
#ifndef __CAPPSTATEGAME_H__
	#define __CAPPSTATEGAME_H__

#include <vector>

#include "CAppState.h"

#include "CArea.h"
#include "CCamera.h"
#include "CEntity.h"
#include "CEnemy.h"
#include "CSurface.h"
#include "CFileReader.h"

#include "CPlayer.h"

//=============================================================================
class CAppStateGame : public CAppState {
	public:
		struct LevelInfo {
			unsigned int ActiveXPosition; //When comes visible
			unsigned int Type;			  //Item or enemy
			unsigned int SubType;		  //What kind
			unsigned int YPosition;		  //Y-position from which appears (X is calculated based on Cameras X-position).

		};
	
		static CAppStateGame Instance;
		

	private:
		
		bool ResetCurrentLevel;
        CPlayer*		Player;

		// GUI
		SDL_Surface*	IconLife;

		// Scrolling space background
		SDL_Surface*	SpaceBG;
		float			BG_offset;

		//Holds the information about what is our current level we are playing
		int CurrentLevelNumber;

		//Defines the X-coordinate in which the level should change. Information should be stored on the last line of the level's info-file. 
		//This may need modification, in case we have e.g. some kind of boss at the end and the level should change after it is killed.
		int LevelEndingPoint;

		//Holds the information about different entities (enemies and items) that should appear.
		std::vector<LevelInfo> Level;

		//Next index to inspect from Level-vector
		unsigned int LevelInfoIndex;

	private:
		CAppStateGame();

	public:
       	void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);

       	void OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);

		std::vector<LevelInfo> GetCurrentLevelInfo(const std::string& filename);

	public:
		void OnActivate();

		void OnLevelChange();

		void OnDeactivate();

		void OnLoop();

		void OnRender(SDL_Surface* Surf_Display);

	public:
		static CAppStateGame* GetInstance();

		void ResetLevelNow();

private:
		void ResetLevel();
};

//=============================================================================

#endif
