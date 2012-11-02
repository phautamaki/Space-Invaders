//==============================================================================
#include "CSoundBank.h"
#include <string>
#include "functions.h"

//==============================================================================
CSoundBank CSoundBank::SoundControl;

//==============================================================================
CSoundBank::CSoundBank() {
}

//------------------------------------------------------------------------------
bool CSoundBank::OnLoad(SoundType type, std::string ID, char* File) {
    
	Mix_Chunk* TempSound = NULL;
	Mix_Music* TempMusic = NULL;

	switch(type) {

	case EFFECT:
		
		if((TempSound = Mix_LoadWAV(File)) == NULL) {
			return false;
		}

		SoundList.insert(make_pair(ID, TempSound));

		if( SoundList.size() - 1 ){
			std::string filename = File;
			debug("Loaded sound " + filename, 2);
		}
		AllocateChannels(SoundList.size()-1);
		return true;
		break;

	case MUSIC:
		

		if((TempMusic = Mix_LoadMUS(File)) == NULL) {
			return -1;
		}

		MusicList.insert(make_pair(ID, TempMusic));

		if( MusicList.size() - 1) {
			std::string filename = File;
			debug("Loaded music " + filename, 2);
			return false;
		}
		return true;
		break;
	}
}

int CSoundBank::AllocateChannels(int NumberOfChannels) {

	//This unreserves everything in the beginning
	Mix_AllocateChannels(0);

	int reserved_count;
	reserved_count=Mix_AllocateChannels(NumberOfChannels);
	if(reserved_count!=NumberOfChannels) {

		debug(IntToString(NumberOfChannels) + " channels were not allocated!");
	
	}

	return reserved_count;
}

//------------------------------------------------------------------------------
void CSoundBank::OnCleanup() {

	for( std::map< std::string, Mix_Chunk*>::iterator i = SoundList.begin();
		i != SoundList.end(); ++i ) {
			Mix_FreeChunk(i->second);
	}

	for( std::map< std::string, Mix_Music*>::iterator i = MusicList.begin();
		i != MusicList.end(); ++i ) {
			Mix_FreeMusic(i->second);
	}

	Mix_AllocateChannels(0);
	Mix_HaltMusic();
    SoundList.clear();
	MusicList.clear();

}

//==============================================================================
void CSoundBank::Play(SoundType type, std::string ID) {
	
	switch(type) {
	
	case EFFECT:
		if(SoundList[ID] == NULL) return;

		if(ID == "ShootingSoundBasic") {
			Mix_PlayChannel(0, SoundList[ID], 0);
		}
		else if(ID == "ShootingSoundBig") {
			Mix_PlayChannel(1, SoundList[ID], 0);
		}
		else{
			Mix_PlayChannel(-1, SoundList[ID], 0);
		}
		
		break;
	
	case MUSIC:
		if(MusicList[ID] == NULL) return;

		//Stops the current music playback
		Mix_HaltMusic();
			
		//Plays the new music
		Mix_PlayMusic(MusicList[ID], -1);
		
		break;
	}
}

void CSoundBank::StopMusic() {

	Mix_HaltMusic();

}

//==============================================================================
