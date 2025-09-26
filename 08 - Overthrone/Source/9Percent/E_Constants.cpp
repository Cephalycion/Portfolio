/******************************************************************************/
/*!
\file   E_Constants.cpp
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Kendrick Sim Hean Guan (70%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\author Ryan Chan Kuan Fu (30%)
\par    email: c.kuanfuryan\@digipen.edu
\par    DigiPen login: c.kuanfuryan

\brief
  Bunch of different extern const values to be used with engine-related classes
  and functions.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

/*	Place universal constant program settings here.
*/
#include "pch.h"
#include "E_Constants.h"

namespace Engine {

	const char* C_programName = "OverThrone";

	int C_windowWidth = 1600, C_windowHeight = 900; // The window size of the program
	int C_renderWidth = 1600, C_renderHeight = 900; // The internal render size.
	float C_effectiveWidth, C_effectiveHeight; // The size that you'd have to render internally to fill the screen. (Initialized at runtime by E_Engine)
	bool C_enableConsole = true;
	int C_fpsTarget = 60; // The FPS target of the program
	float C_fpsActual = -1.0f; // The FPS of the current frame (will be updated each frame by E_Engine)
	float C_frametime = -1.0f; // The dt of this frame (IN SECONDS) (will be updated each frame by E_Engine)
	unsigned long long C_fpsCount = 0; // The number of frames rendered
	bool C_enableVSync = true;
	bool C_isFullscreen = 0; // Is the program fullscreen
#ifdef _DEBUG
	bool C_skipSplash = 1; // Flag to skip the splash screen. Disabled in release mode
#else
	bool C_skipSplash = 0;
#endif

	// =========================
	// COLOR
	// =========================
	const Graphics::Color C_colorWhite = MakeColor(255, 255, 255);
	const Graphics::Color C_colorBlack = MakeColor(0, 0, 0);
	const Graphics::Color C_colorRed = MakeColor(255, 0, 0);
	const Graphics::Color C_colorGreen = MakeColor(0, 255, 0);
	const Graphics::Color C_colorBlue = MakeColor(0, 0, 255);
	const Graphics::Color C_colorOrange = MakeColor(243, 152, 0);

	// =========================
	// MATH
	// =========================
	constexpr f32 C_pi = 3.14156925f; // PI
	constexpr f32 C_eulerConstant = 2.7182818f; // Euler's constant
	constexpr f32 C_epsilon = 0.000001f; // Floating-point tolerance.

	// =========================
	// ut_error
	// =========================
	bool C_errDoRender = false; // Flag for whether to render errors to the screen. Disabled by default in release mode (in E_Engine)
	const char* C_errFontFilepath = "liberation-mono.ttf";
	f32 C_errFontSize = 0.3f; // Size of the text
	int C_errMaxLength = 254; // Maximum length of each error text log
	int C_errMaxLogEntries = 15; // How many error log entries to render to the screen at once
	float C_errEntryLifetime = 6.0f; // How long to keep an error log entry on the display, in seconds
	float C_errFadeLength = 2.0f; // How long to fade out an error. Should not be more than C_errEntryLifetime

	// =========================
	// ut_debug
	// =========================
	bool C_debugDoRender = false; // Flag for whether to render debug info to the screen. Disabled by default in release mode (in E_Engine)
	int C_debugMsgMaxLength = 64; // Maximum number of characters that can be printed to the screen per variable. Includes the null character.
	const char* C_debugFontFilepath = "liberation-mono.ttf";
	float C_debugFontSize = 0.15f; // Size of the text

	// =========================
	// e_assetManager
	// =========================
	const char* C_assetTextureFilepath = "Assets/";
	const char* C_assetFontFilepath = "Assets/Fonts/";
	s32 C_assetFontSize = 128;

	// =========================
	// e_text
	// =========================
	const char* C_textDefaultFontFile = "ParmaPetitOutline.ttf";// TextBuilder's default font file

	// =========================
	// e_physics
	// =========================
	Vec2 C_physicsGravity = MakeVec2(0.0f, -980.0f);
	f32 C_physicsMaxVelocity = 1400.0f;
	f32 C_physicsMaxAngVelocity = 360.0f * 4;
	f32 C_physicsSettleVelocity = 500.0f; // Blocks settle only when its velocity^2 is less than this value
	f32 C_physicsSettleAngVelocity = 100.0f; // Blocks settle only when its angular velocity is less than this value
	f32 C_physicsSettleAngle = 7.0f; // Blocks settle only when it's alignment error with the collision normal is less than this angle

	// =========================
	// e_particleSystem
	// =========================
	s32 C_particleReservedCount = 32; // Number of objects reserved in memory
	s32 C_particlePhysReservedCount = 128; // Number of physics objects reserved in memory
	s32 C_particleSpawnerMaxCountPerLayer = 25; // Maximum number of spawners per layer

	// Hey there Ryan here
	// Here's an enum to const char* array system I devised
	// Each enum corresponds to a filepath
	// This way I can abstract out strings from my function params and just use enums instead
	const char* C_filePaths[FILEPATH_SIZE]
	{
		"Assets/Levels/Level1.csv",
		"Assets/Levels/Level2.csv",
		"Assets/Levels/Level3.csv",
		"Assets/Levels/Demo.csv",
		"LEVEL ENDS HERE",
		"Assets/Levels/Level_Debug.csv",

		"MUSIC STARTS HERE",
		"Assets/Audio/BGM_MENU.mp3",
		"Assets/Audio/BGM_LEVEL1.mp3",
		"MUSIC ENDS HERE",

		"SOUND STARTS HERE",
		"Assets/Audio/SFX_HOOK.mp3",
		"Assets/Audio/SFX_DASH.mp3",
		"Assets/Audio/SFX_PICK.mp3",
		"Assets/Audio/SFX_HURT.mp3",
		"Assets/Audio/SFX_THROW.mp3",
		"Assets/Audio/SFX_PLAYERHURT.mp3",
		"Assets/Audio/SFX_BUTTON.mp3",
		"SOUND ENDS HERE",

		"Textures/Splash/DigiPen_Singapore_WEB_RED.png",
		"Textures/Splash/FMOD.png",
		"Textures/Splash/9Percent.png",
		"Textures/Splash/9PercentGlitch.png",

		"arrow.png",
		"circle.png",
		"Textures/Background/BG-Clouds.png",
		"Textures/Background/BG-Sky.png",
		"Textures/Background/BG-Hills.png",
		"Textures/Background/BG-Wall.png",
		"Textures/Particles/shineBurst.png",
		"Textures/Particles/explosion_creeper.png",
		"PNG ENDS HERE",

		"Assets/settings.txt"
	};
}
