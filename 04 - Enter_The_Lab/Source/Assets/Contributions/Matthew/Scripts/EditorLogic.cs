using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;
using DG.Tweening;
using System;

public class EditorLogic : MonoBehaviour
{
	public GameObject CanvasReference;

	private bool Turn; // [ T = A / F = B ]
	private GameObject CameraReference;
	private GridBuilder GridReference;
	private Vector3 MouseDelta;
	private Vector3 MousePos;

    public AudioClip ChangeMeshSound;

	public bool buildMode = false;

	// Camera
	public Vector3 CameraOffset;
	public Vector3 CameraRotation;
	public float PanSpeedX;
	public float PanSpeedZ;
	public float FollowQuick;
	public float FollowSlow;
	private float FollowTimer;

	// Student
	public float StudentHeight;
	private List<Button> ButtonList = new List<Button>();
	public GameObject UICanvasReference;

    public GameObject SaveLevelPrompt;
    public GameObject EditGridPrompt;
    public GameObject LoadLevelPrompt;
    public GameObject LevelDataReference;

    public Dropdown BaseMeshDropdown;
    public Dropdown TopMeshDropdown;
    public Dropdown RotationDropdown;
    public Dropdown GridTypeDropdown;
    public Dropdown FloorMatDropdown;

    public InputField GridSizeInput;
    int GridSize;

    public InputField levelSaveName;

	#region Dev
	// Debug UI
	public GameObject UI_Turn;
	public GameObject UI_Action;
	public GameObject UI_Ability;
	public GameObject UI_Moves;
	public GameObject UI_Health;
	public GameObject ButtonReference;
	public float ButtonSpacing;
	#endregion

	// Grid Selection
	public float MouseUpdate;
	private float BounceTime;
	public LayerMask Layer;
	private GridNode CurrGrid;

    private Vector3 _panDistance;

    public static EditorLogic Instance = null;

    // Start is called before the first frame update
    void Awake()
    {
        Instance = this;
    }
    void Start()
    {
        // Init GridBuilder
        GridReference = GameObject.Find("GridBuilder").GetComponent<GridBuilder>();
        GridReference.Init();

        // Get Camera Reference
        CameraReference = GameObject.Find("Main Camera");
        CameraReference.transform.Rotate(CameraRotation);


        MouseUpdate = 0.0f;
        CameraReference.transform.position = CameraOffset;

        // foreach (string BaseMeshName in GridReference.TileMeshNames)
        BaseMeshDropdown.AddOptions(GridReference.TileMeshNames);
        TopMeshDropdown.AddOptions(GridReference.TopMeshNames);
        List<string> tempList = new List<string>();
        for (int i = (int)GridNode.TYPE.T_NONE; i < (int)GridNode.TYPE.T_TOTAL; ++i)
            tempList.Add(Enum.GetName(typeof(GridNode.TYPE), i));
        GridTypeDropdown.AddOptions(tempList);
        //List<Dropdown.OptionData> opList = new List<Dropdown.OptionData>();
        tempList = new List<string>();
        foreach (Material mat in GridReference.FloorMats)
        {
            // Dropdown.OptionData op = new Dropdown.OptionData();
            //op.image.texture = mat.mainTexture;
            tempList.Add(mat.name);
        }
        FloorMatDropdown.AddOptions(tempList);
        GridSize = GridReference.GridSize;
        GridSizeInput.text = GridReference.GridSize.ToString();
}


    // Update is called once per frame
    bool moving = false;
    void Update()
    {
        if (SaveLevelPrompt.activeInHierarchy)
            return;
        if (LoadLevelPrompt.activeInHierarchy)
            return;
        if (EditGridPrompt.activeInHierarchy)
            return;
        {
            if(Input.GetMouseButtonDown(0))
            {
                if (IsMouseOverUI())
                    return;
                _panDistance.Set(0, 0, 0);
                moving = true;
                MousePos = Input.mousePosition;
            }
            if (Input.GetMouseButton(0))
            {
                if (!moving)
                    return;
                MouseDelta = MousePos - Input.mousePosition;
                MousePos = Input.mousePosition;
                _panDistance += MouseDelta;

                // Change mouse delta from XY format to XZ format
                Vector3 Delta = new Vector3(MouseDelta.x * PanSpeedX, 0.0f, MouseDelta.y * PanSpeedZ);

                // Rotate by camera rotation
                Delta = Quaternion.Euler(CameraRotation) * Delta;

                // Assign
                CameraReference.transform.position += new Vector3(Delta.x, 0.0f, Delta.z) * Time.deltaTime;
            }
            if (Input.GetMouseButtonUp(0))
            {
                if (IsMouseOverUI()||!moving)
                    return;
                moving = false;
                    MousePos = Input.mousePosition;
                if (_panDistance.magnitude < 2.0f)
                {
                    Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
                    RaycastHit hit;
                    if (Physics.Raycast(ray, out hit, 9001.0f, Layer))
                    {
                        for (int i = 0; i < GridReference.GridList.Count; ++i)
                        {
                            GridReference.GridList[i].SetDefault();
                        }
                        if (hit.collider.gameObject.name == "GridObject(Clone)")
                        {
                            CurrGrid = hit.collider.gameObject.GetComponent<GridNode>();
                            CurrGrid.SetSelected();
                            int currGridTopRef = 0;
                            foreach (string tMesh in GridReference.TopMeshNames)
                            {
                                if (tMesh == CurrGrid.tileTName)
                                {
                                    break;
                                }
                                ++currGridTopRef;

                            }
                            TopMeshDropdown.value = currGridTopRef;

                            int currGridBaseRef = 0;
                            foreach (string bMesh in GridReference.TileMeshNames)
                            {
                                if (bMesh == CurrGrid.tileBName)
                                {
                                    break;
                                }
                                ++currGridBaseRef;

                            }
                            BaseMeshDropdown.value = currGridBaseRef;
                            RotationDropdown.value = (int)Mathf.Repeat(CurrGrid.rotation, 4);
                            GridTypeDropdown.value = (int)CurrGrid.TileType;

                            int currGridFloorRef = 0;
                            foreach (Material fMat in GridReference.FloorMats)
                            {
                                if (fMat.name == CurrGrid.floorName)
                                {
                                    break;
                                }
                                ++currGridFloorRef;

                            }
                            FloorMatDropdown.value = currGridFloorRef;
                        }
                    }
                }
            }
            if (Input.GetMouseButtonDown(1))
            {
                Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
                RaycastHit hit;
                if (Physics.Raycast(ray, out hit, 9001.0f, Layer))
                {
                    for (int i = 0; i < GridReference.GridList.Count; ++i)
                    {
                        GridReference.GridList[i].SetDefault();
                    }
                    if (hit.collider.gameObject.name == "GridObject(Clone)")
                    {
                        CurrGrid = hit.collider.gameObject.GetComponent<GridNode>();
                        CurrGrid.SetSelected();
                        UpdateMesh();
                    }

                }
            }
        }
        UpdateMesh();

        #region Matthew

        //if (Input.GetKeyDown(KeyCode.Q))
        //{
        //    ChangeCurrentGridMesh(true);
        //}
        //if (Input.GetKeyDown(KeyCode.W))
        //{
        //    ChangeCurrentGridMesh(false);
        //}
        //
        //if (Input.GetKeyDown(KeyCode.S))
        //{
        //    ChangeCurrentGridTop(true);
        //}
        //if (Input.GetKeyDown(KeyCode.A))
        //{
        //    ChangeCurrentGridTop(false);
        //}
        //
        //if (Input.GetKeyDown(KeyCode.R))
        //{
        //    RotateCurrentGrid();
        //}

        if(Input.GetKey(KeyCode.Q))
        {
            Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
            RaycastHit hit;
            if (Physics.Raycast(ray, out hit, 9001.0f, Layer))
            {
                if (hit.collider.gameObject.name == "GridObject(Clone)")
                {
                    GridNode _cGrid = hit.collider.gameObject.GetComponent<GridNode>();
                    _cGrid.SetFloor(CurrGrid.transform.Find("Floor").GetComponent<MeshRenderer>().material, CurrGrid.floorName);
                }
            }

        }

        #endregion

        #region Dev
        // Update Debug UI
        UI_Turn.GetComponent<Text>().text = "Build Mode";
        if (CurrGrid)
        {
            UI_Health.GetComponent<Text>().text = "Grid Obstacle: " + CurrGrid.IsObstacle.ToString();
            UI_Action.GetComponent<Text>().text = "Grid Base: " + CurrGrid.tileBName;
            UI_Ability.GetComponent<Text>().text = "Grid Top: " + CurrGrid.tileTName;
            UI_Moves.GetComponent<Text>().text = "Grid Rotation: " + CurrGrid.rotation.ToString();
        }
        else
        {
            UI_Action.GetComponent<Text>().text = "No Grid Selected";
            UI_Ability.GetComponent<Text>().text = "";

        }

        #endregion
    }

	private bool IsMouseOverUI()
	{
		PointerEventData pointerEventData = new PointerEventData(EventSystem.current);
		pointerEventData.position = Input.mousePosition;
		List<RaycastResult> raycastResults = new List<RaycastResult>();
		EventSystem.current.RaycastAll(pointerEventData, raycastResults);
		for (int i = 0; i < raycastResults.Count; ++i)
		{
			if (raycastResults[i].gameObject.GetComponent<UIRaycast>() != null)
			{
				raycastResults.RemoveAt(i);
				i--;
			}
		}
		return raycastResults.Count > 0;
	}

    public void RotateCurrentGrid()
    {
        CurrGrid.rotation++;
        if (CurrGrid.rotation == 4)
        {
            CurrGrid.rotation = 0;
        }
        BeardedManStudios.Forge.Logging.BMSLog.Log(CurrGrid.rotation.ToString());
    }

    public void ChangeCurrentGridMesh(bool next)
    {
        AudioManager.Instance.PlayAudio(ChangeMeshSound);
        if(CurrGrid == null)
            return;
        string meshBaseRef = (CurrGrid.tileBName);
        int meshBaseNum = 0;
        for (int i = 0; i < GridReference.TileMeshNames.Count; ++i)
        {
            if (meshBaseRef == GridReference.TileMeshNames[i])
            {
                meshBaseNum = i;
            }
        }

        string meshTopRef = (CurrGrid.tileTName);
        int meshTopNum = 0;
        for (int i = 0; i < GridReference.TopMeshNames.Count; ++i)
        {
            if (meshTopRef == GridReference.TopMeshNames[i])
            {
                meshTopNum = i;
            }
        }


        if (next)
        {
            ++meshBaseNum;
            if (meshBaseNum == GridReference.TileMeshNames.Count)
                meshBaseNum = 0;
        }
        else
        {
            --meshBaseNum;
            if (meshBaseNum < 0)
                meshBaseNum = GridReference.TileMeshNames.Count-1;
        }

        Material _baseMat = null;
        if (meshBaseNum < GridReference.TileMats.Count)
        {
            _baseMat = GridReference.TileMats[meshBaseNum];
        }
        Material _topMat = null;
        if (meshTopNum < GridReference.TopMats.Count)
        {
            _topMat = GridReference.TopMats[meshTopNum];
        }

        BeardedManStudios.Forge.Logging.BMSLog.Log(meshBaseNum.ToString());
        CurrGrid.SetMesh(
            GridReference.TileObjectMeshes[GridReference.TileMeshNames[meshBaseNum]],
            _baseMat,
            GridReference.TileMeshNames[meshBaseNum],
             GridReference.TopObjectMeshes[GridReference.TopMeshNames[meshTopNum]],
            _topMat,
            CurrGrid.tileTName);
        if (meshBaseNum < GridReference.IsObstacle.Count)
            CurrGrid.IsObstacle = GridReference.IsObstacle[meshBaseNum];
        else
            CurrGrid.IsObstacle = false;
    }

    public void ChangeCurrentGridTop(bool next)
    {
        if(CurrGrid == null)
            return;
        if(CurrGrid.tileBName == "none")
        {
            return;
        }
        if(GridReference == null)
        {
            BeardedManStudios.Forge.Logging.BMSLog.Log("GridRef is NULL!");
            Debug.Break();
        }

        string meshTopRef = (CurrGrid.tileTName);
        int meshTopNum = 0;
        for (int i = 0; i < GridReference.TopMeshNames.Count; ++i)
        {
            if (meshTopRef == GridReference.TopMeshNames[i])
            {
                meshTopNum = i;
            }
        }

        string meshBaseRef = (CurrGrid.tileBName);
        int meshBaseNum = 0;
        for (int i = 0; i < GridReference.TileMeshNames.Count; ++i)
        {
            if (meshBaseRef == GridReference.TileMeshNames[i])
            {
                meshBaseNum = i;
            }
        }

        if (next)
        {
            ++meshTopNum;
            if (meshTopNum == GridReference.TopMeshNames.Count)
                meshTopNum = 0;
        }
        else
        {
            --meshTopNum;
            if (meshTopNum < 0)
                meshTopNum = GridReference.TopMeshNames.Count-1;
        }

        Material _baseMat = null;
        if (meshBaseNum < GridReference.TileMats.Count)
        {
            _baseMat = GridReference.TileMats[meshBaseNum];
        }
        Material _topMat = null;
        if (meshTopNum < GridReference.TopMats.Count)
        {
            _topMat = GridReference.TopMats[meshTopNum];
        }

        BeardedManStudios.Forge.Logging.BMSLog.Log(meshTopNum.ToString());

        CurrGrid.SetMesh(
            GridReference.TileObjectMeshes[GridReference.TileMeshNames[meshBaseNum]],
            _baseMat,
            CurrGrid.tileBName,
             GridReference.TopObjectMeshes[GridReference.TopMeshNames[meshTopNum]],
            _topMat,
            GridReference.TopMeshNames[meshTopNum]
            );

    }

    public void UpdateMesh()
    {
        if (CurrGrid == null)
            return;
        int meshBaseNum = BaseMeshDropdown.value;

        string meshTopRef = (CurrGrid.tileTName);
        
        int meshTopNum = 0;
        if(BaseMeshDropdown.value == 0)
        {
            meshTopNum = 0;
        }
        else
        {
            meshTopNum = TopMeshDropdown.value;
        }

        Material _baseMat = null;
        if (meshBaseNum < GridReference.TileMats.Count)
        {
            _baseMat = GridReference.TileMats[meshBaseNum];
        }
        Material _topMat = null;
        if (meshTopNum < GridReference.TopMats.Count)
        {
            _topMat = GridReference.TopMats[meshTopNum];
        }

        BeardedManStudios.Forge.Logging.BMSLog.Log(meshBaseNum.ToString());
        CurrGrid.SetMesh(
            GridReference.TileObjectMeshes[GridReference.TileMeshNames[meshBaseNum]],
            _baseMat,
            GridReference.TileMeshNames[meshBaseNum],
             GridReference.TopObjectMeshes[GridReference.TopMeshNames[meshTopNum]],
            _topMat,
             GridReference.TopMeshNames[meshTopNum]);
        if (meshBaseNum < GridReference.IsObstacle.Count)
            CurrGrid.IsObstacle = GridReference.IsObstacle[meshBaseNum];
        else
            CurrGrid.IsObstacle = false;
        CurrGrid.rotation = (ushort)RotationDropdown.value;
        CurrGrid.TileType = (GridNode.TYPE)GridTypeDropdown.value;

        CurrGrid.SetFloor(GridReference.FloorMats[FloorMatDropdown.value], GridReference.FloorMats[FloorMatDropdown.value].name);
    }

    public void Save()
    {
        string fileName = "/Level_"+levelSaveName.text + ".json";
        GridReference.SaveLevel(fileName);
        UpdateLoadList();
    }

    void UpdateLoadList()
    {
        LoadLevelManager.Instance.UpdateLevels();
    }

    public void Load(string levelName)
    {
        SaveGridWrapper sgdwrpr = GridReference.ReadFromFile(levelName);
        if (sgdwrpr == null)
        {
            BeardedManStudios.Forge.Logging.BMSLog.LogWarning(levelName + " does not exist!!!");
            return;
        }
        else
            BeardedManStudios.Forge.Logging.BMSLog.Log(levelName + " found");
        foreach(GridNode grid in GridReference.GridList)
        {
            Destroy(grid.gameObject);
        }
        GridReference.GridList.Clear();
        GridReference.GenerateLevel(sgdwrpr.data);
        GridSizeInput.text = GridReference.GridSize.ToString();
        GridSize = GridReference.GridSize;
    }
public void UpdateGridSize()
{
    if(GridSize !=int.Parse(GridSizeInput.text))
        {
            GridSize = int.Parse(GridSizeInput.text);
            GridReference.ChangeGridSize(int.Parse(GridSizeInput.text));
        }
}
}
