using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;
using System.Linq;
using DG.Tweening;

public class Builder : MonoBehaviour
{
	public static Builder instance = null;

	public GameObject top;					// Parent as well as start point of top bar
	public GameObject bottom;				// Parent as well as start point of bottom bar
	public GameObject buildFrom;			// BuildFrom prefab
	public GameObject buildTo;				// BuildTo prefab
	public GameObject follow;				// Image that follows cursor

	public BuildNav prev;
	public BuildNav next;

	[HideInInspector]
	public bool holdingChunk = false;
	[HideInInspector]
	public char letterCur;

	public float gap;								// Gap between BuildFrom objects
	private float width;							// Width of a BuildFrom object
	private string chunkList = "ABCDEFGHIJKLMN";	// List of all chunks
	private Vector2 startPos_Top;
	private Vector3 startPos_Bottom;
	private float range;
	private float tileUnit;
	private Texture2D chunkCur;
	public int buildID;
	private List<BuildTo> buildList = new List<BuildTo>();

	private void Awake()
	{
		if (instance == null)
		{
			instance = this;
		}
		else
		{
			Debug.LogWarning("Multiple Builder instance.");
			return;
		}
	}

	private void Start()
	{
		Physics2D.gravity = new Vector2(0.0f, 0.0f);
		string layout = DataManager.instance.GetLevelLayout();

		buildID = 0;
		startPos_Top = top.GetComponent<RectTransform>().anchoredPosition;
		float distance = 0.0f;
		bool first = true;

		// Generate BuildFroms
		foreach (char letter in chunkList)
		{
			GameObject from = Instantiate(buildFrom, top.transform);
			BuildFrom unitFrom = from.GetComponent<BuildFrom>();
			unitFrom.construct(letter);

			if (first == true)
			{
				first = false;
				width = from.GetComponent<RectTransform>().rect.width;
			}

			Vector2 pos = Vector3.zero;
			pos.x += distance;
			from.GetComponent<RectTransform>().anchoredPosition = pos;

			distance = distance + width + gap;
		}

		// Generate first BuildTo
		GameObject to = Instantiate(buildTo, bottom.transform);
		BuildTo unitTo = to.GetComponent<BuildTo>();
		unitTo.construct();
		buildList.Add(unitTo);
		checkChunk();
		tileUnit = unitTo.image.GetComponent<RectTransform>().rect.width / 32;
		startPos_Bottom = Vector2.zero;

		if (layout.Length != 0)
		{
			for (int i = 0; i < layout.Length; ++i)
			{
				char c = layout[i];

				if (c >= 65 && c <= 90)
				{
					letterCur = c;
					chunkCur = (Texture2D)Resources.Load("ChunkImg/" + c.ToString());
					buildList[buildID].setChunk();
					++buildID;
				}
			}
		}
		buildID = 0;

		// Calculate the range of slider
		float length = (startPos_Top.x * 2) + distance - width - gap;
		range = Mathf.Max(0.0f, length - DataManager.instance.s_width);
	}

	public void saveLevel()
	{
		string layout = "";

		for (int i = 0; i < buildList.Count; ++i)
		{
			BuildTo temp = buildList[i];
			char c = temp.letter;
			if (c >= 65 && c <= 90)
			{
				layout += temp.letter;
			}
			DataManager.instance.inventory.customMaps[(int)DataManager.instance.level - (int)LEVEL.SLOTA] = layout;
		}
		DataManager.instance.SaveInventory();
		DataManager.instance.loadCustomMaps();
	}

	public void Nav(int dir)
	{
		NavActive(prev, false);
		NavActive(next, false);

		buildID -= dir;
		RectTransform t = bottom.GetComponent<RectTransform>();
		t.DOLocalMoveX(t.anchoredPosition.x + (DataManager.instance.s_width * dir), 0.5f)
			.OnComplete(() => checkChunk());
	}

	public void removeChunk(BuildTo input)
	{
		NavActive(prev, false);
		NavActive(next, false);

		// If not building on the last chunk
		if (buildID != buildList.Count - 1)
		{
			// Play animation to delete
			input.transform.DOLocalMoveY(-DataManager.instance.s_height, 0.5f)
				.OnComplete(() => deleteChunk(input));
		}
	}

	private void deleteChunk(BuildTo input)
	{
		buildList.Remove(input);
		Destroy(input.gameObject);
		checkChunk();

		// Shift all buildTo after build 
		for (int i = buildID; i < buildList.Count; ++i)
		{
			RectTransform t = buildList[i].gameObject.GetComponent<RectTransform>();
			t.DOLocalMoveX(t.anchoredPosition.x - DataManager.instance.s_width, 0.5f);
		}
		saveLevel();
	}

	private void NavActive(BuildNav nav, bool active)
	{
		nav.button.interactable = active;
		Color color = nav.image.color;

		if (active)
		{
			color.a = 1.0f;
		}
		else
		{
			color.a = 0.5f;
		}
		nav.image.color = color;
	}

	public void checkChunk()
	{
		if (buildList.ElementAtOrDefault(buildID - 1) != null)
		{
			NavActive(prev, true);
		}
		else
		{
			NavActive(prev, false);
		}
		if (buildList.ElementAtOrDefault(buildID + 1) != null)
		{
			NavActive(next, true);
		}
		else
		{
			NavActive(next, false);
		}
	}

	public void moveSelection(float input)
	{
		Vector2 pos = startPos_Top;
		pos.x -= input * range;
		top.GetComponent<RectTransform>().anchoredPosition = pos;
	}

	public void getChunk(char letter)
	{
		follow.SetActive(true);
		follow.GetComponent<Image>().sprite = Resources.Load<Sprite>("ChunkImg/" + letter.ToString());
		chunkCur = (Texture2D)Resources.Load("ChunkImg/" + letter.ToString());
		letterCur = letter;
		holdingChunk = true;

		AudioManager.instance.play("BuildFrom");
	}

	private void LateUpdate()
	{
		// follow.GetComponent<RectTransform>().anchoredPosition = Input.mousePosition;
		Vector3 screenPoint = Input.mousePosition;
		screenPoint.z = 90.0f;
		follow.transform.position = Camera.main.ScreenToWorldPoint(screenPoint);

		if (Input.GetMouseButtonUp(0))
		{
			follow.SetActive(false);
			holdingChunk = false;
		}
	}

	public GameObject GenerateChunk(BuildTo input)
	{
		GameObject chunk = new GameObject("Chunk", typeof(RectTransform));
		chunk.transform.SetParent(input.transform);
		chunk.transform.SetSiblingIndex(1);
		chunk.transform.localPosition = Vector3.zero;
		chunk.transform.localScale = Vector3.one;

		Vector3 startPos = new Vector3(
			- (input.image.GetComponent<RectTransform>().rect.width / 2) + (tileUnit / 2),
			- (input.image.GetComponent<RectTransform>().rect.height / 2) + (tileUnit / 2),
			0);

		for (int x = 0; x < 32; ++x)
		{
			for (int y = 0; y < 16; ++y)
			{
				GenerateTile(x, y, chunk, startPos);
			}
		}

		if (buildID == buildList.Count - 1)
		{
			startPos_Bottom.x = buildList.Last().gameObject.GetComponent<RectTransform>().anchoredPosition.x + DataManager.instance.s_width;
			GameObject to = Instantiate(buildTo, bottom.transform);
			to.GetComponent<RectTransform>().anchoredPosition = startPos_Bottom;
			BuildTo unitTo = to.GetComponent<BuildTo>();
			unitTo.construct();
			buildList.Add(unitTo);
			checkChunk();
		}

		return chunk;
	}

	private void GenerateTile(int x, int y, GameObject chunk, Vector3 startPos)
	{
		Tile tile;
		Color pxColor = chunkCur.GetPixel(x, y);
		Vector3 pos = startPos + new Vector3(x * tileUnit, y * tileUnit, 0);

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

		GameObject obj = Instantiate(tile.prefab1, Vector3.zero, Quaternion.identity);

		// Clean unnecessary components
		Component[] components = obj.GetComponents(typeof(Component));
		for (int i = 0; i < components.Length; ++i)
		{
			Component c = components[i];
			if (!(c is Transform)		&& 
				!(c is Image)			&&
				!(c is RectTransform)	&&
				!(c is CanvasRenderer))
			{
				Destroy(c);
			}
		}

		obj.transform.SetParent(chunk.transform);
		obj.transform.localScale = new Vector3(tileUnit / 100, tileUnit / 100, 1);
		obj.GetComponent<RectTransform>().anchoredPosition = pos;
	}
}
