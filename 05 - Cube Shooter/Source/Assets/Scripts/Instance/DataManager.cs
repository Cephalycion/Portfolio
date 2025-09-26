using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public enum UPGRADE
{
	NONE = 0,

	CORE_HEALTH,
	CORE_SHIELD,
	CORE_RECHARGE,
	CORE_SPEED,

	AUX_FUELVAMP,
	AUX_PIERCING,
	AUX_DEFLECT,

	WEP_PISTOL,
	WEP_RIFLE,
	WEP_SHOTGUN,

	CBE_GREEN,
	CBE_BLUE,
	CBE_RED,

	LENGTH,
};

public enum LEVEL
{
	TUTORIAL = 0,
	LEVEL1,
	LEVEL2,
	LEVEL3,
	LEVEL4,
	LEVEL5,
	INFINITY,
	SLOTA,
	SLOTB,
	SLOTC,
	SLOTD,
	SLOTE,
	LENGTH,
};

public enum POWERUP
{
	NONE = 0,
	INF_HEALTH,
	INF_AMMO,
	INF_FUEL,
	LENGTH,
};

public enum BULLET
{
	NORMAL = 0,
	MISSILE = 1,
	LENGTH,
}

public class DataManager : MonoBehaviour
{
	public static DataManager instance = null;

	// Default core values
	public float def_health		= 100.0f;
	public float def_shield		= 100.0f;
	public float def_recharge	= 10.0f;
	public float def_speed		= 300.0f;

	// Multiplier per point in inventory
	public float mul_health		= 20.0f;
	public float mul_shield		= 20.0f;
	public float mul_recharge	= 5.0f;
	public float mul_speed		= 50.0f;

	public GameObject shine;

	public Tile[] tileArray; // For inspector use only
	public Dictionary<Color, Tile> tileDictionary;
	public Level[] levels;
	public Inventory inventory;

	public LEVEL level = LEVEL.TUTORIAL;
	public int seed;

	public Color AMBER_COLOR;
	public Color ORANGE_COLOR;
	public Color RED_COLOR;
	public Gradient AMBER_GRADIENT;
	public Gradient ORANGE_GRADIENT;
	public Gradient RED_GRADIENT;

	public float s_width = 1920.0f;
	public float s_height = 1080.0f;

	public float volume;
	public bool bloom;
	public bool vignette;

	private void Awake()
	{
		if (instance == null)
		{
			instance = this;
			DontDestroyOnLoad(gameObject);
		}
		else
		{
			// Debug.LogWarning("Multiple DataManager instance.");
			Destroy(gameObject);
			return;
		}

		// Create a dictionary
		tileDictionary = new Dictionary<Color, Tile>();
		foreach (Tile t in tileArray)
		{
			tileDictionary.Add(t.color, t);
		}

		// Populate the alpha keys at relative time 0 and 1  (0 and 100%)
		GradientAlphaKey[] alphaKey = new GradientAlphaKey[2];
		alphaKey[0].alpha = 1.0f;
		alphaKey[0].time = 0.0f;
		alphaKey[1].alpha = 1.0f;
		alphaKey[1].time = 1.0f;

		// Amber colour one-time process
		ColorUtility.TryParseHtmlString("#FFC701", out AMBER_COLOR);
		AMBER_GRADIENT.SetKeys(getKey(AMBER_COLOR), alphaKey);

		// Orange colour one-time process
		ColorUtility.TryParseHtmlString("#FF8000", out ORANGE_COLOR);
		ORANGE_GRADIENT.SetKeys(getKey(ORANGE_COLOR), alphaKey);

		// Red colour one-time process
		ColorUtility.TryParseHtmlString("#FF0000", out RED_COLOR);
		RED_GRADIENT.SetKeys(getKey(RED_COLOR), alphaKey);

		initialise();
	}

	private void initialise()
	{
		// Load savefile
		inventory = SaveSystem.loadInventory();

		// Load settings
		if (PlayerPrefs.HasKey("volume") &&
			PlayerPrefs.HasKey("bloom") &&
			PlayerPrefs.HasKey("volume"))
		{
			volume = PlayerPrefs.GetFloat("volume");
			bloom = PlayerPrefs.GetInt("bloom") == 1 ? true : false;
			vignette = PlayerPrefs.GetInt("vignette") == 1 ? true : false;
		}
		else
		{
			Debug.Log("No player preferences. Generating default settings...");
			PlayerPrefs.SetFloat("volume", 1.0f);
			PlayerPrefs.SetInt("bloom", true ? 1 : 0);
			PlayerPrefs.SetInt("vignette", true ? 1 : 0);
			volume = 1.0f;
			bloom = true;
			vignette = true;
		}

		// Custom maps init
		loadCustomMaps();
	}

	private GradientColorKey[] getKey(Color color)
	{
		// Populate the color keys at the relative time 0 and 1 (0 and 100%)
		GradientColorKey[] colorKey = new GradientColorKey[2];
		colorKey[0].color = color;
		colorKey[0].time = 0.0f;
		colorKey[1].color = color;
		colorKey[1].time = 1.0f;

		return colorKey;
	}

	public void loadCustomMaps()
	{
		int j = 0;
		for (int i = (int)LEVEL.SLOTA; i <= (int)LEVEL.SLOTE; ++i)
		{
			levels[i].layout = inventory.customMaps[j];
			levels[i].duration = 20 * levels[i].layout.Length;
			++j;
		}
	}

	public void loadLevel(int i)
	{
		// Debug.Log("Trying to load level " + i);
		level = (LEVEL)i;

		switch ((LEVEL)i)
		{
			case LEVEL.INFINITY:
				{
					seed = PlayMenu.instance.getSeed();
					Debug.Log("Seed: " + seed);
					break;
				}
			default:
				{
					break;
				}
		}
		SceneLoader.instance.LoadScene("Game");
	}

	public string GetLevelLayout()
	{
		return levels[(int)level].layout;
	}

	public float GetLevelDuration()
	{
		return levels[(int)level].duration;
	}

	public void SaveInventory()
	{
		SaveSystem.saveInventory(inventory);
	}

	public PlayerData getPlayerData()
	{
		PlayerData data = new PlayerData();
		data.construct(
			def_health		+ inventory.getElement(UPGRADE.CORE_HEALTH)		* mul_health,
			def_shield		+ inventory.getElement(UPGRADE.CORE_SHIELD)		* mul_shield,
			def_recharge	+ inventory.getElement(UPGRADE.CORE_RECHARGE)	* mul_recharge,
			def_speed		+ inventory.getElement(UPGRADE.CORE_SPEED)		* mul_speed
			);
		return data;
	}

	private void Update()
	{
		if (Input.GetKey(KeyCode.LeftShift) || Input.GetKey(KeyCode.RightShift))
		{
			if (Input.GetKeyDown(KeyCode.Alpha1) && ShopManager.instance != null)
			{
				// Add 100 stars
				inventory.stars += 100;
				SaveInventory();
				ShopManager.instance.starAmount.text = inventory.stars.ToString();
			}
			if (Input.GetKeyDown(KeyCode.Alpha2))
			{
				// Reset savefile
				PlayerPrefs.DeleteAll();
				SaveSystem.resetInventory();
				initialise();
				SceneLoader.instance.LoadScene("MainMenu");
			}
		}
	}
}
