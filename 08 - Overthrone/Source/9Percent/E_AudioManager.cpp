/******************************************************************************/
/*!
\file   E_AudioManager.cpp
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Ryan Chan Kuan Fu (100%)
\par    email: c.kuanfuryan\@digipen.edu
\par    DigiPen login: c.kuanfuryan

\brief
  AudioManager class and Audio struct which encapsulates an AEAudio object and 
  gives it some additional functionality. AudioManager allows volume to be set 
  seperately for BGM and SFX.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#include "pch.h"
#include "E_AudioManager.h"
#include "E_IOManager.h"

namespace Engine
{
	// This destructor currently crashes, not sure how to fix
	AudioManager::~AudioManager()
	{
	}

	void AudioManager::UnloadAudio()
	{
		stopAudio();
		for (Audio const& a : audioList)
		{
			if (AEAudioIsValidAudio(a.audio))
			{
				AEAudioUnloadAudio(a.audio);
			}
		}
		AEAudioUnloadAudioGroup(groupBGM);
		AEAudioUnloadAudioGroup(groupSFX);
	}

	void AudioManager::setVolumeBGM(f32 v)
	{
		Singleton<AudioManager>::Instance()->volumeBGM = v;
		AEAudioSetGroupVolume(Singleton<AudioManager>::Instance()->groupBGM, v);
	}

	void AudioManager::setVolumeSFX(f32 v)
	{
		Singleton<AudioManager>::Instance()->volumeSFX = v;
		AEAudioSetGroupVolume(Singleton<AudioManager>::Instance()->groupSFX, v);
	}

	f32 AudioManager::getVolumeBGM()
	{
		return Singleton<AudioManager>::Instance()->volumeBGM;
	}

	f32 AudioManager::getVolumeSFX()
	{
		return Singleton<AudioManager>::Instance()->volumeSFX;
	}

	void AudioManager::addAudio(Audio audio)
	{
		if (AEAudioIsValidAudio(audio.audio))
		{
			audioList.push_back(audio);
		}
	}

	void AudioManager::playAudio(FILEPATH path, bool loop)
	{
		s32 loops = loop ? -1 : 0;
		sPtr<AudioManager> manager = Singleton<AudioManager>::Instance();
		for (Audio const& a : manager->audioList)
		{
			if (a.path == path)
			{
				if (path > BGM_START && path < BGM_END)
				{
					AEAudioPlay(a.audio, manager->groupBGM, 1.0f, 1.0f, loops);
				}
				else if (path > SFX_START && path < SFX_END)
				{
					AEAudioPlay(a.audio, manager->groupSFX, 1.0f, 1.0f, loops);
				}
			}
		}
	}

	void AudioManager::stopAudio()
	{
		sPtr<AudioManager> manager = Singleton<AudioManager>::Instance();
		AEAudioStopGroup(manager->groupBGM);
		AEAudioStopGroup(manager->groupSFX);
	}

	void AudioManager::Init()
	{
		groupBGM = AEAudioCreateGroup();
		groupSFX = AEAudioCreateGroup();

		// Read the audio files (AudioManager instance must exist at this point!)
		Singleton<IOManager>::Instance()->ReadAudioList();
		Singleton<IOManager>::Instance()->ReadSettings();
	}
}