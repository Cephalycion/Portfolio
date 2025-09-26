/******************************************************************************/
/*!
\file   E_Debug.cpp
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Kendrick Sim Hean Guan (100%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\brief
  This is the implementation file for a system that prints variables to the screen
  for debug purposes.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#include "pch.h"
#include "E_Asset.h"

namespace Engine {

	namespace INTERNAL {

		void VariableTracker_Base::PrintToStr(std::string* out)
		{
			out->assign(name);
			out->append(": ");
			AppendVariableToStr(out);
		}

		/* Specializations */
		void VariableTracker_TemplateBase<Vec2>::AppendVariableToStr(std::string* str)
		{
			const Vec2* variable = GetVariable();
			str->append(std::to_string(variable->x));
			str->append(",");
			str->append(std::to_string(variable->y));
		}
	}

	Debug::Debug()
		: text("", C_debugFontSize, MakeVec2(C_windowWidth * -0.5f, 0.0f), Color(), false, AssetManager::GetFont(C_debugFontFilepath), TEXT_ALIGN_LEFT, TEXT_ALIGN_TOP)
	{
	}

	void Debug::RemoveAllValueTrackers()
	{
		valuesVec.clear();
	}

	void Debug::RenderAll()
	{
		if (!C_debugDoRender)
			return;

		if (trackersVec.empty() && valuesVec.empty())
			return;


		f32 renderY = C_windowHeight * 0.5f;
		std::string buffer;

		auto RenderVec = [this, &renderY, &buffer](std::vector<sPtr<INTERNAL::VariableTracker_Base>>& vec) -> void {
			for (auto& debugLog : vec)
			{
				debugLog->PrintToStr(&buffer);
				text.SetTextStr(buffer);
				text.SetPosition(MakeVec2(text.GetPosition().x, renderY));
				text.Render();
				renderY -= C_debugFontSize * C_assetFontSize;
			}
		};

		RenderVec(trackersVec);
		RenderVec(valuesVec);

		RemoveAllValueTrackers();
	}

}
