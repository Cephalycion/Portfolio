using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

[SerializeField]
[System.Serializable]
public class GridNode : MonoBehaviour
{
	public GameObject Grid_E; // Excluded
    public GameObject Grid_D; // Default
    public GameObject Grid_S; // Selected
    public GameObject Text_ID;
    public GameObject Text_XZ;

    //public Material defaultFloorMaterial;

    public enum DIRECTION
    {
        D_UP = 0,
        D_RIGHT,
        D_DOWN,
        D_LEFT,
    }

	public enum TYPE
	{
		T_NONE = 0,
		T_SPAWN_A,
		T_SPAWN_B,
		T_DIPLOMA,

        T_TOTAL
	}

    public List<string> GetData()
    {
        List<string> tempList = new List<string>();

        tempList.Add(tileBName);
        tempList.Add(tileTName);
        tempList.Add(rotation.ToString());
        tempList.Add(IsObstacle.ToString());
        tempList.Add(TileType.ToString());
        tempList.Add(floorName);

        return tempList;
    }

    public string tileBName = "none", tileTName = "none";
    public string floorName = "none";
    // public Mesh tileMesh, tileTMesh;

    public ushort rotation; // 0,1,2,3

    public int GX;
    public int GZ;
    
    public Student student; // Each grid can hold 1 student

	// For BFS / A* pathing
	public bool m_visited;
	public bool m_obstacle;
	public GridNode m_previous;
	public TYPE m_type;

	//public bool Visited;
	//public bool IsObstacle;

    void Start()
    {
        m_visited = false;
        // m_obstacle = false;
        //if (defaultFloorMaterial != null)
        //    transform.Find("Floor").GetComponent<SpriteRenderer>().material = defaultFloorMaterial;
        SetExcluded();
    }

	public TYPE TileType
	{
		get { return m_type; }
		set { m_type = value; }
	}

	public bool Visited
	{
		get{ return m_visited; }
		set{ m_visited = value; }
	}
    
	public bool IsObstacle
	{
		get { return m_obstacle; }
		set { m_obstacle = value; }
	}

	public GridNode Previous
	{
		get { return m_previous; }
		set { m_previous = value; }
	}

	public void UpdatePos(float x, float z, int rotation)
	{
		gameObject.transform.position = new Vector3(x, 0.0f, z);
    }

    public void SetMesh(Mesh tileBase, Material matBase, string tileBaseName, Mesh tileTop = null, Material matTop = null, string tileTopName = "null")
    {
        GameObject _base = transform.Find("TileMesh").gameObject;
        MeshFilter _baseMF = _base.GetComponent<MeshFilter>();
        MeshRenderer _baseMR = _base.GetComponent<MeshRenderer>();
        if (tileBase != null)
        {
            _baseMF.mesh = tileBase;
            _baseMR.material = matBase;
           // _baseMF.
        
           // _base.GetComponent<BoxCollider>().size = new Vector3(2, 4, 2);
        }
        else
        {
            _baseMF.mesh = null;
            _baseMR.material = null;
           // gameObject.GetComponent<BoxCollider>().size = new Vector3(2, 0, 2);
        }
        tileBName = tileBaseName;
        GameObject _top = _base.transform.Find("TopMesh").gameObject;
        MeshFilter _topMF = _top.GetComponent<MeshFilter>();
        MeshRenderer _topMR = _top.GetComponent<MeshRenderer>();

        if (tileBase == null)
        {
            _topMF.mesh = null;
            _topMR.material = null;
            tileTName = "none";

            return;
        }

        if (tileTop != null)
        {
            _topMF.mesh = tileTop;
            _topMR.material = matTop;
        }
        else
        {
            _topMF.mesh = null;
            _topMR.material = null;
        }

        tileTName = tileTopName;
        if (tileBName.Contains("P_"))
        {
            GetComponentInChildren<ProjectileBlocker>().Type = ProjectileBlocker.BlockerType.BLOCK_PARTIAL;
        }
        else if (tileBName.Contains("W_"))
        {
            GetComponentInChildren<ProjectileBlocker>().Type = ProjectileBlocker.BlockerType.BLOCK_FULL;
        }
        else
        {
            GetComponentInChildren<ProjectileBlocker>().Type = ProjectileBlocker.BlockerType.BLOCK_NONE;
        }


    }

    public void SetFloor(Material matFloor, string floorName)
    {
        GameObject _floor = transform.Find("Floor").gameObject;
        MeshRenderer _floorMR = _floor.GetComponent<MeshRenderer>();
        if (matFloor != null)
        {
            _floorMR.material = matFloor;
        }
        else
        {
            _floorMR.material = null;
        }
        this.floorName = floorName;
    }

    //public void SetFloor(Material floor)
    //{
    //    if (floor != null)
    //        transform.Find("Floor").GetComponent<SpriteRenderer>().material = floor;
    //    else
    //        transform.Find("Floor").GetComponent<SpriteRenderer>().material = defaultFloorMaterial;
    //}

    public Vector3 GetPos()
	{
		return gameObject.transform.position;
	}

	public void SetExcluded()
	{
		//Grid_E.SetActive(true);
		Grid_D.SetActive(false);
		Grid_S.SetActive(false);
	}

	public void SetDefault()
	{
		Grid_E.SetActive(false);
		Grid_D.SetActive(true);
		Grid_S.SetActive(false);
	}

	public void SetSelected()
	{
		Grid_E.SetActive(false);
		Grid_D.SetActive(false);
		Grid_S.SetActive(true);
	}

	public void SetID(int ID)
	{
		Text_ID.GetComponent<Text>().text = "" + ID;
	}

	public void SetXZ(int x, int z)
	{
		Text_XZ.GetComponent<Text>().text = "" + x + ", " + z;
		GX = x;
		GZ = z;
	}

    private void Update()
    {
        GameObject tileMesh = transform.Find("TileMesh").gameObject;
        tileMesh.transform.rotation = Quaternion.Euler(Vector3.up * 90 * rotation);
        if (rotation >= 4)
            rotation = 0;
        switch (TileType)
        {
            case TYPE.T_NONE:
                Grid_D.GetComponent<SpriteRenderer>().color = new Color(0.5f, 0.5f, 0.5f);
                break;
            case TYPE.T_SPAWN_A:
                Grid_D.GetComponent<SpriteRenderer>().color = new Color(1, 0, 1);
                break;
            case TYPE.T_SPAWN_B:
                Grid_D.GetComponent<SpriteRenderer>().color = new Color(1, 1, 0);
                break;
            case TYPE.T_DIPLOMA:
                Grid_D.GetComponent<SpriteRenderer>().color = new Color(0, 1, 0);
                break;
        }
    }
}
