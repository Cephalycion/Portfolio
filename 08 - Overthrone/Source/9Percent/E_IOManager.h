/******************************************************************************/
/*!
\file   E_IOManager.h
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Ryan Chan Kuan Fu (46%)
\par    email: c.kuanfuryan\@digipen.edu
\par    DigiPen login: c.kuanfuryan

\author Matthew Chan Shao Jie (45%)
\par    email: m.chan\@digipen.edu
\par    DigiPen login: m.chan

\author Kendrick Sim Hean Guan (9%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\brief
  Handles most input/output for the game. Is a singleton. Contains functions for
  reading audio .mp3, level .csv, settings .txt. Along with write to settings.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "pch.h"
#include <fstream>
#include "G_Map.h"
#include "E_AudioManager.h"

namespace Engine
{
    class IOManager
    {
    protected:
        // Disallow copy and move
        IOManager(const IOManager&) = delete;
        IOManager& operator=(const IOManager&) = delete;
        IOManager(IOManager&&) = delete;
        IOManager& operator=(IOManager&&) = delete;

        void skipBOM(std::ifstream &input);
        // Find the end of the delimited entry in a .csv
        size_t LocateDelimiterEnd(const std::string& s, size_t delimiterStart);
        // Remove duplicated quotations within a delimited entry
        size_t ResolveDuplicatedQuotes(std::string* s, size_t delimiterStart, size_t delimiterEnd);

    public:
        static int strtoi(const std::string& input);
        IOManager();
        ~IOManager();

        void ReadAudioList();
        void ReadLevel(FILEPATH fileEnumerator);
        void ReadSettings();
        void WriteSettings(f32 volumeBGM, f32 volumeSFX);

        static std::string GetFilePath(FILEPATH fileEnumerator);
    };
}