using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ChunkLoader : MonoBehaviour
{
	public static ChunkLoader instance = null;

	public GameObject canvas;
	public RectTransform leftWall;
	public float tileUnit = 100.0f;

	public int p_chunkWidth;
	public int p_chunkHeight;
	public int p_ceilingGap;
	public int p_platformChance;
	public int p_enemy;
	public int p_enemyIncrement;
	public int p_ratio;
	public int p_ratioIncrement;
	public int p_powerupChance;
	public Chance[] p_sectionWidths;
	public Chance[] p_stepHeights;
	public Chance[] p_platformWidths;

	private const string BLACK		= "#000000";
	private const string RED		= "#FF0000";
	private const string MAGENTA	= "#FF00FF";
	private const string YELLOW		= "#FFFF00";
	private const string CYAN		= "#00FFFF";
	private const string ORANGE		= "#FF8000";

	private Color black;
	private Color red;
	private Color magenta;
	private Color yellow;
	private Color cyan;
	private Color orange;
	private Color none;

	private Texture2D chunkCur;
	private int chunkID;
	private Vector3 startPos;
	private int highestChunk;
	private bool isProcedural;
	private System.Random rand;
	private int p_sectionHeight;
	private int p_sectionWidth;
	private int p_stepHeight;
	private Dictionary<int, GameObject> chunks;
	private float spawnTrigger;
	private bool spawnEnemy;

	private void Awake()
	{
		if (instance == null)
		{
			instance = this;
		}
		else
		{
			Debug.LogWarning("Multiple ChunkLoader instance.");
			return;
		}

		if (!ColorUtility.TryParseHtmlString(BLACK,		out black)		||
			!ColorUtility.TryParseHtmlString(RED,		out red)		||
			!ColorUtility.TryParseHtmlString(MAGENTA,	out magenta)	||
			!ColorUtility.TryParseHtmlString(YELLOW,	out yellow)		||
			!ColorUtility.TryParseHtmlString(CYAN,		out cyan)		||
			!ColorUtility.TryParseHtmlString(ORANGE,	out orange))
		{
			Debug.Log("Could not parse a HTML color to RGBA");
		}
		none = new Color(0, 0, 0, 0);
		p_sectionHeight = 4;
		chunks = new Dictionary<int, GameObject>();
		spawnTrigger = 0;
	}

	private void Start()
	{
		ResetLoader();

		switch (DataManager.instance.level)
		{
			case LEVEL.INFINITY:
				{
					GenerateLevelProcedurally();
					break;
				}
			default:
				{
					GenerateLevelFromImage(DataManager.instance.GetLevelLayout());
					break;
				}
		}
	}

	private void ResetLoader()
	{
		chunkCur = null;
		chunkID = 0;
		startPos = new Vector3(
			-DataManager.instance.s_width / 2 + tileUnit / 2, 
			-DataManager.instance.s_height / 2 + tileUnit / 2, 
			-1);
		// Debug.Log("startPos: " + startPos);
		isProcedural = false;
		rand = new System.Random(DataManager.instance.seed);
		p_sectionWidth = GetChanceValue(p_sectionWidths);
		p_stepHeight = GetChanceValue(p_stepHeights);
		spawnEnemy = false;
	}

	private void GenerateLevelFromImage(string name)
	{
		foreach (char letter in name)
		{
			if (letter >= 65 && letter <= 90)
			{
				chunkCur = (Texture2D)Resources.Load("ChunkImg/" + letter.ToString());
				GenerateChunk();
			}
		}

		Player.instance.finishGenerationProtocol(
			startPos.x - tileUnit / 2,
			highestChunk * tileUnit
			);
	}

	private void GenerateLevelProcedurally()
	{
		// Update values
		isProcedural = true;
		highestChunk = p_chunkHeight;

		// Generate 2 chunks first
		for (int i = 0; i < 2; ++i)
		{
			GenerateProceduralTexture();
		}

		Player.instance.finishGenerationProtocol(
			Mathf.Infinity,
			highestChunk * tileUnit
			);
	}

	// This function will dynamically create a Texture2D and assign it to chunkCur
	// This makes it almost identical to reading an image from directory
	// Allowing us to reuse GenerateChunk and GenerateTile as is
	private void GenerateProceduralTexture()
	{
		Texture2D texture = new Texture2D(p_chunkWidth, p_chunkHeight, TextureFormat.ARGB32, false);
		bool powerupSpawned = false;
		int sectionWidth = 0;
		int platformHeight = 0;
		int platformLength = 0;

		for (int x = 0; x < p_chunkWidth; ++x)
		{
			// Safe area in first chunk
			if (x > p_chunkWidth * 0.2f)
			{
				spawnEnemy = true;
			}
			if (sectionWidth > p_sectionWidth)
			{
				int direction;
				if (rand.NextDouble() < 0.5)
				{
					direction = -p_stepHeight;
				}
				else
				{
					direction = p_stepHeight;
				}
				p_sectionHeight = Mathf.Clamp(p_sectionHeight + direction, 1, p_chunkHeight - p_ceilingGap);
				p_sectionWidth = GetChanceValue(p_sectionWidths);
				p_stepHeight = GetChanceValue(p_stepHeights);
				sectionWidth = 0;
			}
			++sectionWidth;

			for (int y = 0; y < p_chunkHeight; ++y)
			{
				if (texture.GetPixel(x, y) == magenta	||
					texture.GetPixel(x, y) == yellow	||
					texture.GetPixel(x, y) == cyan)
				{
					continue;
				}

				texture.SetPixel(x, y, none);

				// Procedural logic here
				if (chunkID == 0)
				{
					// Spawn point
					if (x == 0 && y > 4)
					{
						texture.SetPixel(x, y, none);
						continue;
					}

					// Chunk container
					if (y == 0 || (y < 4 && (x == 0 || x == p_chunkWidth - 1)))
					{
						texture.SetPixel(x, y, black);
						continue;
					}
				}
				
				// Ground terrain
				if (y < p_sectionHeight)
				{
					texture.SetPixel(x, y, black);
					continue;
				}

				// Enemies
				if (spawnEnemy && texture.GetPixel(x, y - 1) == black)
				{
					int decision = rand.Next(0, 100);
					if (decision < p_enemy)
					{
						int ratio = rand.Next(0, 100);
						if (ratio < p_ratio)
						{
							texture.SetPixel(x, y, orange);
						}
						else
						{
							texture.SetPixel(x, y, red);
						}
						continue;
					}
				}

				// Air platforms
				if (platformLength > 0)
				{
					if (y == platformHeight)
					{
						// Chance for powerup
						if (powerupSpawned == false)
						{
							int decision = rand.Next(0, 100);
							if (decision < p_powerupChance)
							{
								powerupSpawned = true;
								int choice = rand.Next(0, 3);

								switch (choice)
								{
									case 0:
										{
											texture.SetPixel(x, y + 1, magenta);
											break;
										}
									case 1:
										{
											texture.SetPixel(x, y + 1, yellow);
											break;
										}
									case 2:
										{
											texture.SetPixel(x, y + 1, cyan);
											break;
										}
								}
							}
						}
						texture.SetPixel(x, y, black);
						--platformLength;
						continue;
					}
				}
				else
				{
					// If at least 3 empty spaces below
					if (checkSpacesDown(x, y, texture) > 2 && y < p_chunkHeight - 2)
					{
						// Chance for platform
						int decision = rand.Next(0, 100);
						if (decision < p_platformChance)
						{
							// If there is enough empty space to start building
							if (checkArea(x, y, texture, 2))
							{
								platformHeight = y;
								platformLength = GetChanceValue(p_platformWidths);
								continue;
							}
						}
					}
				}
			}
		}
		texture.Apply();
		chunkCur = texture;
		GenerateChunk();
	}

	// Returns true if all checked spaces are empty
	private bool checkArea(int x, int y, Texture2D t, int a)
	{
		for (int i = x - a; i <= x; ++i)
		{
			for (int j = y - a; j <= y; ++j)
			{
				// If black
				if (t.GetPixel(i, j).a != 0)
				{
					return false;
				}
			}
		}
		return true;
	}

	/*
	private bool isOutOfBounds(int x, int y, Texture2D t)
	{
		if (x < 0 || y < 0 || x > t.width || y > t.height)
		{
			return true;
		}
		return false;
	}
	*/

	// Returns the number of empty spaces below this coordinate
	private int checkSpacesDown(int x, int y, Texture2D t)
	{
		int v = y;
		for (int i = 1; v - i > 0; ++i)
		{
			if (t.GetPixel(x, v - i).a != 0)
			{
				return i - 1;
			}
		}
		return 32;
	}

	private int GetChanceValue(Chance[] chances)
	{
		int total = 0;

		// If no chances, return
		if (chances == null || chances.Length == 0)
		{
			Debug.Log("No chance.");
			return -1;
		}

		// Iterate through all weights and add to total
		for (int i = 0; i < chances.Length; ++i)
		{
			if (chances[i].weight >= 0)
			{
				total += chances[i].weight;
			}
		}
		
		// Get decision based on total
		float decision = rand.Next(0, total);
		float chance = 0.0f;

		// Iterate through chances
		for (int i = 0; i < chances.Length; ++i)
		{
			// Continue if no weight
			if (chances[i].weight <= 0.0f)
			{
				continue;
			}

			// Add weight to current chance
			chance += chances[i].weight;

			// If hit, return
			if (decision <= chance)
			{
				return chances[i].value;
			}
		}

		Debug.Log("No chance.");
		return -1;
	}

	private void Update()
	{
		if (isProcedural == false)
		{
			return;
		}

		// Track player position
		if (Player.instance.isAlive)
		{
			Vector3 pos = Player.instance.position;

			// If the player has entered the last chunk
			if (pos.x > spawnTrigger)
			{
				// Delete the chunk 3 chunks to the left
				int idx = chunkID - 3;
				GameObject temp = null;
				chunks.TryGetValue(idx, out temp);
				if (temp != null)
				{
					chunks.Remove(idx);
					Destroy(temp);
				}

				// Set left wall position
				leftWall.position = new Vector2(startPos.x - tileUnit / 2 - (p_chunkWidth * tileUnit * 2), 0.0f);
				Player.instance.updateCamMinBoundsX(leftWall.position.x);

				// Generate a new chunk
				GenerateProceduralTexture();

				// Tell GameManager
				GameManager.instance.playerPassedChunk();

				p_enemy = Mathf.Min(p_enemy + p_enemyIncrement, 100);
				p_ratio = Mathf.Min(p_ratio + p_ratioIncrement, 100);
			}
		}
	}

	private void GenerateChunk()
	{
		if (chunkCur.height > highestChunk)
		{
			highestChunk = chunkCur.height;
		}
		if (chunkCur == null)
		{
			return;
		}
		GameObject chunk = new GameObject();
		chunk.name = "Chunk" + chunkID;
		chunk.transform.SetParent(canvas.transform);
		chunk.transform.localPosition = Vector3.zero;
		chunk.transform.localScale = Vector3.one;
		chunk.transform.SetSiblingIndex(1);
		chunks.Add(chunkID, chunk);

		for (int x = 0; x < chunkCur.width; ++x)
		{
			for (int y = 0; y < chunkCur.height; ++y)
			{
				GenerateTile(x, y, chunk);
			}
		}
		++chunkID;
		spawnTrigger = startPos.x;
		startPos += new Vector3(chunkCur.width * tileUnit, 0, 0);
		chunkCur = null;
	}

	private void GenerateTile(int x, int y, GameObject chunk)
	{
		Tile tile;
		GameObject prefab = null;
		Color pxColor = chunkCur.GetPixel(x, y);
		Vector3 pos = startPos + new Vector3(x * tileUnit, y * tileUnit, 0);

		// Set player position
		if (x == 0 && y == 4 && chunkID == 0)
		{
			Player.instance.position = pos;
			// Debug.Log("Player: " + pos);
		}

		// If pivel is transparent, ignore
		if (pxColor.a == 0)
		{
			return;
		}

		// Get corresponding tile
		if (DataManager.instance.tileDictionary.TryGetValue(pxColor, out tile) == false)
		{
			Debug.Log("Colour " + pxColor + " does not correspond to a prefab.");
			return;
		}

		// Spawning logic
		switch ("#" + ColorUtility.ToHtmlStringRGB(pxColor))
		{
			case BLACK:
				{
					// Check 4 sides
					if ((chunkCur.GetPixel(x + 1, y) != pxColor) ||
						(chunkCur.GetPixel(x - 1, y) != pxColor) ||
						(chunkCur.GetPixel(x, y + 1) != pxColor) ||
						(chunkCur.GetPixel(x, y - 1) != pxColor))
					{
						prefab = tile.prefab2;
					}
					else
					{
						prefab = tile.prefab1;
					}
					break;
				}
			default:
				{
					prefab = tile.prefab1;
					break;
				}
		}

		if (prefab != null)
		{
			GameObject obj = Instantiate(prefab, Vector3.zero, Quaternion.identity);
			obj.transform.SetParent(chunk.transform);
			obj.transform.localScale = Vector3.one;
			obj.transform.localPosition = pos;
		}
	}
}
