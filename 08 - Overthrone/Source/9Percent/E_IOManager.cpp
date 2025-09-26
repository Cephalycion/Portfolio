/******************************************************************************/
/*!
\file   E_IOManager.cpp
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

#include "pch.h"
#include "E_IOManager.h"
#include <string>

namespace Engine
{
	IOManager::IOManager()
	{
	}

	IOManager::~IOManager()
	{
	}

	void IOManager::ReadAudioList()
	{
		sPtr<Engine::AudioManager> inst = Singleton<Engine::AudioManager>::Instance();

		// LOAD MUSIC
		for (s32 f = sCast<s32>(BGM_START + 1); f < sCast<s32>(BGM_END); ++f)
		{
			inst->addAudio(Audio(AEAudioLoadMusic(C_filePaths[f]), sCast<FILEPATH>(f)));
		}
		// LOAD SOUND
		for (s32 f = sCast<s32>(SFX_START + 1); f < sCast<s32>(SFX_END); ++f)
		{
			inst->addAudio(Audio(AEAudioLoadSound(C_filePaths[f]), sCast<FILEPATH>(f)));
		}
	}

	// Helper function to skip the Byte Order Mark (BOM) in the .csv file
	void IOManager::skipBOM(std::ifstream& input)
	{
		char peeker[3]{};
		input.read(peeker, 3);

		if ((unsigned char)peeker[0] == 0xEF &&
			(unsigned char)peeker[1] == 0xBB &&
			(unsigned char)peeker[2] == 0xBF)
		{
			return;
		}
		input.seekg(0);
	}

	size_t IOManager::LocateDelimiterEnd(const std::string& s, size_t delimiterStart)
	{
		// Need to find single '"' but ignore doubled '""'
		size_t delimiterEnd{ delimiterStart };
		size_t secondDelimiterPos{ delimiterStart };
		while (delimiterEnd != std::string::npos)
		{
			// Get the next delimiter position
			delimiterEnd = s.find('"', secondDelimiterPos + 1);
			// Check if the next delimiter character is directly after it
			secondDelimiterPos = s.find('"', delimiterEnd + 1);
			if (delimiterEnd + 1 != secondDelimiterPos)
				// We've found the end of the delimited string
				return delimiterEnd;
		}
		// There was no end of the delimited string. This shouldn't happen, but also shouldn't crash the program.
		Error::RaiseErr("IOManager::LocateDelimiterEnd", "Couldn't find an ending \"", ERROR_WARN);
		return std::string::npos;
	}
	size_t IOManager::ResolveDuplicatedQuotes(std::string* s, size_t delimiterStart, size_t delimiterEnd)
	{
		size_t numRemoved{};

		delimiterStart = s->find('"', delimiterStart + 1);
		while (delimiterStart < delimiterEnd)
		{
			s->erase(delimiterStart, 1);
			--delimiterEnd;
			++numRemoved;
			delimiterStart = s->find('"', delimiterStart + 1);
		}
		return numRemoved;
	}

	//sneeki lil function
	int IOManager::strtoi(const std::string& input)
	{
		int res = 0;
		for (int i = 0; i < input.length(); ++i)
		{
			if (input[i] >= '0' && input[i] <= '9')
			{
				res *= 10;
				res += input[i] - '0';
			}
		}
		return res;
	}

	// Read level all-in-one solution
	// Does not initialise individual block objects however
	void IOManager::ReadLevel(FILEPATH fileEnumerator)
	{
		sPtr<Game::Map> mapInst = Singleton<Game::Map>::Instance();

		std::ifstream fileToRead(C_filePaths[fileEnumerator]);
		if (fileToRead.is_open())
		{
			size_t width{}, height{};
			std::string s{};

			// Calculate width and height
			while (std::getline(fileToRead, s))
			{
				// Each line is 1 height.
				// Total width is number of commas + 1.
				size_t columns{ sCast<size_t>(std::count(s.begin(),s.end(),',')) + 1 };
				if (width < columns)
					width = columns;
				++height;
			}
			if (mapInst->arr != nullptr)
			{
				mapInst->DeleteMap();
			}

			mapInst->width = width;
			mapInst->height = height;

			// Dynamic allocation with triple pointers
			mapInst->arr = new Game::Block**[height];
			for (int i = 0; i < height; ++i)
			{
				mapInst->arr[i] = new Game::Block*[width];

				for (int j = 0; j < width; ++j)
					mapInst->arr[i][j] = new Game::Block();
			}

			// Return to the beginning of filestream
			fileToRead.clear();
			fileToRead.seekg(0);
			skipBOM(fileToRead);

			// Read each cell and assign types to the blocks
			int x{0}, y{ 0 };
			while (std::getline(fileToRead, s))
			{
				std::string cell;
				size_t prevoffset{};
				size_t offset{};
				size_t strDelimitOffset{ s.find('"') }; // The position of string delimiter '"' in our level data file
				while (offset != std::string::npos)
				{
					offset = s.find(',', prevoffset);
					// Check whether this comma might be within a delimited string.
					if (strDelimitOffset != std::string::npos && strDelimitOffset < offset)
					{
						// Find delimited string end
						size_t strDelimitEndOffset = LocateDelimiterEnd(s, strDelimitOffset);
						// Cleanup the string
						size_t dupeQuotesRemoved = ResolveDuplicatedQuotes(&s, strDelimitOffset, strDelimitEndOffset);
						strDelimitEndOffset -= dupeQuotesRemoved;

						// Find the comma after this delimited string
						offset = s.find(',', strDelimitEndOffset + 1);
						// Remove the delimiters
						s.erase(strDelimitEndOffset, 1);
						s.erase(strDelimitOffset, 1);
						offset -= 2; // Compensate for removed '"'
						strDelimitOffset = s.find('"', strDelimitEndOffset);
					}
					// Extract this cell's value
					cell = s.substr(prevoffset, offset - prevoffset);
					//std::cout << cell << " ";
					prevoffset = offset + 1;

					size_t idlen = cell.length();
					if (cell.find('|') != std::string::npos)
					{
						idlen = cell.find('|');	
						mapInst->arr[y][x]->data = cell.substr(idlen+1);
					}
					int id = strtoi(cell.substr(0, idlen));
					mapInst->arr[y][x]->type = sCast<Game::BLOCK_TYPE>(id);
					
					++x;
				}
				++y;
				x = 0;
			}
			std::cout << std::endl;
			fileToRead.close();
			mapInst->loaded = true;
		}
		else
		{
			Error::RaiseErrFormatted("IOManager::ReadLevel", "Failed to load %s", ERROR_ERR, C_filePaths[fileEnumerator]);
		}
	}

	// Read settings, should only call once in initialisation of audioManager
	void IOManager::ReadSettings()
	{
		sPtr<Engine::AudioManager> inst = Singleton<Engine::AudioManager>::Instance();

		std::ifstream fileToRead(C_filePaths[SETTINGS]);
		if (fileToRead.is_open())
		{
			std::string s{};
			std::getline(fileToRead, s);

			// Check for empty string
			if (s.empty()) { s = "0.0"; }
			std::cout << s << '\n';

			inst->setVolumeBGM(sCast<f32>(std::stof(s)));
			std::getline(fileToRead, s);
			std::cout << s << '\n';

			inst->setVolumeSFX(sCast<f32>(std::stof(s)));
			fileToRead.close();
		}
		else
		{
			Error::RaiseErrFormatted("IOManager::ReadLevel", "Failed to load %s", ERROR_ERR, C_filePaths[SETTINGS]);
		}
	}

	// Write settings to both audioManager and text file
	// Call every time settings are changed
	void IOManager::WriteSettings(f32 volumeBGM, f32 volumeSFX)
	{
		sPtr<Engine::AudioManager> inst = Singleton<Engine::AudioManager>::Instance();
		inst->setVolumeBGM(volumeBGM);
		inst->setVolumeSFX(volumeSFX);

		std::ofstream fileToWrite;
		fileToWrite.open(C_filePaths[SETTINGS]);
		if (fileToWrite.is_open())
		{
			fileToWrite << std::to_string(sCast<float>(inst->getVolumeBGM()));
			fileToWrite << '\n';
			fileToWrite << std::to_string(sCast<float>(inst->getVolumeSFX()));
			fileToWrite.close();
		}
		else
		{
			Error::RaiseErrFormatted("IOManager::ReadLevel", "Failed to load %s", ERROR_ERR, C_filePaths[SETTINGS]);
		}
	}

	std::string IOManager::GetFilePath(FILEPATH fileEnumerator)
	{
		std::string ret{};
		if (fileEnumerator > 0 && fileEnumerator < FILEPATH_SIZE)
		{ ret = C_filePaths[fileEnumerator]; }
		return ret;
	}

}