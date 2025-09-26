/******************************************************************************/
/*!
\file   E_Parallax.cpp
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Kendrick Sim Hean Guan (100%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\brief
  This is the implementation file for applying a parallax effect to objects rendered
  by modifying the position of the camera.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#include "pch.h"
#include "E_Parallax.h"

namespace Engine {

	ParallaxManager::ParallaxManager()
		: referenceCam(nullptr)
		, parallaxCam(new Camera())
		, scale(1.0f)
	{
	}

	void ParallaxManager::SetReferenceCam(const sPtr<Camera>& newReferenceCam)
	{
		referenceCam = newReferenceCam;

		// Copy rotation and zoom. Position will be calculated later in ApplyParallax()
		parallaxCam->SetRotation(referenceCam->GetRotation());
		parallaxCam->SetZoom(referenceCam->GetZoom());
	}

	void ParallaxManager::ApplyParallax(float newScale)
	{
		scale = newScale;
		if (std::fabs(scale - 1.0f) < C_epsilon)
		{
			// Scale == 1.0f -> Effectively no parallax
			Singleton<CameraManager>::Instance()->SetActiveCam(referenceCam);
			return;
		}

		RefreshParallaxCam();
		Singleton<CameraManager>::Instance()->SetActiveCam(parallaxCam);
	}

	void ParallaxManager::RefreshParallaxCam()
	{
		parallaxCam->SetPosition(referenceCam->GetPosition() * scale);
	}

	void ParallaxManager::DisableParallaxCam()
	{
		Singleton<CameraManager>::Instance()->SetActiveCam(referenceCam);
	}

}
