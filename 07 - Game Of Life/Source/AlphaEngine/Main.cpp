/*!************************************************************************
\file Main.cpp
\author Chan Kuan Fu Ryan
\par DP email: c.kuanfuryan@digipen.edu
\par Course: CSD1451
\par Game Of Life Exercise
\date 11/1/2024
\brief
This source file emulates Game Of Life with 30x30 cells. 
Includes some legacy code from Alpha Engine Tutorial.
The functions include:
- getCell
	Takes in a row and column and returns the id of the cell at that location.
- getNeighbours
	Takes in an id and returns the number of live neighbours of that cell.
- wWinMain
	Main function for the application.
**************************************************************************/
// ---------------------------------------------------------------------------
// includes

#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"
// #include <iostream>

// Number of cells in total.
#define CELL_NUM 900

// Number of cells in a row. 
// Not to be confused with the pixel size of a cell.
#define CELL_SIZ 30		

// Planets can rotate around each other (LEGACY)
/*
typedef struct Planet {
	struct Planet* parent;	// Who this planet is parented to

	f32 scale;			// absolute scale from the parent
	f32 pos_x, pos_y;	// absolute position relative to parent

	f32 orbit;			// current orbit angle relative to parent
	f32 orbit_speed;	// orbit speed

	f32 spin;			// current spin angle
	f32 spin_speed;		// spin speed

	AEMtx33 transform;  // Final transformation matrix for rendering
} Planet;
*/

// Cell struct (I'm going to use this instead of a class because I'm too lazy)
typedef struct Cell
{
	bool currentState;	// Is it alive right now? (For render)
	bool futureState;	// Should it be alive next turn?
} Cell;

Cell CELL_ARR[CELL_NUM];

s32 getCell(s32 row, s32 col) 
{ 
	return col + (row * CELL_SIZ); 
}

s32 getNeighbours(s32 id)
{
	s32 row = id / CELL_SIZ;
	s32 col = id % CELL_SIZ;
	s32 startrow = row - 1;
	s32 startcol = col - 1;
	s32 ret = 0;

	for (s32 i = 0; i < 3; ++i)
	{
		for (s32 j = 0; j < 3; ++j)
		{
			s32 rowCheck = startrow + i;
			s32 colCheck = startcol + j;

			if (rowCheck < 0 || rowCheck >= CELL_SIZ ||	// Row must be within bounds
				colCheck < 0 || colCheck >= CELL_SIZ ||	// Column must be within bounds
				(rowCheck == row && colCheck == col))	// Don't check against itself
				continue;

			if (CELL_ARR[getCell(rowCheck, colCheck)].currentState == true)
				++ret;
		}
	}
	return ret;
}

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

	int gGameRunning = 1;

	// Initialization of your own variables go here
	// u32 CL_RED = 0xFFFF0000;
	// u32 CL_GRN = 0xFF00FF00;
	// u32 CL_BLU = 0xFF0000FF;
	// u32 CL_WHT = 0xFFFFFFFF;
	// u32 CL_BLK = 0x00000000;
	u32 CL_DRK = 0xFF555555;
	f32	CELL_LEN = 30.f; // We'll do 30px cells for this assignment
	s32 WIN_WIDTH = 900;
	s32 WIN_HEIGHT = 900;
	bool simulating = false;

	// Initialise CELL_ARR
	for (int i = 0; i < CELL_NUM; ++i)
	{
		CELL_ARR[i].currentState = false;
		CELL_ARR[i].futureState = false;
	}

	// Using custom window procedure
	AESysInit(hInstance, nCmdShow, WIN_WIDTH, WIN_HEIGHT, 1, 60, true, NULL);

	// Changing the window title
	// AESysSetWindowTitle("My New Demo!");
	AESysSetWindowTitle("Game Of Life");

	// reset the system modules
	AESysReset();

	// MESH CREATION
	// We inform Alpha Engine that we are going to create a mesh
	AEGfxMeshStart();

	// AEGfxTriAdd() takes in 3 sets of 5 parameters,
	// totalling to a whooping 15 parameters to input.
	//
	// Each set represents
	// This tells Alpha Engine that the mesh will contain 2
	// triangles that makes up a white square.
	//
	// Notice that this 1x1 white square is centered around the origin.
	//

	// Rainbow mesh (LEGACY)
	/*
	AEGfxTriAdd(
		-0.5f, -0.5f, CL_RED, 0.0f, 1.0f,	// bottom left is red
		0.5f, -0.5f, CL_GRN, 1.0f, 1.0f,	// bottom right is green
		-0.5f, 0.5f, CL_BLU, 0.0f, 0.0f);	// top left is blue

	AEGfxTriAdd(
		0.5f, -0.5f, CL_GRN, 1.0f, 1.0f,	// bottom right is green
		0.5f, 0.5f, CL_WHT, 1.0f, 0.0f,		// top right is white
		-0.5f, 0.5f, CL_BLU, 0.0f, 0.0f);	// top left is blue
	*/

	AEGfxTriAdd(
		-0.5f, -0.5f,	CL_DRK, 0.0f, 1.0f,
		0.5f, -0.5f,	CL_DRK, 1.0f, 1.0f,
		-0.5f, 0.5f,	CL_DRK, 0.0f, 0.0f);

	AEGfxTriAdd(
		0.5f, -0.5f,	CL_DRK, 1.0f, 1.0f,
		0.5f, 0.5f,		CL_DRK, 1.0f, 0.0f,
		-0.5f, 0.5f,	CL_DRK, 0.0f, 0.0f);

	// Pointer to Mesh. All our sprites use the same mesh, so we can just have one.
	AEGfxVertexList* pMesh = 0;

	// Saving the mesh (list of triangles) in pMesh
	pMesh = AEGfxMeshEnd();

	// This will load a texture in `Assets/PlanetTexture.png`
	// and store it in pTex.
	// AEGfxTexture* pTex = AEGfxTextureLoad("Assets/PlanetTexture.png");
	AEGfxTexture* pTex = AEGfxTextureLoad("Assets/Pause.png");

	//
	// Loads a font and prepares it at a font height of 72 pixels.
	//
	// Note that it returns a s8, which is something like a "handle"
	// to the font managed by Alpha Engine.
	//
	s8 pFont = AEGfxCreateFont("Assets/liberation-mono.ttf", 72);

	// Initialize planets (LEGACY)
	/*
	Planet planets[3] = { 0 };
	planets[0].scale = 200.f;
	planets[0].spin_speed = 1.f;
	planets[0].orbit_speed = 0.f;
	planets[0].pos_x = planets[0].pos_y = 0.f;

	planets[1].parent = &planets[0];
	planets[1].scale = 100.f; // 
	planets[1].spin_speed = -1.f;
	planets[1].orbit_speed = -1.f;
	planets[1].pos_x = planets[1].pos_y = 200.f;

	planets[2].parent = &planets[1];
	planets[2].scale = 50.f;
	planets[2].spin_speed = 2.f;
	planets[2].orbit_speed = -2.f;
	planets[2].pos_x = planets[2].pos_y = 100.f;
	*/

	AEInputShowCursor(1);

	// Game Loop
	while (gGameRunning)
	{
		// Informing the system about the loop's start
		AESysFrameStart();

		// f32 dt = (f32)AEFrameRateControllerGetFrameTime();

		// Your own update logic goes here
		if (simulating)
		{
			for (s32 id = 0; id < CELL_NUM; ++id)
			{
				CELL_ARR[id].currentState = CELL_ARR[id].futureState;
			}
		}
		
		// INPUT READING
		if (AEInputCheckTriggered(AEVK_LBUTTON) && simulating == false)
		{
			s32 x, y;
			AEInputGetCursorPosition(&x, &y);

			s32 col = x / (s32)CELL_LEN;
			s32 row = y / (s32)CELL_LEN;

			s32 id = getCell(row, col);
			CELL_ARR[id].currentState = !CELL_ARR[id].currentState;
		}

		if (AEInputCheckTriggered(AEVK_SPACE))
		{
			simulating = !simulating;
		}

		if (simulating)
		{
			for (s32 id = 0; id < CELL_NUM; ++id)
			{
				s32 i = getNeighbours(id);

				if (CELL_ARR[id].currentState == true)
				{
					if (i == 2 || i == 3)
						CELL_ARR[id].futureState = true;
					else
						CELL_ARR[id].futureState = false;
				}
				else
				{
					if (i == 3)
						CELL_ARR[id].futureState = true;
					else
						CELL_ARR[id].futureState = false;
				}
			}
		}
		
		// After the previous line, x should contain the x-position of the cursor
		// while y will contain the y-position of the cursor.

		// CELL RENDER
		f32 xVal = -((f32)WIN_WIDTH / 2.f) + (CELL_LEN / 2.f);
		f32 yVal = ((f32)WIN_HEIGHT / 2.f) - (CELL_LEN / 2.f);
		bool generating = true;
		s32 id = 0;
		
		while (generating)
		{
			// Create a scale matrix that scales by 500 x and y
			AEMtx33 scale = { 0 };
			// AEMtx33Scale(&scale, 500.f, 500.f);
			AEMtx33Scale(&scale, CELL_LEN - 2.f, CELL_LEN - 2.f);

			// Create a rotation matrix that rotates by 90 degrees
			// Note that PI in radians is 180 degrees.
			// Since 90 degrees is 180/2, 90 degrees in radians is PI/2
			AEMtx33 rotate = { 0 };
			// AEMtx33Rot(&rotate, PI / 2);
			AEMtx33Rot(&rotate, 0.f);

			// Create a translation matrix that translates by
			// 200 in the x-axis and 100 in the y-axis
			AEMtx33 translate = { 0 };
			// AEMtx33Trans(&translate, 200.f, 100.f);
			AEMtx33Trans(&translate, xVal, yVal);

			// Concatenate the matrices into the 'transform' variable.
			// We concatenate in the order of translation * rotation * scale
			// i.e. this means we scale, then rotate, then translate.
			AEMtx33 transform = { 0 };
			AEMtx33Concat(&transform, &rotate, &scale);
			AEMtx33Concat(&transform, &translate, &transform);

			// Your own rendering logic goes here
			// Tell the Alpha Engine to set the background to black.
			AEGfxSetBackgroundColor(0.8f, 0.8f, 0.8f);

			// Tell the Alpha Engine to get ready to draw something with texture.
			// AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxSetRenderMode(AE_GFX_RM_COLOR);

			// Set the the color to multiply to white, so that the sprite can
			// display the full range of colors (default is black).
			AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

			// Set the color to add to nothing, so that we don't alter the sprite's color
			if (CELL_ARR[id].currentState == true)
				AEGfxSetColorToAdd(1.0f, 0.0f, 0.0f, 0.0f);
			else
				AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);

			// Set blend mode to AE_GFX_BM_BLEND, which will allow transparency.
			AEGfxSetBlendMode(AE_GFX_BM_BLEND);
			AEGfxSetTransparency(1.0f);

			// Tell Alpha Engine to use the texture stored in pTex
			// AEGfxTextureSet(pTex, 0, 0);

			// Actually drawing the mesh 
			AEGfxSetTransform(transform.m);
			AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

			xVal += CELL_LEN;
			if (xVal >= WIN_WIDTH / 2.f)
			{
				xVal = -((f32)WIN_WIDTH / 2.f) + (CELL_LEN / 2.f);
				yVal -= CELL_LEN;

				if (yVal <= -(WIN_HEIGHT / 2.f))
					generating = false;
			}
			++id;
			// std::cout << id;
		}

		// If not simulating, render pause texture
		if (!simulating)
		{
			AEMtx33 scale = { 0 };
			AEMtx33Scale(&scale, (f32)WIN_WIDTH, (f32)WIN_HEIGHT);
			AEMtx33 rotate = { 0 };
			AEMtx33Rot(&rotate, 0.f);
			AEMtx33 translate = { 0 };
			AEMtx33Trans(&translate, 0.f, 0.f);
			AEMtx33 transform = { 0 };
			AEMtx33Concat(&transform, &rotate, &scale);
			AEMtx33Concat(&transform, &translate, &transform);

			AEGfxSetBackgroundColor(0.8f, 0.8f, 0.8f);
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 0.7f);
			AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
			AEGfxSetBlendMode(AE_GFX_BM_BLEND);
			AEGfxSetTransparency(1.0f);
			AEGfxTextureSet(pTex, 0, 0);
			AEGfxSetTransform(transform.m);
			AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

			// And also render some text
			// Draws text that is neatly aligned to the center of the window.
			const char* pText = "PAUSED";
			f32 width;
			f32 height;
			AEGfxGetPrintSize(pFont, pText, 1.f, &width, &height);
			AEGfxPrint(
				pFont,			// font handle given by AEGfxCreateFont()
				pText, 			// null-terminated c-string to print
				-width / 2.f,	// x position on normalized coordinates, ranging from -1.f to 1.f
				-height / 2.f,	// y position in normalized coordinates, ranging from -1.f to 1.f 
				1.f, 			// how much to scale the text by
				1.f, 			// percentage of red, ranging from 0.f to 1.f
				0.f, 			// percentage of green, ranging from 0.f to 1.f
				1.f, 			// percentage of blue, ranging from 0.f to 1.f
				1.f				// percentage of alpha, ranging from 0.f to 1.f
			);
		}

		// PLANET RENDER CODE (LEGACY)
		/*
		for (int i = 0; i < (sizeof(planets) / sizeof(*planets)); ++i)
		{
			planets[i].spin += planets[i].spin_speed * dt;
			planets[i].orbit += planets[i].orbit_speed * dt;
		}

		// Draw the planets
		for (int i = 0; i < (sizeof(planets) / sizeof(*planets)); ++i)
		{
			// Calculate the final transform
			AEMtx33 transform;
			AEMtx33Identity(&transform);

			AEMtx33 orbit;
			AEMtx33Rot(&orbit, planets[i].orbit);

			AEMtx33 spin;
			AEMtx33Rot(&spin, planets[i].spin);

			AEMtx33 scale;
			AEMtx33Scale(&scale, planets[i].scale, planets[i].scale);

			AEMtx33 translate;
			AEMtx33Trans(&translate, planets[i].pos_x, planets[i].pos_y);

			// Concatenate everything
			AEMtx33Concat(&transform, &spin, &scale);
			AEMtx33Concat(&transform, &translate, &transform);
			AEMtx33Concat(&transform, &orbit, &transform);

			//
			// Calculate the transform of the parent that contains
			// things the child (this planet) cares about so that
			// we can move the child to where the parent is.
			// 
			// We care about two things from the parent:
			// - The parent's position
			// - The parent's orbit angle
			// Of course, if that parent has a parent (grandparent),
			// we have to use their position and orbit angle as well.
			//
			// NOTE: This is clearly not the best way to do this. 
			// It is slow, but at least it's robust. Making it more 
			// efficient is left as an exercise to the reader :)
			// 
			AEMtx33 parent_transform;
			AEMtx33Identity(&parent_transform);

			for (Planet* p = planets[i].parent; p != 0; p = p->parent)
			{
				AEMtx33 current_parent_orbit;
				AEMtx33Identity(&current_parent_orbit);
				AEMtx33Rot(&current_parent_orbit, p->orbit);

				AEMtx33 current_parent_pos;
				AEMtx33Trans(&current_parent_pos, p->pos_x, p->pos_y);

				// Translate THEN rotate
				AEMtx33 current_parent_transform;
				AEMtx33Concat(&current_parent_transform, &current_parent_orbit, &current_parent_pos);
				AEMtx33Concat(&parent_transform, &current_parent_transform, &parent_transform);
			}

			// Putting everything together
			AEMtx33Concat(&transform, &parent_transform, &transform);

			// Choose the transform to use
			AEGfxSetTransform(transform.m);

			// Actually drawing the mesh 
			AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
		}
		*/

		// Tell Alpha Engine to use the matrix in 'transform' to apply onto all
		// the vertices of the mesh that we are about to choose to draw in the next line.
		// AEGfxSetTransform(transform.m);

		// Tell Alpha Engine to draw the mesh with the above settings.
		// AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

		// Informing the system about the loop's end
		AESysFrameEnd();

		// check if forcing the application to quit
		if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
			gGameRunning = 0;
	}

	// This will free the Mesh.
	AEGfxMeshFree(pMesh);

	// 'Frees' the texture.
	AEGfxTextureUnload(pTex);

	// Releases resources used by a font
	AEGfxDestroyFont(pFont);

	// free the system
	AESysExit();
}