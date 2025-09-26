/******************************************************************************/
/*!
\file   G_Constants.cpp
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Ryan Chan Kuan Fu (35%)
\par    email: c.kuanfuryan\@digipen.edu
\par    DigiPen login: c.kuanfuryan

\author Kendrick Sim Hean Guan (35%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\author Marc Alviz Evangelista (21%)
\par    email: marcalviz.e\@digipen.edu
\par    DigiPen login: marcalviz.e

\author Matthew Chan Shao Jie (9%)
\par    email: m.chan\@digipen.edu
\par    DigiPen login: m.chan

\brief
  Bunch of different const values to be used with game-related classes
  and functions.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#include "pch.h"
#include "G_Constants.h"

namespace Game {

	// =========================
	// g_blockTexture
	// =========================
	const char* C_bTexFilepath = "Textures/Blocks/";
	const char* C_bTexFiletype = ".png";


	/* HACKS */
	bool C_hackFreecam = false;
	bool C_hackNoclip = false;
	bool C_hackInvincible = false;
	bool C_hackDemo = false;
}
