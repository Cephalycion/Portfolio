/*!
@copyright	DIGIPEN INSTITUTE OF TECHNOLOGY SINGAPORE
@file       Button.c
@author     Marc Alviz Evangelista(marcalviz.e),
@course     CSD1401 Software Engineering Project 1
@section    a
@tutorial   Final Project
@date       18/11/2023
@brief      This file contains the definitions of functions which pertains
			to buttons in game
*//*_________________________________________________________________________*/
#include "Button.h"

void ButtonDraw(Button btn, float deg)
{
	if (btn.btnHovering)
	{
		CP_Image_DrawAdvanced(btn.btnImageHover, btn.btnPos.x, btn.btnPos.y,
			(float)CP_Image_GetWidth(btn.btnImage) * btn.btnScale.x, (float)CP_Image_GetHeight(btn.btnImage) * btn.btnScale.y, // change this to adjust with scale
			btn.btnAlpha, deg);
	}
	else
	{
		CP_Image_DrawAdvanced(btn.btnImage, btn.btnPos.x, btn.btnPos.y,
			(float)CP_Image_GetWidth(btn.btnImage) * btn.btnScale.x, (float)CP_Image_GetHeight(btn.btnImage) * btn.btnScale.y, // change this to adjust with scale
			btn.btnAlpha, deg);
	}
}

void ButtonSetRes(Button* btn, int newRes, CP_Vector oldRes)
{
	// Update scale and pos here
	CP_Vector btnPos = CP_Vector_Set(btn->btnPos.x, btn->btnPos.y);
	CP_Vector btnFrac = CP_Vector_Set(btnPos.x / oldRes.x, btnPos.y / oldRes.y);
	switch (newRes)
	{
	case 1: // 720
		btn->btnScaleRes720 = CP_Vector_Set(1280.f / oldRes.x, 720.f / oldRes.y);
		btn->btnPosRes720 = CP_Vector_Set(btnFrac.x * 1280.f, btnFrac.y * 720.f);
		break;
	case 2: // 1080
		btn->btnScaleRes1080 = CP_Vector_Set(1920.f / oldRes.x, 1080.f / oldRes.y);
		btn->btnPosRes1080 = CP_Vector_Set(btnFrac.x * 1920.f, btnFrac.y * 1080.f);
		break;
	case 3: // fullscreen
		btn->btnScaleResFullscreen = CP_Vector_Set(CP_System_GetDisplayWidth() / oldRes.x, CP_System_GetDisplayHeight() / oldRes.y);
		btn->btnPosResFullscreen = CP_Vector_Set(btnFrac.x * (float)CP_System_GetDisplayWidth(), btnFrac.y * (float)CP_System_GetDisplayHeight());
		break;
	default:
		break;
	}
}

bool ButtonCheckHover(Button* btn, CP_Vector mouse)
{
	CP_Vector btnMaxPos;
	CP_Vector btnMinPos;

	btnMaxPos.x = btn->btnPos.x + (float)(CP_Image_GetWidth(btn->btnImage) * btn->btnScale.x) / 2.f;
	btnMaxPos.y = btn->btnPos.y + (float)(CP_Image_GetHeight(btn->btnImage) * btn->btnScale.y) / 2.f;

	btnMinPos.x = btn->btnPos.x - (float)(CP_Image_GetWidth(btn->btnImage) * btn->btnScale.x) / 2.f;
	btnMinPos.y = btn->btnPos.y - (float)(CP_Image_GetHeight(btn->btnImage) * btn->btnScale.y) / 2.f;
	// check if within confines of buttton image
	if (mouse.x >= btnMinPos.x && mouse.x <= btnMaxPos.x
		&& mouse.y >= btnMinPos.y && mouse.y <= btnMaxPos.y)
	{
		// width == numColumns, height == numRows
		int arraySize = CP_Image_GetWidth(btn->btnImage) * CP_Image_GetHeight(btn->btnImage);

		CP_Color* colorArray = malloc(arraySize * sizeof(CP_Color));

		CP_Image_GetPixelData(btn->btnImage, colorArray);

		// Find out which pixel is clicked on
		CP_Vector relativeClick;
		// shift to the top left corner
		relativeClick.x = mouse.x - (btn->btnPos.x - (float)(CP_Image_GetWidth(btn->btnImage)/ 2.f));
		relativeClick.y = mouse.y - (btn->btnPos.y - (float)(CP_Image_GetHeight(btn->btnImage) * btn->btnScale.y / 2.f));

		// check if the pixel clicked has alpha > 0
		if (colorArray != NULL)
		{
			int yIndex = (int)relativeClick.y * (int)(CP_Image_GetWidth(btn->btnImage));
			int xIndex = (int)relativeClick.x;

			int pixelIndex = yIndex + xIndex;

		if (colorArray[pixelIndex].a > 0)
			{
				free(colorArray);
				return true;
			}
			else
			{
				free(colorArray);
				return false;
			}
		}
	}
	else
	{
		return false;
	}
	return false;
}

bool ButtonCheckClick(Button* btn, CP_Vector click)
{
	CP_Vector btnMaxPos;
	CP_Vector btnMinPos;

	btnMaxPos.x = btn->btnPos.x + (float)(CP_Image_GetWidth(btn->btnImage) * btn->btnScale.x) / 2.f;
	btnMaxPos.y = btn->btnPos.y + (float)(CP_Image_GetHeight(btn->btnImage) * btn->btnScale.y) / 2.f;

	btnMinPos.x = btn->btnPos.x - (float)(CP_Image_GetWidth(btn->btnImage) * btn->btnScale.x) / 2.f;
	btnMinPos.y = btn->btnPos.y - (float)(CP_Image_GetHeight(btn->btnImage) * btn->btnScale.y) / 2.f;
	// check if within confines of buttton image
	if (click.x >= btnMinPos.x && click.x <= btnMaxPos.x
		&& click.y >= btnMinPos.y && click.y <= btnMaxPos.y)
	{
		// width == numColumns, height == numRows
		int arraySize = CP_Image_GetWidth(btn->btnImage) * CP_Image_GetHeight(btn->btnImage);

		CP_Color* colorArray = malloc(arraySize * sizeof(CP_Color));

		CP_Image_GetPixelData(btn->btnImage, colorArray);

		// Find out which pixel is clicked on
		CP_Vector relativeClick;
		// shift to the top left corner
		relativeClick.x = click.x - (btn->btnPos.x - (float)(CP_Image_GetWidth(btn->btnImage) / 2.f));
		relativeClick.y = click.y - (btn->btnPos.y - (float)(CP_Image_GetHeight(btn->btnImage) * btn->btnScale.y / 2.f));

		// check if the pixel clicked has alpha > 0
		if (colorArray != NULL)
		{
			int yIndex = (int)relativeClick.y * CP_Image_GetWidth(btn->btnImage);
			int xIndex = (int)relativeClick.x;

			int pixelIndex = yIndex + xIndex;

			if (colorArray[pixelIndex].a > 0)
			{
				free(colorArray);
				return true;
			}
			else
			{
				free(colorArray);
				return false;
			}
		}
	}
	else
	{
		return false;
	}

	// return accordingly
	return false;
}

void ButtonReleaseVars(Button* btn)
{
	CP_Image_Free(&btn->btnImage);
	if (btn->btnInteractable)
	{
		CP_Image_Free(&btn->btnImageHover);
	}
	if (btn->btnSoundEffect != NULL)
	{
		CP_Sound_Free(&btn->btnSoundEffect);
	}
}

void ButtonInit(Button* btn, CP_Vector pos, CP_Vector scale,
	int alpha, char* imageName, bool isInteractable, char* soundName, bool hasSound, char* fontName, bool hasFont)
{
	btn->btnAlpha = alpha;

	btn->btnPos = CP_Vector_Set(pos.x, pos.y);
	btn->btnScale = CP_Vector_Set(scale.x, scale.y);

	btn->btnHovering = false;
	btn->btnInteractable = isInteractable;

	char imageString[100];
	memset(imageString, '\0', sizeof(imageString));

	strcat_s(imageString, sizeof(imageString), "Assets/");
	strcat_s(imageString, sizeof(imageString), imageName);

	btn->btnImage = CP_Image_Load(imageString);

	if (isInteractable)
	{
		char imageHoverString[100];
		memset(imageHoverString, '\0', sizeof(imageHoverString));
	
		strcat_s(imageHoverString, sizeof(imageHoverString), "Assets/hover_");
		strcat_s(imageHoverString, sizeof(imageHoverString), imageName);
	
		btn->btnImageHover = CP_Image_Load(imageHoverString);
	}

	if (hasSound)
	{
		char soundString[100];
		memset(soundString, '\0', sizeof(soundString));

		strcat_s(soundString, sizeof(soundString), "Assets/");
		strcat_s(soundString, sizeof(soundString), soundName);
		btn->btnSoundEffect = CP_Sound_Load(soundString);
	}
	if (hasFont)
	{
		char fontString[100];
		memset(fontString, '\0', sizeof(fontString));

		strcat_s(fontString, sizeof(fontString), "Assets/");
		strcat_s(fontString, sizeof(fontString), fontName);
		btn->btnFont = CP_Font_Load(fontString);
	}

	//btn->btn_draw = DrawButton;
	//btn->btn_isClicked = CheckButtonClick;

}

