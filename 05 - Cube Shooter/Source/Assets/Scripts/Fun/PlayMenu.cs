using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class PlayMenu : MonoBehaviour
{
	public static PlayMenu instance;
	public TMPro.TMP_InputField seedInput;
	public Button[] levelButtons;
	public Unread history;

	private void Awake()
	{
		if (instance == null)
		{
			instance = this;
		}
		else
		{
			Debug.LogWarning("Multiple PlayMenu instance.");
			Destroy(gameObject);
			return;
		}
	}

	// Start is called before the first frame update
	private void Start()
    {
		// On click listeners
		for (int i = 0; i < levelButtons.Length; ++i)
		{
			int temp = i; // Dude trust me I have to do this
			levelButtons[i].onClick.AddListener(() => DataManager.instance.loadLevel(temp));
		}

		history.checkUnread();
	}

	public int getSeed()
	{
		int result;
		if (int.TryParse(seedInput.text, out result) == false)
		{
			result = Random.Range(0, 1000000000);
			Debug.Log("Generating random seed...");
		}
		return result;
	}

	private void OnDestroy()
	{
		for (int i = 0; i < levelButtons.Length; ++i)
		{
			int temp = i;
			levelButtons[i].onClick.RemoveListener(() => DataManager.instance.loadLevel(temp));
		}
	}
}
