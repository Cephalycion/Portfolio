using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class BuildFrom : MonoBehaviour
{
	public ButtonV2 button;
	public Image image;
	private char letter;

	public void construct(char input)
	{
		letter = input;
		Sprite sprite = Resources.Load<Sprite>("ChunkImg/" + input.ToString());
		image.sprite = sprite;
		button.onDown.AddListener(() => Builder.instance.getChunk(letter));
	}

	public void destruct()
	{
		button.onDown.RemoveListener(() => Builder.instance.getChunk(letter));
	}
}