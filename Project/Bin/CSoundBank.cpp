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

		if( SoundList.size() ){
			std::string filename = File;
			debug("Loaded sound " + filename, 2);
		}
		AllocateChannels(SoundList.size());
		return true;
		break;

	case MUSIC:
		
		if((TempMusic = Mix_LoadMUS(File)) == NULL) {
			return false;
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

	return false;
}


int CSoundBank::AllocateChannels(int NumberOfChannels) {

	//This unreserves everything in the beginning
	Mix_AllocateChannels(0);

	int reserved_count;
	reserved_count=Mix_AllocateChannels(NumberOfChannels);
	if(reserved_count!=NumberOfChannels) {

		debug("All " + IntToString(NumberOfChannels) + " channels were not allocated!");
	
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
int CSoundBank::Play(SoundType type, std::string ID, bool loop) {

	int channel = -1;
	
	switch(type) {
	
	case EFFECT:
 		if(SoundList[ID] == NULL) return channel;
 
		loop ? channel = Mix_PlayChannel(-1, SoundList[ID], -1) : Mix_PlayChannel(-1, SoundList[ID], 0);

		break;
	
	case MUSIC:
		if(MusicList[ID] == NULL) return channel;

		//Stops the current music playback
		Mix_HaltMusic();
			
		//Plays the new music
		Mix_PlayMusic(MusicList[ID], -1);
		
		break;
	}
	
	debug(IntToString(channel));

	return channel;
}

void CSoundBank::StopChannel(int channel) {

	Mix_HaltChannel(channel);

}

void CSoundBank::StopMusic() {

	Mix_HaltMusic();

}

void CSoundBank::Volume(int channel, int volume) {

	Mix_Volume(channel, volume);

}

//==============================================================================
