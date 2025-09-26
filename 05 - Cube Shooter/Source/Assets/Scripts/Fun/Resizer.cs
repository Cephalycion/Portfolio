using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Resizer : MonoBehaviour
{
	public Camera cam;

	private void Awake()
	{
		RectTransform rt = GetComponent<RectTransform>();
		float cWidth = rt.rect.width;
		float cHeight = cWidth * Camera.main.pixelHeight / Camera.main.pixelWidth;
		rt.SetSizeWithCurrentAnchors(RectTransform.Axis.Vertical, cHeight);
		cam.orthographicSize = cHeight / 2;

		// GameObject[] bg = GameObject.FindGameObjectsWithTag("Background");
		   
		// for (int i = 0; i < bg.Length; ++i)
		// {
		// 	RectTransform bgrt = bg[i].GetComponent<RectTransform>();
		// 	bgrt.sizeDelta = new Vector2(bgrt.rect.width, cHeight);
		// 	Debug.Log("BG: " + i);
		// }
	}
}
