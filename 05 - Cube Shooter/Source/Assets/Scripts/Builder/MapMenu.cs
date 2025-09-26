using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class MapMenu : MonoBehaviour
{
	public Slot[] slots;

	// Start is called before the first frame update
	private void Start()
	{
		loadLevelCheck();

		// On click listeners
		for (int i = 0; i < slots.Length; ++i)
		{
			int temp = (int)LEVEL.SLOTA + i;
			slots[i].play.onClick.AddListener(() => DataManager.instance.loadLevel(temp));
			slots[i].build.onClick.AddListener(() => loadBuilder(temp));
		}
	}

	private void loadBuilder(int level)
	{
		DataManager.instance.level = (LEVEL)level;
		SceneLoader.instance.LoadScene("Builder");
	}

	// For slot play buttons, we need to do a check if there is an existing layout
	// Make sure to do the check again every time we go back to this menu
	public void loadLevelCheck()
	{
		for (int i = 0; i < slots.Length; ++i)
		{
			int temp = (int)LEVEL.SLOTA + i;
			Button play = slots[i].play;
			Image playImg = slots[i].playImg;
			Color color = playImg.color;

			if (DataManager.instance.levels[temp].layout.Length == 0)
			{
				play.interactable = false;
				color.a = 0.5f;
				playImg.color = color;
			}
			else
			{
				play.interactable = true;
				color.a = 1.0f;
				playImg.color = color;
			}
		}
	}

	private void OnDestroy()
	{
		for (int i = 0; i < slots.Length; ++i)
		{
			int temp = (int)LEVEL.SLOTA + i;
			slots[i].play.onClick.RemoveListener(() => DataManager.instance.loadLevel(temp));
			slots[i].build.onClick.RemoveListener(() => loadBuilder(temp));
		}
	}
}
