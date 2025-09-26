/******************************************************************************/
/*!
\file   E_Engine.cpp
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Kendrick Sim Hean Guan (86%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\author Marc Alviz Evangelista (9%)
\par    email: marcalviz.e\@digipen.edu
\par    DigiPen login: marcalviz.e

\author Ryan Chan Kuan Fu (5%)
\par    email: c.kuanfuryan\@digipen.edu
\par    DigiPen login: c.kuanfuryan

\brief
  This is the implementation file for the backend engine manager, interfacing with
  AEEngine and WINAPI for the running of the program window and handles the
  the running of the scene management system.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#include "pch.h"
#include "E_Engine.h"
#include "E_Constants.h"
#include "E_Camera.h"
#include "E_Physics.h"
#include "E_IOManager.h"
#include "E_AudioManager.h"
#include <CommCtrl.h>

#pragma comment(lib,"comctl32.lib")

/* Included to destroy their singletons on program exit */
#include "E_Asset.h"
#include "E_Text.h"

namespace Engine {

	LRESULT ProgramEngine::WndProcCallback(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
	{
		UNREFERENCED_PARAMETER(uIdSubclass);
		UNREFERENCED_PARAMETER(dwRefData);

		switch (uMsg)
		{
		// When the user focuses or exits moving the window.
		case WM_SETFOCUS:
		case WM_EXITSIZEMOVE:
			SetCursorConstraint();
			break;
		}

		return DefSubclassProc(hWnd, uMsg, wParam, lParam);
	}

	void ProgramEngine::SetCursorConstraint()
	{
		RECT constraint;
		GetWindowRect(AESysGetWindowHandle(), &constraint);
		ClipCursor(&constraint);
	}

	ProgramEngine::ProgramEngine()
		: isRunning(false)
		, sceneManager()
	{
	}
	ProgramEngine::~ProgramEngine()
	{
	}

	void ProgramEngine::Initialize(::HINSTANCE hInstance, s32 nCmdShow)
	{
		// Program constants
#ifdef _DEBUG
		C_debugDoRender = C_errDoRender = true;
		C_enableConsole = true;
#else
		C_debugDoRender = C_errDoRender = false;
		C_enableConsole = false;
#endif

		// Using custom window procedure
		AESysInit(hInstance, nCmdShow, C_windowWidth, C_windowHeight, C_enableConsole, C_fpsTarget, C_enableVSync, NULL);
		// Changing the window title
		AESysSetWindowTitle(C_programName);
		// reset the system modules
		AESysReset();
		// Associate the WNDPROC callback with Engine so that we're aware of events that happen to our program window.
		HWND hWnd = AESysGetWindowHandle();
		if (!SetWindowSubclass(hWnd, ProgramEngine::WndProcCallback, 1, 0))
			Error::RaiseErr("ProgramEngine::Initialize", "Failed to set WndProcCallback!");

		// Constrain cursor to the window
		SetCursorConstraint();

		// Initialize first scene
		sceneManager.QueueSwitchToScene(Scene::SCENE_SPLASH);

		// Initialise IOManager and AudioManager Singleton
		Singleton<IOManager>::Instance();
		Singleton<AudioManager>::Instance()->Init();
	}

	void ProgramEngine::Run()
	{
		isRunning = true;

		while (isRunning)
		{
			// Informing the system about the loop's start
			AESysFrameStart();
			++C_fpsCount;
			C_frametime = sCast<f32>(AEFrameRateControllerGetFrameTime());

			// Game Tick
			Singleton<Error>::Instance()->Update(C_frametime);
			sceneManager.FlushChanges();
			Update();

			Render();
			Singleton<Debug>::Instance()->RenderAll();
			Singleton<Error>::Instance()->RenderAll();

			// Remove value trackers in case debugDoRender is false, to fix memory leak
			Singleton<Debug>::Instance()->RemoveAllValueTrackers();

			// Informing the system about the loop's end
			AESysFrameEnd();
		}
	}
	void ProgramEngine::Shutdown()
	{
		// Unload audio first
		Singleton<AudioManager>::Instance()->UnloadAudio();

		// Shutdown the scenes
		sceneManager.Cleanup();

		// Free singletons
		Singleton<C_TYPE_PHYSICSMANAGER>::Destroy();
		Singleton<Graphics::CameraManager>::Destroy();
		Singleton<Graphics::Camera>::Destroy();
		Singleton<Graphics::AssetManager>::Destroy();
		Singleton<Engine::IOManager>::Destroy();
		Singleton<Engine::AudioManager>::Destroy();
		Singleton<TextBuilder>::Destroy();
		Singleton<Error>::Destroy();
		Singleton<Debug>::Destroy();

		// free the system
		AESysExit();
	}

	void ProgramEngine::Update()
	{
		auto sceneIter = sceneManager.GetFirstActiveSceneIter();
		auto endIter = sceneManager.GetEndIter();

		for (; sceneIter != endIter; ++sceneIter)
			(*sceneIter)->Update();
	}

	void ProgramEngine::Render()
	{
		AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

		auto sceneIter = sceneManager.GetFirstSceneIter();
		auto endIter = sceneManager.GetEndIter();

		for (; sceneIter != endIter; ++sceneIter)
			(*sceneIter)->Render();
	}

	void ProgramEngine::PushScene(Scene::SCENE_IDENTIFIER sceneIdentifier)
	{
		sceneManager.QueuePushScene(sceneIdentifier);
	}
	void ProgramEngine::PopScene()
	{
		sceneManager.QueuePopScene();
	}
	void ProgramEngine::SwitchToScene(Scene::SCENE_IDENTIFIER sceneIdentifier)
	{
		sceneManager.QueueSwitchToScene(sceneIdentifier);
	}
	int ProgramEngine::GetNumLoadedScenes() const
	{
		return sceneManager.GetNumLoadedScenes();
	}
	void ProgramEngine::Quit()
	{
		isRunning = 0;
	}
	s32 ProgramEngine::GetWinWidth()
	{
		return C_windowWidth;
	}
	s32 ProgramEngine::GetWinHeight()
	{
		return C_windowHeight;
	}

	void ProgramEngine::SetShowDebugInfo(bool showDebug)
	{
		C_debugDoRender = C_errDoRender = showDebug;
	}
}
