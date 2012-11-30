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
#include "CInput.h"

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

	//Holds the information about what is our current level we are playing
	int CurrentLevelNumber;

	bool			BossDead;
	bool			BossFightOn;
	CPlayer*		Player;

private:
	
	int levelBeginningScores;
	bool keepOldGun;

	// GUI
	SDL_Surface*	UIIcons;
	SDL_Surface*	UItiles;

	// Scrolling space background
	SDL_Surface*	SpaceBG;
	float			BG_offset;

	// Input field
	CInput Input;

	bool GameOver;

	
	//Defines the X-coordinate in which the level should change. Information should be stored on the last line of the level's info-file. 
	//This may need modification, in case we have e.g. some kind of boss at the end and the level should change after it is killed.
	int LevelEndingPoint;
	//Holds the information about different entities (enemies and items) that should appear.
	std::vector<LevelInfo> Level;
	//Next index to inspect from Level-vector
	unsigned int LevelInfoIndex;
	bool ResetCurrentLevel;

	CAppStateGame();

public:
	void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
	void OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);
	
	void OnActivate();
	void OnLevelChange();
	void OnDeactivate();
	void OnLoop();
	void OnRender(SDL_Surface* Surf_Display);
		void RenderUIBase(SDL_Surface* Surf_Display) const;
		void RenderUIIcons(SDL_Surface* Surf_Display) const;

	std::vector<LevelInfo> GetCurrentLevelInfo(const std::string& filename);
	static CAppStateGame* GetInstance();
	void ResetLevelNow();

private:
	void ResetLevel();
	void CheckHighScores();
};

//=============================================================================

#endif
