/******************************************************************************/
/*!
\file   E_Parallax.h
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Kendrick Sim Hean Guan (100%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\brief
  This is an interface file for applying a parallax effect to objects rendered
  by modifying the position of the camera.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "pch.h"
#include "E_Camera.h"

namespace Engine {

	class ParallaxManager
	{
	protected:
		sPtr<Camera> referenceCam; // Parallax calculations will be relative to this camera
		sPtr<Camera> parallaxCam; // This camera will have parallax calculations applied onto it

		float scale; // The scale of parallax

	public:
		ParallaxManager();

		// Sets which camera to take positional reference from when calculating parallax effects
		void SetReferenceCam(const sPtr<Camera>& newReferenceCam);

		// Sets a camera with parallax of the specified scale as the active camera. Lower scale
		// slows movement relative to the reference camera.
		// Making positional changes to the reference cam after calling this function will not
		// apply those changes to the parallax camera. Call RefreshParallaxCam() to force changes
		// in such cases as needed.
		void ApplyParallax(float newScale);
		// Calculates parallax based on the referenceCam and applies it to the parallax camera.
		void RefreshParallaxCam();
		// Reinstates the reference camera as the active camera.
		void DisableParallaxCam();

	};

}

