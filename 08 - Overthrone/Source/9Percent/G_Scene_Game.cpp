/******************************************************************************/
/*!
\file   G_Scene_Game.cpp
\par    Project: OverThrone
\par    Course: CSD1451
\par    Section A
\par    Software Engineering Project 2
\date   04/04/2024

\author Kendrick Sim Hean Guan (40%)
\par    email: kendrickheanguan.s\@digipen.edu
\par    DigiPen login: kendrickheanguan.s

\author Ryan Chan Kuan Fu (30%)
\par    email: c.kuanfuryan\@digipen.edu
\par    DigiPen login: c.kuanfuryan

\author Marc Alviz Evangelista (21%)
\par    email: marcalviz.e\@digipen.edu
\par    DigiPen login: marcalviz.e

\author Matthew Chan Shao Jie (9%)
\par    email: m.chan\@digipen.edu
\par    DigiPen login: m.chan

\brief
  This is the implementation file for all member functions of a class called
  Scene_Game.

All content © 2024 DigiPen Institute of Technology Singapore.
All rights reserved.
*/
/******************************************************************************/

#include "pch.h"
#include "G_Scene_Game.h"
#include "E_Engine.h"
#include "E_Parallax.h"
#include "G_Player.h"
#include "G_EnemyBoss.h"
#include "G_LevelObjects.h"
#include "G_TiledBackground.h"
#include "G_BlockTexture.h"
#include "G_TextLayer.h"
#include "G_GameState.h"
#include "E_IOManager.h"
#include "E_ParticleSystem.h"
#include "G_Sign.h"

#include "G_ParticleSystem.h"

namespace Game {

	using namespace Engine;

	void Scene_Game::LoadNextLevel(const sPtr<Map>& mapPtr)
	{
		Singleton<Game::Player>::Instance()->OnNewLevelLoad();
		Singleton<ParticleManager>::Instance()->DeleteAllLayers();
		Singleton<LevelObjectsManager>::Instance()->DeleteAllLayers();
		Singleton<TextLayerManager>::Instance()->DeleteAllLayers();
		mapPtr->loadNext = false;
		switch (mapPtr->currLevel)
		{
		default:
		case 1:
			Singleton<IOManager>::Instance()->ReadLevel(FL_LEVEL1);
			break;
		case 2:
			Singleton<IOManager>::Instance()->ReadLevel(FL_LEVEL2);
			break;
		case 3:
			Singleton<IOManager>::Instance()->ReadLevel(FL_LEVEL3);
			break;
		case 4:
			Singleton<IOManager>::Instance()->ReadLevel(FL_LEVEL_DEMO);
			break;
		}
		mapPtr->Init(this, Singleton<Camera>::Instance());


		// Initialize parallax background
		Singleton<LevelObjectsManager>::Instance()->AddObject(OBJECTS_LAYER_PARALLAX_25, sPtr<Object>(new MovingTiledBackgroundObject(
			C_filePaths[PNG_BG_CLOUDS], MakeVec2(1800.0f, 900.0f),
			TiledBackgroundObject::CalcPosBasedOnMapAndParallax(900.0f, 0.25f, 0.4f), MakeVec2(20.0f, 0.0f), true, false))
		);
		Singleton<LevelObjectsManager>::Instance()->AddObject(OBJECTS_LAYER_PARALLAX_50, sPtr<Object>(new TiledBackgroundObject(
			C_filePaths[PNG_BG_HILLS], C_bgHillsSize,
			TiledBackgroundObject::CalcPosBasedOnMapAndParallax(C_bgHillsSize.y, 0.5f), true, false))
		);
		Singleton<LevelObjectsManager>::Instance()->AddObject(OBJECTS_LAYER_PARALLAX_75, sPtr<Object>(new TiledBackgroundObject(
			C_filePaths[PNG_BG_WALL], C_bgWallSize,
			TiledBackgroundObject::CalcPosBasedOnMapAndParallax(C_bgWallSize.y, 0.75f), true, false))
		);
		Singleton<LevelObjectsManager>::Instance()->AddObject(OBJECTS_LAYER_SKYBOX, ObjectBuilder::CreateSquare(
			Transform(0.0f, 0.0f, 0.0f, sCast<f32>(C_windowWidth), sCast<f32>(C_windowHeight)),
			SpriteBuilder::CreateSpriteSquare(C_filePaths[PNG_BG_SKY])
		));
	}

	void Scene_Game::Init()
	{
#ifdef _DEBUG
		// If the game state manager is already initialized before we set it up, something is really wrong...
		assert(!Singleton<GameStateManager>::IsInitialized());
#endif
		// Setup gamestate
		Singleton<GameStateManager>::Instance();

		// Setup parallax camera
		Singleton<ParallaxManager>::Instance()->SetReferenceCam(Singleton<Camera>::Instance());

		// Load game textures
		Singleton<BlockTextureManager>::Instance();
		// Get a reference to the level objects manager
		auto objManager = Singleton<LevelObjectsManager>::Instance();

		// Read map from .csv
		LoadNextLevel(Singleton<Map>::Instance());
		
		Engine::AudioManager::playAudio(Engine::BGM_LEVEL1, true);
	}

	void Scene_Game::Update()
	{
		// Limit update to less than 1000ms/C_fpsTarget so that physics doesn't break
		// and things are consistent.
		// But only do this when the player hasn't finished the level so that the ending
		// where all blocks break looks good and not slow-mo for no reason.
		// The 0.0002f is a bit of tolerance for frametime fluctuations.
		if (C_frametime > 1.0f / sCast<f32>(C_fpsTarget) + 0.0002f &&
			!Singleton<GameStateManager>::Instance()->GetIsLevelClear())
		{
			Error::RaiseErrFormatted("Scene_Game::Update", "Limited frametime (was %f)", ERROR_INFO, C_frametime);
			C_frametime = 1.0f / sCast<f32>(C_fpsTarget);
		}

		/* Hacks: Timewarp */
		static bool slo = false;
		if (AEInputCheckTriggered(AEVK_M))
			slo = !slo;
		if (slo)
			C_frametime /= 5.0f;

		/* Hacks: Switch Level */
		if (AEInputCheckTriggered(AEVK_PERIOD))
			Singleton<Map>::Instance()->NextLevel();
		else if (AEInputCheckTriggered(AEVK_COMMA) && Singleton<Map>::Instance()->currLevel > 1)
			Singleton<Map>::Instance()->NextLevel(Singleton<Map>::Instance()->currLevel - 1);
		/* Hacks: Go to Demo Level */
		if (AEInputCheckTriggered(AEVK_SLASH))
		{
			C_hackDemo = true;
			Singleton<Map>::Instance()->currLevel = 4;
			Singleton<Map>::Instance()->loadNext = true;
		}

		sPtr<Map> blockMap = Singleton<Map>::Instance();
		if (blockMap->loadNext)
			LoadNextLevel(blockMap);

		if (AEInputCheckTriggered(AEVK_ESCAPE) || AEInputCheckTriggered(AEVK_P))
			Singleton<ProgramEngine>::Instance()->PushScene(SCENE_PAUSE);

		if (AEInputCheckTriggered(AEVK_0))
			C_hackFreecam = !C_hackFreecam;
		if (C_hackFreecam)
		{
			if (AEInputCheckCurr(AEVK_UP))
				Singleton<Camera>::Instance()->SetPosition(Singleton<Camera>::Instance()->GetPosition() + MakeVec2(0.0f, 10.0f));
			if (AEInputCheckCurr(AEVK_DOWN))
				Singleton<Camera>::Instance()->SetPosition(Singleton<Camera>::Instance()->GetPosition() + MakeVec2(0.0f, -10.0f));
			if (AEInputCheckCurr(AEVK_LEFT))
				Singleton<Camera>::Instance()->SetPosition(Singleton<Camera>::Instance()->GetPosition() + MakeVec2(-10.0f, 0.0f));
			if (AEInputCheckCurr(AEVK_RIGHT))
				Singleton<Camera>::Instance()->SetPosition(Singleton<Camera>::Instance()->GetPosition() + MakeVec2(10.0f, 0.0f));
		}

		// Only update the game systems if we're not focusing on the boss
		if (Singleton<GameStateManager>::Instance()->GetIsFocusBossDying())
		{
			// Only update the boss
			Singleton<LevelObjectsManager>::Instance()->RunMacro(OBJECTS_LAYER_FOREGROUND, [](const sPtr<Object>& obj) -> bool {
				if (std::dynamic_pointer_cast<EnemyBoss>(obj))
					obj->Update();
				return true;
			});
			// Update the camera
			Singleton<EnemyBossDeadCamManager>::Instance()->Update(C_frametime);
			// For updating sign objects' text locations
			Singleton<LevelObjectsManager>::Instance()->Update(OBJECTS_LAYER_BACKGROUND, C_frametime);
		}
		else
		{
			// Update physics
			auto physicsManager = Singleton<C_TYPE_PHYSICSMANAGER>::Instance();
			physicsManager->UpdatePhysics(C_frametime);
			physicsManager->UpdateCollision();

			// Update the map
			blockMap->Update();
			Singleton<ProjectileManager>::Instance()->UpdateList();
			// Update objects
			Singleton<LevelObjectsManager>::Instance()->RunMacro(OBJECTS_LAYER_FOREGROUND, [&blockMap = blockMap](const sPtr<Object>& obj) -> bool {
				if (!obj)
					return false;

				// Autokill for out-of-bounds objects
				if (obj->GetPosition().y <= -((f32)blockMap->height * C_blockLength) - 1000.0f)
					return false;

				return true;
			});
			Singleton<LevelObjectsManager>::Instance()->Update(C_frametime);
		}

		// Update game texture animations
		Singleton<BlockTextureManager>::Instance()->UpdateTexturesAnimation(C_frametime);

		// Update particles
		Singleton<ParticleManager>::Instance()->Update(C_frametime);

		// Insert pending text objects
		Singleton<TextLayerManager>::Instance()->Update(C_frametime);
	}
	void Scene_Game::Render()
	{
		// Render parallax backgrounds
		Singleton<LevelObjectsManager>::Instance()->Render(OBJECTS_LAYER_SKYBOX);
		Singleton<ParallaxManager>::Instance()->ApplyParallax(0.25f);
		Singleton<LevelObjectsManager>::Instance()->Render(OBJECTS_LAYER_PARALLAX_25);
		Singleton<ParallaxManager>::Instance()->ApplyParallax(0.5f);
		Singleton<LevelObjectsManager>::Instance()->Render(OBJECTS_LAYER_PARALLAX_50);
		Singleton<ParallaxManager>::Instance()->ApplyParallax(0.75f);
		Singleton<LevelObjectsManager>::Instance()->Render(OBJECTS_LAYER_PARALLAX_75);
		Singleton<ParallaxManager>::Instance()->DisableParallaxCam();

		// Render the foreground map and objects
		Singleton<CameraManager>::Instance()->SetActiveCam(Singleton<Camera>::Instance());

		Singleton<ParticleManager>::Instance()->Render(PARTICLE_LAYER_GAME);
		Singleton<LevelObjectsManager>::Instance()->Render(OBJECTS_LAYER_BACKGROUND);
		Singleton<Map>::Instance()->Render();
		Singleton<ParticleManager>::Instance()->Render(PARTICLE_LAYER_ABOVE_MAP);
		Singleton<LevelObjectsManager>::Instance()->Render(OBJECTS_LAYER_FOREGROUND);
		Singleton<ProjectileManager>::Instance()->RenderList();
		Singleton<ParticleManager>::Instance()->Render(PARTICLE_LAYER_ABOVE_DYNAMIC);

		Singleton<CameraManager>::Instance()->ResetActiveCam();

		// Render UI here
		Singleton<TextLayerManager>::Instance()->Render(TEXT_LAYER_INGAME_OVERLAY);
		Singleton<TextLayerManager>::Instance()->Render(TEXT_LAYER_SCREEN_OVERLAY);
	}
	void Scene_Game::Exit()
	{
		Singleton<CameraManager>::Instance()->ResetActiveCam();
		Singleton<EnemyBossDeadCamManager>::Destroy();
		Singleton<ParallaxManager>::Destroy();
		Engine::AudioManager::stopAudio();

		Singleton<TextLayerManager>::Destroy();

		Singleton<ParticleManager>::Destroy();
		Singleton<ProjectileManager>::Destroy();
		Singleton<PhysicsManager>::Destroy();
		Singleton<Map>::Destroy();
		Singleton<Player>::Destroy();
		Singleton<LevelObjectsManager>::Destroy();
		Singleton<BlockTextureManager>::Destroy();

		Singleton<GameStateManager>::Destroy();
	}
}