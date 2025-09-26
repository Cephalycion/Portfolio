/******************************************************************************/
/*!
\file   Main.cpp
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Kendrick Sim Hean Guan (100%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\brief
  This source file contains the entry point of the program.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

// ---------------------------------------------------------------------------
// includes

#include "pch.h"
#include "E_Engine.h"
#include "E_Constants.h"
#include "E_IOManager.h"
#include "E_AudioManager.h"
#include <crtdbg.h> // To check for memory leaks
#include <AEEngine.h>

using namespace Engine;


// ---------------------------------------------------------------------------
// main

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// Run engine
	auto engine = Singleton<ProgramEngine>::Instance();
	engine->Initialize(hInstance, nCmdShow);

	engine->Run();

	engine->Shutdown();
	engine.reset();
	Singleton<ProgramEngine>::Destroy();
}
