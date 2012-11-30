//==============================================================================
// SoundBank Class
//==============================================================================
#ifndef _CSOUNDBANK_H_
	#define _CSOUNDBANK_H_

#include <SDL.h>
#include <SDL_mixer.h>
#include <map>
#include <string>

//==============================================================================
class CSoundBank {
	public:
		static CSoundBank					SoundControl;
		std::map<std::string, Mix_Chunk*>	SoundList;

		std::map<std::string, Mix_Music*>	MusicList;

	public:
		CSoundBank();

		enum SoundType {
			EFFECT = 0,
			MUSIC
		};

		bool OnLoad(SoundType type, std::string ID, char* File);

		int AllocateChannels(int NumberOfChannels);

		void OnCleanup();

	public:
		int Play(SoundType type, std::string ID, bool loop = false, int volume = 128);

		void StopChannel(int channel);

		void StopMusic();

		void Volume(int channel, int volume);

};

//==============================================================================

#endif
