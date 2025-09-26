using UnityEngine;
using System.Collections.Generic;
using UnityEngine.UI;

public class CameraResolution : MonoBehaviour
{
	public List<CanvasScaler> canvasList;
	public int cameraSize;
	private int screenSizeX = 0;
	private int screenSizeY = 0;

	private void RescaleCamera()
	{
		if (Screen.width == screenSizeX && Screen.height == screenSizeY) return;

		float targetaspect = 16.0f / 9.0f;
		float windowaspect = (float)Screen.width / (float)Screen.height;
		float scaleheight = windowaspect / targetaspect;
		Camera camera = GetComponent<Camera>();

		if (scaleheight < 1.0f)
		{
			Rect rect = camera.rect;

			rect.width = 1.0f;
			rect.height = scaleheight;
			rect.x = 0;
			rect.y = (1.0f - scaleheight) / 2.0f;

			camera.rect = rect;
		}
		else // add pillarbox
		{
			float scalewidth = 1.0f / scaleheight;

			Rect rect = camera.rect;

			rect.width = scalewidth;
			rect.height = 1.0f;
			rect.x = (1.0f - scalewidth) / 2.0f;
			rect.y = 0;

			camera.rect = rect;
		}

		screenSizeX = Screen.width;
		screenSizeY = Screen.height;

		camera.orthographicSize = cameraSize;

		float woh = 0.0f;
		if (targetaspect < windowaspect)
		{
			woh = 1.0f;
		}
		for (int i = 0; i < canvasList.Count; ++i)
		{
			canvasList[i].matchWidthOrHeight = woh;
		}
	}

	void OnPreCull()
	{
		if (Application.isEditor) return;
		Rect wp = Camera.main.rect;
		Rect nr = new Rect(0, 0, 1, 1);

		Camera.main.rect = nr;
		GL.Clear(true, true, Color.black);

		Camera.main.rect = wp;
	}

	void Start()
	{
		RescaleCamera();
	}

	void Update()
	{
		RescaleCamera();
	}
}