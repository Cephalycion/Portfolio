/******************************************************************************/
/*!
\file   E_Engine.h
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
  This is an interface file for the backend engine manager, interfacing with
  AEEngine and WINAPI for the running of the program window and handles the
  the running of the scene management system.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "pch.h"
#include "E_SceneManager.h"
#include "E_Constants.h"
#include <vector>
#include <Windows.h>
#include <CommCtrl.h>

#pragma comment(lib,"comctl32.lib")

// There was originally going to be support for user resizing of the game window.
// Unfortunately it seems like AlphaEngine does not support updating of its internal window size on the fly:
//   - No function to set window size via AlphaEngine
//	 - AESysInit()'s pWinCallBack parameter (WNDPROC callback to listen for window size changes) seems to be broken:
//		- After the WM_NCCREATE message that is expected when the window is created, the program is immediately sent
//		  a WM_NCDESTROY, shutting down the window and causing it to never be displayed.
//		  This seems to be an indication of bad hooking with WINAPI (see https://stackoverflow.com/questions/65619337/),
//		  which we obviously cannot fix on our side.
//		  * There of course may be another reason for the WM_NCDESTROY though I haven't found one.
//		  * We can bypass this parameter with SetWindowSubclass() which actually works perfectly, but
//			still, AlphaEngine doesn't know (or listen for) window size changes, and we can't inform
//			AlphaEngine ourselves because again AlphaEngine doesn't expose any such function to do so.


namespace Engine {

	class ProgramEngine
	{
	private:
		// Disallow copy and move
		ProgramEngine(const ProgramEngine&) = delete;
		ProgramEngine& operator=(const ProgramEngine&) = delete;
		ProgramEngine(ProgramEngine&&) = delete;
		ProgramEngine& operator=(ProgramEngine&&) = delete;

	protected:
		static LRESULT WndProcCallback(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
		static void SetCursorConstraint();

		void Update();
		void Render();

		bool isRunning;
		Scene::SceneManager sceneManager;

	public:
		ProgramEngine();
		~ProgramEngine();

		// Program Init
		void Initialize(::HINSTANCE hInstance, s32 nCmdShow);
		// Program Shutdown
		void Shutdown();

		// Game Loop
		void Run();

		// Scene Management
		void PushScene(Scene::SCENE_IDENTIFIER sceneIdentifier);
		void PopScene();
		void SwitchToScene(Scene::SCENE_IDENTIFIER sceneIdentifier);
		int GetNumLoadedScenes() const;

		void Quit();
		s32 GetWinWidth();
		s32 GetWinHeight();
		// Program Settings
		void SetShowDebugInfo(bool showDebug);
	};
}
