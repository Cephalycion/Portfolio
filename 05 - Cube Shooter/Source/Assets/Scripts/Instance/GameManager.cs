using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class GameManager : MonoBehaviour
{
	public static GameManager instance = null;
	public static bool isPaused = false;

	public int chunksForCheckpoint = 5;
	public bool timing = false;

	public GameObject tutorial;
	public GameObject endMenu;
	public GameObject deathMenu;
	public GameObject pauseMenu;

	public TMPro.TextMeshProUGUI earned;
	public TMPro.TextMeshProUGUI entext;
	public TMPro.TextMeshProUGUI stars;

	public GameObject classic;
	public GameObject procedural;

	public TMPro.TextMeshProUGUI timer;
	public TMPro.TextMeshProUGUI kills;

	public TMPro.TextMeshProUGUI UI_Progress;
	public TMPro.TextMeshProUGUI UI_Last;
	public TMPro.TextMeshProUGUI UI_Total;

	public GameObject worldCanvas;
	public GameObject projectile;

	[HideInInspector]
	public List<GameObject> projectilePool = new List<GameObject>();

	private float elapsedTime = 0.0f;
	private float enemiesKilled = 0.0f;
	private int chunksPassed = 0;
	private int checkpointCounter = 0;
	private float lastEnemiesKilled = 0;

	private float totalKPM = 0.0f;
	private string select;

	private void Awake()
	{
		if (instance == null)
		{
			instance = this;
		}
		else
		{
			Debug.LogWarning("Multiple GameManager instance.");
			return;
		}

		switch (DataManager.instance.level)
		{
			case LEVEL.TUTORIAL:
				{
					tutorial.SetActive(true);
					break;
				}
			case LEVEL.INFINITY:
				{
					procedural.SetActive(true);
					break;
				}
			default:
				{
					break;
				}
		}
		classic.SetActive(true);
	}

	private void Start()
	{
		Physics2D.gravity = new Vector2(0.0f, -9.81f);
		endMenu.SetActive(false);
		deathMenu.SetActive(false);
		isPaused = false;
		UI_Progress.text = checkpointCounter.ToString() + "/" + chunksForCheckpoint.ToString();

		// BGM selection
		select = "";
		switch (DataManager.instance.level)
		{
			case LEVEL.TUTORIAL:
				{
					select = "1";
					break;
				}
			case LEVEL.LEVEL1  :
				{
					select = "2";
					break;
				}
			case LEVEL.LEVEL2  :
				{
					select = "3";
					break;
				}
			case LEVEL.LEVEL3  :
				{
					select = "4";
					break;
				}
			case LEVEL.LEVEL4  :
				{
					select = "5";
					break;
				}
			case LEVEL.LEVEL5  :
				{
					select = "6";
					break;
				}
			case LEVEL.INFINITY:
				{
					select = "7";
					break;
				}
			case LEVEL.SLOTA   :
			case LEVEL.SLOTB   :
			case LEVEL.SLOTC   :
			case LEVEL.SLOTD   :
			case LEVEL.SLOTE   :
				{
					select = UnityEngine.Random.Range(1, 9).ToString();
					break;
				}
		}
		AudioManager.instance.play(select, true);

		// Projectile pooling
		GameObject tmp;
		for (int i = 0; i < 200; ++i)
		{
			tmp = Instantiate(projectile, worldCanvas.transform);
			tmp.SetActive(false);
			projectilePool.Add(tmp);
		}
	}

	public GameObject getPooledObject()
	{
		for (int i = 0; i < projectilePool.Count; ++i)
		{
			if (!projectilePool[i].activeInHierarchy)
			{
				return projectilePool[i];
			}
		}
		return null;
	}

	private void Update()
	{
		if (timing)
		{
			elapsedTime += Time.deltaTime;
			TimeSpan ts = TimeSpan.FromSeconds(elapsedTime);
			timer.text = ts.ToString("mm\\:ss\\:ff");
		}
		else
		{
			return;
		}

		if (Input.GetKeyDown(KeyCode.Escape) && Player.instance.isAlive)
		{
			if (isPaused)
			{
				gameResume();
			}
			else
			{
				gamePause();
			}
		}
	}

	public void gamePause()
	{
		pauseMenu.SetActive(true);
		Time.timeScale = 0.0f;
		isPaused = true;
	}

	public void gameResume()
	{
		pauseMenu.SetActive(false);
		Time.timeScale = 1.0f;
		isPaused = false;
	}

	public void increaseKills()
	{
		++enemiesKilled;
		kills.text = enemiesKilled.ToString("0");
	}

	// Called when the player dies
	public void playerDied()
	{
		timing = false;

		switch (DataManager.instance.level)
		{
			case LEVEL.INFINITY:
				{
					playerEnd();
					break;
				}
			default:
				{
					AudioManager.instance.stopFade(select);
					deathMenu.SetActive(true);
					break;
				}
		}
	}

	// Called when the x position of the player exceeds the end of the loaded level
	public void playerEnd()
	{
		AudioManager.instance.stopFade(select);
		timing = false;
		endMenu.SetActive(true);

		if (DataManager.instance.level != LEVEL.INFINITY)
		{
			totalKPM = Mathf.Min(enemiesKilled, (enemiesKilled / elapsedTime) * DataManager.instance.GetLevelDuration());
		}
		endProtocol();
	}

	private void endProtocol()
	{
		Inventory inven = DataManager.instance.inventory;

		// Reward: KPM
		earned.text = ((int)totalKPM).ToString("0");
		inven.stars += (int)totalKPM;

		// Reward: Kills
		// earned.text = enemiesKilled.ToString("0");
		// entext.text = "KILLS IN " + timer.text + " MIN";
		// DataManager.instance.inventory.stars += (int)enemiesKilled;

		// Save inventory
		stars.text = DataManager.instance.inventory.stars.ToString();

		// Save match history
		for (int i = 0; i < 5; ++i)
		{
			if (inven.matchHistory[i][0] > -1.0f)
			{
				// If this is the last iteration and still not inserted
				if (i == 4)
				{
					// Move everything up by 1
					for (int j = 0; j < 4; ++j)
					{
						for (int k = 0; k < 18; ++k)
						{
							inven.matchHistory[j][k] = inven.matchHistory[j + 1][k];
						}
					}
					insertHistory(inven, i);
					break;
				}
			}
			else
			{
				// Insert if current slot is empty
				insertHistory(inven, i);
				break;
			}
		}

		inven.unread = true;
		DataManager.instance.inventory = inven;
		DataManager.instance.SaveInventory();
	}

	private void insertHistory(Inventory inven, int i)
	{
		inven.matchHistory[i][0] = (float)DataManager.instance.level;

		for (int j = 1; j < 11; ++j)
		{
			inven.matchHistory[i][j] = inven.getElement((UPGRADE)j);
		}

		inven.matchHistory[i][11] = enemiesKilled;
		inven.matchHistory[i][12] = elapsedTime;
		inven.matchHistory[i][13] = Mathf.Floor(totalKPM);

		Vector4 vec4 = Player.instance.savedColor;
		inven.matchHistory[i][14] = vec4.x;
		inven.matchHistory[i][15] = vec4.y;
		inven.matchHistory[i][16] = vec4.z;
		inven.matchHistory[i][17] = vec4.w;
	}

	public void playerPassedChunk()
	{
		++chunksPassed;
		++checkpointCounter;
		// UI_Progress.text = chunksPassed.ToString();
		UI_Progress.text = checkpointCounter.ToString() + "/" + chunksForCheckpoint.ToString();

		if (checkpointCounter == chunksForCheckpoint)
		{
			checkpointCounter = 0;

			float currEnemiesKilled = enemiesKilled - lastEnemiesKilled;
			lastEnemiesKilled = enemiesKilled;

			float KPM = Mathf.Min(currEnemiesKilled, (enemiesKilled / elapsedTime) * 60.0f);
			totalKPM += KPM;

			UI_Last.text = KPM.ToString("0");
			UI_Total.text = totalKPM.ToString("0");
		}
	}
}