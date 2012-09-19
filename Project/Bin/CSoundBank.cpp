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
int CSoundBank::OnLoad(char* File) {
    Mix_Chunk* TempSound = NULL;

    if((TempSound = Mix_LoadWAV(File)) == NULL) {
        return -1;
    }

    SoundList.push_back(TempSound);

	if( SoundList.size() - 1 ){
		std::string filename = File;
		debug("Loaded sound " + filename, 2);
	}
    return (SoundList.size() - 1);
}

//------------------------------------------------------------------------------
void CSoundBank::OnCleanup() {
    for(unsigned int i = 0;i < SoundList.size();i++) {
        Mix_FreeChunk(SoundList[i]);
    }

    SoundList.clear();
}

//==============================================================================
void CSoundBank::Play(int ID) {
    if(ID < 0 || ID >= SoundList.size()) return;
    if(SoundList[ID] == NULL) return;

    Mix_PlayChannel(-1, SoundList[ID], 0);
}

//==============================================================================
