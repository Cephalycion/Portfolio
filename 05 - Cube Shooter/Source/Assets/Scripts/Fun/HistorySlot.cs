using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using System;

public class HistorySlot : MonoBehaviour
{
	public TMPro.TextMeshProUGUI level = null;			// 0
	public List<TMPro.TextMeshProUGUI> cores = null;	// 01 02 03 04
	public List<GameObject> auxiliaries = null;			// 05 06 07
	public List<GameObject> weapons = null;				// 08 09 10
	public TMPro.TextMeshProUGUI kills = null;			// 11
	public TMPro.TextMeshProUGUI time = null;			// 12
	public TMPro.TextMeshProUGUI kpm = null;			// 13
	public Image[] images = null;                       // 14 15 16 17
	public Button button = null;

	private void Awake()
	{
		images = gameObject.GetComponentsInChildren<Image>();
		for (int i = 0; i < auxiliaries.Count; ++i)
		{
			auxiliaries[i].SetActive(false);
		}
		for (int i = 0; i < weapons.Count; ++i)
		{
			weapons[i].SetActive(false);
		}
	}

	public void parseData(float[] input)
	{
		if (input[0] < 0.0f)
		{
			Debug.Log("No match history.");
			return;
		}

		// level
		LEVEL lev = (LEVEL)(int)input[0];
		string levStr = "";
		switch (lev)
		{
			case LEVEL.TUTORIAL:
				{
					levStr = "T";
					break;
				}
			case LEVEL.INFINITY:
				{
					levStr = "\u221e";
					break;
				}
			case LEVEL.SLOTA:
			case LEVEL.SLOTB:
			case LEVEL.SLOTC:
			case LEVEL.SLOTD:
			case LEVEL.SLOTE:
				{
					levStr = char.ToString((char)(65 + (int)lev - (int)LEVEL.SLOTA));
					break;
				}
			default:
				{
					levStr = ((int)lev).ToString();
					break;
				}
		}
		level.text = levStr;

		// cores
		for (int i = 0; i < 4; ++i)
		{
			cores[i].text = ((int)input[i + 1]).ToString();
		}

		// auxiliaries
		for (int i = 0; i < 3; ++i)
		{
			if (input[i + 5] > 1.0f)
			{
				auxiliaries[i].SetActive(true);
			}
		}

		// weapons
		for (int i = 0; i < 3; ++i)
		{
			if (input[i + 8] > 1.0f)
			{
				weapons[i].SetActive(true);
				break;
			}
		}

		// kills
		kills.text = input[11].ToString("0");

		// time
		TimeSpan ts = TimeSpan.FromSeconds(input[12]);
		time.text = ts.ToString("mm\\:ss\\:ff");

		// kpm
		kpm.text = input[13].ToString("0");

		// assign onClick callback
		button.onClick.AddListener(() => { 
			PopupManager.instance.setLinker((int)input[13]);
			AudioManager.instance.play("Select");
		});

		// color
		Vector4 color = new Vector4(input[14], input[15], input[16], input[17]);
		level.color = color;
		for (int i = 0; i < cores.Count; ++i)
		{
			cores[i].color = color;
		}
		kills.color = color;
		time.color = color;
		kpm.color = color;
		for (int i = 0; i < images.Length; ++i)
		{
			images[i].color = color;
		}
	}
}
