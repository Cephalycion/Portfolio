/******************************************************************************/
/*!
\file   E_AudioManager.h
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

#pragma once
#include "pch.h"

namespace Engine
{
    typedef struct Audio
    {
        AEAudio audio;
        FILEPATH path;
        Audio(AEAudio a, FILEPATH p) : audio { a }, path { p }{};
    } Audio;

    class AudioManager
    {
    private:
        // Disallow copy and move
        AudioManager(const AudioManager&) = delete;
        AudioManager& operator=(const AudioManager&) = delete;
        AudioManager(AudioManager&&) = delete;
        AudioManager& operator=(AudioManager&&) = delete;

    protected:
        std::vector<Audio> audioList;

        f32 volumeBGM;
        f32 volumeSFX;

        AEAudioGroup groupBGM;
        AEAudioGroup groupSFX;

    public:
        AudioManager() : 
            audioList{}, 
            volumeBGM{ 0.5f }, 
            volumeSFX{ 0.5f }, 
            groupBGM{}, 
            groupSFX{} {};
        ~AudioManager();

        void addAudio(Audio audio);

        // path: FILEPATH enumerator
        // loop: Loop this audio after it finishes?
        static void playAudio(FILEPATH path, bool loop);

        static void stopAudio();    // Stops all audio that is currently playing
        void Init();
        void UnloadAudio();

        static void setVolumeBGM(f32 v);
        static void setVolumeSFX(f32 v);
        static f32 getVolumeBGM();
        static f32 getVolumeSFX();
    };
}