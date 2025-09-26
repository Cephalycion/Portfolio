/******************************************************************************/
/*!
\file   E_Constants.h
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

#pragma once
#include "E_AEEngineInterface.h"
#include <string>
#include <memory>

/* Forward Declarations */
namespace Game {
	class PhysicsManager;
}

namespace Engine {

	/* Macros
	*/
#define sPtr std::shared_ptr
#define uPtr std::unique_ptr
#define wPtr std::weak_ptr
#define sCast static_cast
#define dCast dynamic_cast
#define rCast reinterpret_cast
#define cCast const_cast


	/*	Place universal constant program settings here.
	*	
	*	Note: Do not put classes here as they will memory leak (e.g. std::string, etc.).
	*/

	extern const char* C_programName;

	extern int C_windowWidth, C_windowHeight;	// The window size of the program
	extern int C_renderWidth, C_renderHeight;	// The internal render size.
	extern float C_effectiveWidth, C_effectiveHeight; // The size that you'd have to render internally to fill the screen.
	extern bool C_enableConsole;
	extern int C_fpsTarget;						// The FPS target of the program
	extern float C_fpsActual;					// The FPS of the current frame
	extern float C_frametime;					// The dt of this frame (IN SECONDS)
	extern unsigned long long C_fpsCount;		// The number of frames rendered
	extern bool C_enableVSync;
	extern bool C_isFullscreen;					// Is the program fullscreen
	extern bool C_skipSplash;					// Flag to skip the splash screen

	// =========================
	// COLOR
	// =========================
	extern const Graphics::Color C_colorWhite;
	extern const Graphics::Color C_colorBlack;
	extern const Graphics::Color C_colorRed;
	extern const Graphics::Color C_colorGreen;
	extern const Graphics::Color C_colorBlue;
	extern const Graphics::Color C_colorOrange;

	// =========================
	// MATH
	// =========================
	extern const f32 C_pi;					// PI
	extern const f32 C_eulerConstant;			// Euler's constant
	extern const f32 C_epsilon;				// Floating-point tolerance.

	// =========================
	// e_error
	// =========================
	extern bool C_errDoRender;				// Flag for whether to render errors to the screen
	extern const char* C_errFontFilepath;
	extern f32 C_errFontSize;					// Size of the text
	extern int C_errMaxLength;					// Maximum length of each error text log
	extern int C_errMaxLogEntries;				// How many error log entries to render to the screen at once
	extern float C_errEntryLifetime;				// How long to keep an error log entry on the display, in seconds
	extern float C_errFadeLength;				// How long to fade out an error. Must not be more than UT_C_errEntryLifetime

	// =========================
	// e_debug
	// =========================
	extern bool C_debugDoRender;				// Flag for whether to render debug info to the screen
	extern int C_debugMsgMaxLength;				// Maximum number of characters that can be printed to the screen per variable. Includes the null character.
	extern const char* C_debugFontFilepath;
	extern float C_debugFontSize;				// Font size of the debug text

	// =========================
	// e_assetManager
	// =========================
	extern const char* C_assetTextureFilepath;
	extern const char* C_assetFontFilepath;
	extern s32 C_assetFontSize;

	// =========================
	// e_text
	// =========================
	extern const char* C_textDefaultFontFile;	// TextBuilder's default font file

	// =========================
	// e_physics
	// =========================
	using C_TYPE_PHYSICSMANAGER = Game::PhysicsManager;
	extern Vec2 C_physicsGravity;
	extern f32 C_physicsMaxVelocity;
	extern f32 C_physicsMaxAngVelocity;
	extern f32 C_physicsSettleVelocity;			// Blocks settle only when its velocity^2 is less than this value
	extern f32 C_physicsSettleAngVelocity;		// Blocks settle only when its angular velocity is less than this value
	extern f32 C_physicsSettleAngle;			// Blocks settle only when it's alignment error with the collision normal is less than this angle

	// =========================
	// e_particleSystem
	// =========================
	extern s32 C_particleReservedCount;			// Number of objects reserved in memory
	extern s32 C_particlePhysReservedCount;		// Number of physics objects reserved in memory
	extern s32 C_particleSpawnerMaxCountPerLayer;// Maximum number of spawners per layer

	enum FILEPATH
	{
		// LEVELS
		FL_LEVEL1 = 0,
		FL_LEVEL2,
		FL_LEVEL3,
		FL_LEVEL_DEMO,
		FL_END,
		FL_LEVELDEBUG,

		// MUSIC
		BGM_START,
		BGM_MENU,
		BGM_LEVEL1,
		BGM_END,

		// SOUND
		SFX_START,
		SFX_HOOK,
		SFX_DASH,
		SFX_PICK,
		SFX_HURT,
		SFX_THROW,
		SFX_PLAYERHURT,
		SFX_BUTTON,
		SFX_END,

		// PNG IMAGES
		PNG_SPLASH_DIGIPEN,
		PNG_SPLASH_FMOD,
		PNG_SPLASH_9PERCENT,
		PNG_SPLASH_9PERCENT_GLITCH,

		PNG_ARROW,
		PNG_CIRCLE,
		PNG_BG_CLOUDS,
		PNG_BG_SKY,
		PNG_BG_HILLS,
		PNG_BG_WALL,
		PNG_PARTICLE_DEATHSHINE,
		PNG_PARTICLE_EXPLOSION_CREEPER,
		PNG_END,

		// SETTINGS
		SETTINGS,

		// ENDING ENUMERATOR (No value in array!)
		// Denotes the number of strings in the array
		// DO NOT REMOVE
		FILEPATH_SIZE,
	};
	extern const char* C_filePaths[FILEPATH_SIZE];
}