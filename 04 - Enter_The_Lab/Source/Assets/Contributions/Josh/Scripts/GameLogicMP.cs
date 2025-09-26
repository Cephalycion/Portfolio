using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;
using DG.Tweening;
using BeardedManStudios.Forge.Networking.Generated;
using BeardedManStudios.Forge.Networking;
using BeardedManStudios.Forge.Networking.Unity;
using UnityEngine.SceneManagement;
using System;
using TMPro;

/// <summary>
/// GameLogicMP
///     This is a fork of the Original GameLogic done by Ryan to allow for Multiplayer.
/// </summary>
public class GameLogicMP : GameLogicBehavior
{
	[Header("CANVAS")]
	private GameObject CanvasReference;
    public GameObject UICanvasReference;

	[Header("BUTTONS")]
    public List<GameObject> ButtonReferance = new List<GameObject>();
    public GameObject ButtonCancel;

	[Header("STUDENT")]
	public List<GameObject> StudentReference = new List<GameObject>();
    public GameObject StudentStats;
    public float StudentHeight;
	public float TurnTimeMax;

    [Header("CAMERA")]
    public Vector3 CameraOffset;
    public Vector3 CameraRotation;
    public float PanSpeedX;
    public float PanSpeedZ;
    public float CamFast;
    public float CamSlow;
    private float CamTimer;
	private GridNode CameraGrid;
    public Button CamLeft;
    public Button CamRight;

    [Header("PROJECTILE")]
    public GameObject Projectile;
    public float ProjectileSpeed;
    public int AttackRange;
    private List<GameObject> ProjectileList = new List<GameObject>();

	[Header("DEV UI")]
    private GameObject UI_Turn;
    private GameObject UI_Action;
    private GameObject UI_Ability;
    private GameObject UI_Moves;
    private GameObject UI_Health;
    private GameObject UI_TEAM_A;
    private GameObject UI_TEAM_B;
    public GameObject UIDebugBecauseRyan;
    public float ButtonSpacing;

	[Header("GRID SELECTION")]
	private GridBuilder GridReference;
    public float MouseUpdate;
    private float BounceTime;
    public LayerMask Layer;
    private GridNode CurrGrid;
    private List<GridNode> SelectionList = new List<GridNode>();

	[Header("WIN CONDITION")]
	public int DiplomaRange = 0;
    public int WinThreshold = 10;
    public int WinnerEarnings = 6;
    public int LoserEarnings = 4;
    public GameObject WinDisplay;
	public GameObject DiplomaObject;
	List<List<GridNode>> DiplomaAreas = new List<List<GridNode>>();

	[HideInInspector]
    public static GameLogicMP instance = null;
    private List<Student> StudentListA = new List<Student>(); // Team A
    private List<Student> StudentListB = new List<Student>(); // Team B
    private List<GridNode> SpawnPointA = new List<GridNode>();
    private List<GridNode> SpawnPointB = new List<GridNode>();
	private int PointsA = 0;
	private int PointsB = 0;
    private Vector3 totalPan;
	private bool Turn; // [ T = A / F = B ]
    private bool Action;
	[HideInInspector]
	public Student Selected;
	private int Selected_Ability;
    private int Selected_ID;
	private float TurnTimeCurr;
    private GameObject CameraReference;
    private bool LoadFlag;
    private Queue<Data.TPoint> CamQueue = new Queue<Data.TPoint>();
    private bool IsRunning = false;
	public TopBar topbar;
    [Header("Wait Canvas")]
    public GameObject WaitRoot;
    public TextMeshProUGUI WittyTitle;
    public TextMeshProUGUI WittyText;
    public TextMeshProUGUI IPAddress;

	// Start is called before the first frame update
	void Start()
    {
		TurnTimeCurr = TurnTimeMax;

        // Bind some referances. if your moving Canvas Names, please fix them
        foreach (var gaemObject in SceneManager.GetActiveScene().GetRootGameObjects())
        {
            if (gaemObject.name == "GameCanvas")
            {
                CanvasReference = gaemObject;
            }
            GameLogicMP logic = gaemObject.GetComponent<GameLogicMP>();
            if (logic != null)
            {
                if (MultiPrefab.Instance.serverSide == MPSide.MP_SERVERALONE)
                {
                    if (!logic.networkObject.IsOwner)
                    {
                        name = "Client on Server";
                        gameObject.SetActive(false);
                    }
                    else
                        instance = this;
                }
                else
                    instance = this;
            }
        }
        CameraReference = GameObject.Find("Main Camera");
        Canvas thonkery = transform.Find("UICanvas").GetComponent<Canvas>();
        thonkery.worldCamera = CameraReference.GetComponent<Camera>();
        thonkery.planeDistance = 1;
        UI_Turn = UIDebugBecauseRyan.transform.Find("Turn").gameObject;
        UI_Action = UIDebugBecauseRyan.transform.Find("Action").gameObject;
        UI_Ability = UIDebugBecauseRyan.transform.Find("Ability").gameObject;
        UI_Moves = UIDebugBecauseRyan.transform.Find("Moves").gameObject;
        UI_Health = UIDebugBecauseRyan.transform.Find("Health").gameObject;
        UI_TEAM_A = UIDebugBecauseRyan.transform.Find("TEAM_A Points").gameObject;
        UI_TEAM_B = UIDebugBecauseRyan.transform.Find("TEAM_B Points").gameObject;
    }

    bool commonStarted = false;

    public void StartCommon()
    {
        if (commonStarted)
        {
            Debug.LogError("Common already started");
            return;
        }
        commonStarted = true;
        LoadFlag = false;
        CamTimer = 0.0f;
        BounceTime = MouseUpdate;

        // Init GridBuilder
        GridReference = GameObject.Find("GridBuilder").GetComponent<GridBuilder>();
        GridReference.Init();
        GridReference.GenerateLevel(SceneConstant.Map);

        // Iterate through the grids and set the spawnpoints
        for (int i = 0; i < GridReference.GridList.Count; ++i)
        {
            switch (GridReference.GridList[i].TileType)
            {
                case GridNode.TYPE.T_SPAWN_A:
                    {
                        SpawnPointA.Add(GridReference.GridList[i]);
                        break;
                    }
                case GridNode.TYPE.T_SPAWN_B:
                    {
                        SpawnPointB.Add(GridReference.GridList[i]);
                        break;
                    }
            }
        }
        // Set Diploma areas
        for (int i = 0; i < GridReference.GridList.Count; ++i)
        {
            if (GridReference.GridList[i].tileTName == "Diploma")
            {
                List<int> IndexList = GetGridsInRange(GridReference.GridList[i], DiplomaRange, true);
				List<GridNode> NewList = new List<GridNode>();
                for (int j = 0; j < IndexList.Count; ++j)
                {
                    GridReference.GridList[IndexList[j]].TileType = GridNode.TYPE.T_DIPLOMA;
					NewList.Add(GridReference.GridList[IndexList[j]]);
                }
				GameObject NewObject = Instantiate(DiplomaObject);
				DiplomaHolder NewHolder = NewObject.GetComponent<DiplomaHolder>();
				NewHolder.Init(NewList, CanvasReference);
				DiplomaAreas.Add(NewList);
			}
        }

        LoadFlag = false;
        CamTimer = 0.0f;
        BounceTime = MouseUpdate;
        IsRunning = true;
        PointsA = 0;
        PointsB = 0;

        // Get Camera Reference
        
        CameraReference.transform.Rotate(CameraRotation);
    }

    void StartServer()
    {
		// SpawnStudent t parameter legend
		// 0 = Afiq
		// 1 = Spy
		// 2 = Shinon
		// 3 = Mbrine

		// Server inits all the Spawn students while the client only spawns the grids.
        if (SceneConstant.PlaySelection.Count > 0)
        {
            
            for (int i = 0; i < 4; ++i)
            {
                int x = SpawnPointA[i].GX;
                int z = SpawnPointA[i].GZ;
                StudentListA.Add(SpawnStudent(x, z, SceneConstant.PlaySelection[i].itemIndex, 0));
            }
        }
        else
        {
            for (int i = 0; i < 4; ++i)
            {
                int x = SpawnPointA[i].GX;
                int z = SpawnPointA[i].GZ;
                StudentListA.Add(SpawnStudent(x, z, i, 0));
            }
        }
		
        //for (int i = 0; i < 4; ++i)
        //{
        //    int x = SpawnPointB[i].GX;
        //    int z = SpawnPointB[i].GZ;
        //    StudentListB.Add(SpawnStudent(x, z, i, 1));
        //}
        Turn = false;
        EndTurn();
        #region WaitScreen
        WaitRoot.GetComponent<Canvas>().worldCamera = CameraReference.GetComponent<Camera>();
        WaitRoot.GetComponent<Canvas>().planeDistance = 0.5f;
        WittyTitle = WaitRoot.transform.Find("Panel/Title").GetComponent<TextMeshProUGUI>();
        WittyText = WaitRoot.transform.Find("Panel/Witty").GetComponent<TextMeshProUGUI>();
        IPAddress = WaitRoot.transform.Find("Panel/IPAddress").GetComponent<TextMeshProUGUI>();
        IPAddress.text = "Your IP Address is: " + IPManager.GetIP(ADDRESSFAM.IPv4);
        string[] WittyList = {
            "Contemplating the meaning of life...",
            "Akarin!",
            "TOSHINO KYOUKO!!!",
            "Thinking for more Witty text...",
            "Now loading, Now loading, Now loading!",
            "Play the world!",
            "Genzai! Banzai!",
            "Dressing Alice in blue...",
            "Reticulating splines...",
            "Generating witty dialog...",
            "Swapping time and space...",
            "Tokenizing real life...",
            "Bending the spoon...",
            "Filtering morale...",
            "Don't think of purple hippos...",
            "We need a new fuse...",
            "Have a good day.",
            "Life is Short – Talk Fast!!!!",
            "Optimism – is a lack of information.....",
        };
        WittyText.text = WittyList[UnityEngine.Random.Range(0, WittyList.Length)];
        #endregion
    }

    bool once = false;
    bool recvfirstplayers = false;

    // Update is called once per frame
    void Update()
    {
		#region Multiplayer prechecks
		if (MultiPrefab.Instance.serverSide == MPSide.MP_NONE)
            return;
        else
        {
            if (networkObject == null && networkObject.NetworkReady)
                return;
            if (!networkObject.IsOwner && MultiPrefab.Instance.serverSide == MPSide.MP_SERVERALONE)
            {
                // it is the other client.
                return;
            }
            if (!once)
            {
                if (MultiPrefab.Instance.serverSide == MPSide.MP_SERVERALONE && networkObject.IsServer)
                {
                    StartCommon();
                    StartServer();
                }
                else
                {
                    ServerConnectedStuff();
                    Debug.Log("Ready, Sent inital Students. Waiting to recieve...");
                }
                once = true;
                return;
            }
        }
        if (!recvfirstplayers && MultiPrefab.Instance.serverSide == MPSide.MP_CLIENTA)
            return;
        if (!clientjoined && MultiPrefab.Instance.serverSide == MPSide.MP_SERVERALONE)
            return;
		#endregion

		if (IsRunning == true)
        {
            #region Check win condition
            if (StudentListA.Count <= 0 || PointsB >= WinThreshold)
            {
                WinCondition(false);
            }
            else if (StudentListB.Count <= 0 || PointsA >= WinThreshold)
            {
                WinCondition(true);
            }
            #endregion

            #region Turn switching
            if (Turn == true && MultiPrefab.Instance.serverSide == MPSide.MP_SERVERALONE)
            {
                DoTurn(StudentListA);
            }
            else if (Turn == false && MultiPrefab.Instance.serverSide == MPSide.MP_CLIENTA)
            {
                DoTurn(StudentListB);
            }
            #endregion

            #region Camera queue system
            if (CamTimer >= 0.0f)
            {
                CamTimer -= Time.deltaTime;
            }
            else
            {
                if (CamQueue.Count > 0)
                {
                    Data.TPoint temp = CamQueue.Dequeue();
                    Vector3 offset = temp.grid.GetPos() + CameraOffset;

                    // Used for normal cam queue
                    if (temp.duration >= 0.0f)
                    {
                        while ((offset - CameraReference.transform.position).magnitude < 0.01f && CamQueue.Count > 0)
                        {
                            temp = CamQueue.Dequeue();
                        }
                    }

					CameraGrid = temp.grid;
                    CameraReference.transform.DOMove(
                        offset,
                        temp.duration);
                    CamTimer = temp.duration + temp.time;
                }
            }
            if (CamTimer <= 0.0f && LoadFlag == true)
            {
                LoadSelected();
                LoadFlag = false;
            }
            #endregion

            #region Update Projectile List
            for (int i = 0; i < ProjectileList.Count; ++i)
            {
                if (ProjectileList[i] == null)
                {
                    ProjectileList.RemoveAt(i);
                    i--;
                }
            }
            #endregion

            #region Update Debug UI
            if (Turn == true)
            {
                UI_Turn.GetComponent<Text>().text = "Turn A" + Selected_ID;
            }
            else
            {
                UI_Turn.GetComponent<Text>().text = "Turn B" + Selected_ID;
            }
            if (Action == true)
            {
                UI_Action.GetComponent<Text>().text = "Action : True";
                UI_Ability.GetComponent<Text>().text = "Ability : " + Selected.Abilities[Selected_Ability];
            }
            else
            {
                UI_Action.GetComponent<Text>().text = "Action : False";
                UI_Ability.GetComponent<Text>().text = "Ability : None";
            }
            UI_Moves.GetComponent<Text>().text = "Moves : " + Selected.currMoves;
            UI_Health.GetComponent<Text>().text = "Health : " + Selected.currHealth;
            UI_TEAM_A.GetComponent<Text>().text = "TEAM_A : " + PointsA;
            UI_TEAM_B.GetComponent<Text>().text = "TEAM_B : " + PointsB;
			#endregion

			#region Camera Rotation (Pending GUI)
			if (Input.GetKeyDown("q"))
			{
				RotateCamera(90.0f);
			}
			if (Input.GetKeyDown("e"))
			{
				RotateCamera(-90.0f);
			}
			#endregion

			#region Turn Timer
			if (TurnTimeCurr > 0.0f)
			{
				TurnTimeCurr -= Time.deltaTime;
			}
			else
			{
				EndTurn(true);
			}
			topbar.UpdateTimer(TurnTimeCurr);
			#endregion
		}
	}

    private List<studserialkiller> Inv2Game(List<Inventory_Item> inventory)
    {
        List<studserialkiller> studserialkillers = new List<studserialkiller>();
        foreach (var student in inventory)
        {
            studserialkiller s = new studserialkiller
            {
                t = student.itemIndex
            };
        }
        return studserialkillers;
    }

    private void ServerConnectedStuff()
    {
        if (SceneConstant.PlaySelection.Count == 0)
        {
            List<studserialkiller> studserialkillers = new List<studserialkiller>();
            for (int i = 0; i < 4; i++)
            {
                studserialkiller s = new studserialkiller
                {
                    t = i
                };
                studserialkillers.Add(s);
            }
            networkObject.SendRpc(RPC_CLIENT_STUDENT_PRE, Receivers.Others, studserialkillers.ObjectToByteArray(),"Player 2");
        }
        else
        {
            networkObject.SendRpc(RPC_CLIENT_STUDENT_PRE, Receivers.Others, Inv2Game(SceneConstant.PlaySelection).ObjectToByteArray(),SceneConstant.username);
        }

        #region WaitScreen
        WaitRoot.GetComponent<Canvas>().worldCamera = CameraReference.GetComponent<Camera>();
        WaitRoot.GetComponent<Canvas>().planeDistance = 0.5f;
        WittyTitle = WaitRoot.transform.Find("Panel/Title").GetComponent<TextMeshProUGUI>();
        WittyText = WaitRoot.transform.Find("Panel/Witty").GetComponent<TextMeshProUGUI>();
        IPAddress = WaitRoot.transform.Find("Panel/IPAddress").GetComponent<TextMeshProUGUI>();
        IPAddress.text = "Your IP Address is: " + IPManager.GetIP(ADDRESSFAM.IPv4);
        string[] WittyList = {
            "Contemplating the meaning of life...",
            "Akarin!",
            "TOSHINO KYOUKO!!!",
            "Thinking for more Witty text...",
            "Now loading, Now loading, Now loading!",
            "Play the world!",
            "Genzai! Banzai!",
            "Dressing Alice in blue...",
            "Reticulating splines...",
            "Generating witty dialog...",
            "Swapping time and space...",
            "Tokenizing real life...",
            "Bending the spoon...",
            "Filtering morale...",
            "Don't think of purple hippos...",
            "We need a new fuse...",
            "Have a good day.",
            "Life is Short – Talk Fast!!!!",
            "Optimism – is a lack of information.....",
        };
        WittyText.text = WittyList[UnityEngine.Random.Range(0, WittyList.Length)];
        #endregion
    }

    void RotateCamera(float angle)
	{
		CameraOffset = Quaternion.Euler(0.0f, angle, 0.0f) * CameraOffset;
		CameraRotation.y += angle;
		CameraRotation.y = Wrap(CameraRotation.y, 0.0f, 360.0f);
		Vector3 offset = CameraGrid.GetPos() + CameraOffset;
		CameraReference.transform.DOMove(offset, 0.5f);
		CameraReference.transform.DORotate(CameraRotation, 0.5f);
	}

	public float Wrap(float value, float min, float max)
	{
		float ret = value;
		if (value < min)
		{
			float diff = Mathf.Abs(min - value);
			ret = max - diff;
		}
		if (value > max)
		{
			float diff = Mathf.Abs(value - max);
			ret = min + diff;
		}
		return ret;
	}

	void WinCondition(bool team)
    {
        BeardedManStudios.Forge.Logging.BMSLog.Log("WIN_CONDITION");
        IsRunning = false;
        Clean();

        string winner;
        int earningsA;
        int earningsB;

        if (team == true)
        {
            winner = "TEAM_A";
            earningsA = WinnerEarnings;
            earningsB = LoserEarnings;
            // Add currency to account A
        }
        else
        {
            winner = "TEAM_B";
            earningsA = LoserEarnings;
            earningsB = WinnerEarnings;
            // Add currency to account B
        }

        // Instantiate WinDisplay
        GameObject NewObject = Instantiate(WinDisplay, UICanvasReference.transform);
        WinManager NewManager = NewObject.GetComponent<WinManager>();
        NewManager.SetWinner(winner);
        NewManager.SetData(true, "TEAM_A", earningsA);
        NewManager.SetData(false, "TEAM_B", earningsB);
        if (MultiPrefab.Instance.serverSide == MPSide.MP_CLIENTA)
        {
            SceneConstant.username = SceneConstant.usernameclient;
            PlayFabClient.GetInstance().AddTuna(earningsB, null, null);
            MultiPrefab.Instance.Stop();
        }
        else if (MultiPrefab.Instance.serverSide == MPSide.MP_SERVERALONE)
        {
            PlayFabClient.GetInstance().AddTuna(earningsA, null, null);
            MultiPrefab.Instance.Stop();
        }
    }

    public void StudentDeath(Student target)
    {
        AudioManager.Instance.PlayAudio(target.deathSound, 0.5f, false);

        if (target == Selected)
        {
            Selected_ID--;
        }
        Destroy(target.gameObject, 1.0f);

        if (StudentListA.Contains(target))
        {
            StudentListA.Remove(target);
        }
        else
        {
            StudentListB.Remove(target);
        }
    }

    Vector3 GetWorldPosition(float z)
    {
        Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
        Plane ground = new Plane(Vector3.up, new Vector3(0, 0, z));
        ground.Raycast(ray, out float distance);
        return ray.GetPoint(distance);
    }

    private Vector3 TouchStart = new Vector3(0,0,0);

    void DoTurn(List<Student>Students)
    {
		if (Input.GetMouseButtonDown(0))
        {
            TouchStart = GetWorldPosition(0);
            totalPan.Set(0, 0, 0);
        }
        if (Input.GetMouseButton(0))
        {
            Vector3 dir = TouchStart - GetWorldPosition(0);
            dir = new Vector3(dir.x, 0.0f, dir.z);
            Camera.main.transform.position += dir;
            totalPan += dir;
        }

        if (ProjectileList.Count == 0)
        {
            if (BounceTime > 0.0f)
            {
                BounceTime -= Time.deltaTime;
            }
            else
            {
                // Tracking mouse
                BounceTime = MouseUpdate;

                // Set selection grids to default state
                for (int i = 0; i < SelectionList.Count; ++i)
                {
                    SelectionList[i].SetDefault();
                }

				GridRaycast();

				// Extra raycast for mobile devices
				if (Input.GetMouseButtonDown(0))
				{
					GridRaycast();
				}
			}

            if (Input.GetMouseButtonUp(0) && CurrGrid != null && !IsMouseOverUI()&& totalPan.magnitude< 2.0f)
            {
                switch (Selected.Abilities[Selected_Ability])
                {
                    case Student.Ability.MOVE:
                        {
                            // If the selected grid has no student
                            if (CurrGrid.student == null)
                            {
                                StudentMove();
                                --Selected.currMoves;
                                EndAction();
                            }
                            break;
                        }
                    case Student.Ability.ATTACK:
                        {
                            Student ToAttack = CurrGrid.student;
                            if (ToAttack != null)
                            {
                                // Check if selected student is from ally team
                                bool TeamKill = false;
                                if (Students.Contains(ToAttack))
                                {
                                    TeamKill = true;
                                }
                                // If the selected grid has a student to attack
                                if (TeamKill == false)
                                {
                                    StudentAttack(Selected, ToAttack);
                                    Selected.currMoves = 0;
                                    EndAction();
                                }
                            }
                            break;
                        }
                }
            }
        }

        #region Next Student
        if ((Selected.currMoves <= 0 || Selected == null) && ProjectileList.Count == 0)
        {
            ++Selected_ID;
            if (Selected_ID < Students.Count)
            {
                Selected = Students[Selected_ID];
                LoadFlag = true;
                Clean();
                EndAction();
                int Index = GridReference.Idx(Selected.SX, Selected.SZ);
                CameraMove(GridReference.GridList[Index], CamFast, 0.0f);
            }
            else
            {
                EndTurn(true);
            }
        }
        #endregion
    }

	void GridRaycast()
	{
		Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);

		// If Raycast hit gameObject
		if (Physics.Raycast(ray, out RaycastHit hit, 9001.0f, Layer))
		{
			if (hit.collider.gameObject.name == "GridObject(Clone)")
			{
				CurrGrid = hit.collider.gameObject.GetComponent<GridNode>();
				if (SelectionList.Contains(CurrGrid))
				{
					CurrGrid.SetSelected();
				}
				else
				{
					CurrGrid = null;
				}
			}
		}
		else
		{
			CurrGrid = null;
		}
	}

    List<int> GetGridsInRange(GridNode grid, int range, bool clipping)
    {
        // Initialise variables
        List<int> ListReturn = new List<int>();
        Queue<GridNode> GridQueue = new Queue<GridNode>();
        Queue<GridNode> StepQueue = new Queue<GridNode>();
        GridQueue.Enqueue(grid);

        // Make sure visited is set to false
        for (int i = 0; i < GridReference.GridList.Count; ++i)
        {
            GridReference.GridList[i].Visited = false;
        }
        // BFS Limit
        int counter = 0;
        while (GridQueue.Count > 0 && counter < range)
        {
            GridNode curr = GridQueue.Dequeue();

            #region Directions
            // UP
            if (IsWithinBounds(curr.GZ + 1))
            {
                int next = GridReference.Idx(curr.GX, curr.GZ + 1);
                BFSHelper(next, StepQueue, ListReturn, clipping);
            }
            // DOWN
            if (IsWithinBounds(curr.GZ - 1))
            {
                int next = GridReference.Idx(curr.GX, curr.GZ - 1);
                BFSHelper(next, StepQueue, ListReturn, clipping);
            }
            // LEFT
            if (IsWithinBounds(curr.GX - 1))
            {
                int next = GridReference.Idx(curr.GX - 1, curr.GZ);
                BFSHelper(next, StepQueue, ListReturn, clipping);
            }
            // RIGHT
            if (IsWithinBounds(curr.GX + 1))
            {
                int next = GridReference.Idx(curr.GX + 1, curr.GZ);
                BFSHelper(next, StepQueue, ListReturn, clipping);
            }
            #endregion

            if (GridQueue.Count == 0)
            {
                ++counter;
                while (StepQueue.Count > 0)
                {
                    GridQueue.Enqueue(StepQueue.Dequeue());
                }
            }
        }
        return ListReturn;
    }

    void BFSHelper(int next, Queue<GridNode> GridQueue, List<int> ListReturn, bool clipping)
    {
        GridNode temp = GridReference.GridList[next];
        if (temp.Visited == false)
        {
            if (clipping == true)
            {
                GridQueue.Enqueue(temp);
                GridReference.GridList[next].Visited = true;
                ListReturn.Add(next);
            }
            else
            {
                if (temp.student == null && temp.IsObstacle == false)
                {
                    GridQueue.Enqueue(temp);
                    GridReference.GridList[next].Visited = true;
                    ListReturn.Add(next);
                }
            }
        }
    }

    bool IsWithinBounds(int a)
    {
        if (a >= 0 && a < GridReference.GridSize)
        {
            return true;
        }
        return false;
    }

    bool IsMouseOverUI()
    {
        PointerEventData pointerEventData = new PointerEventData(EventSystem.current)
        {
            position = Input.mousePosition
        };
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

    void EndAction()
    {
        SetGridExcluded();
        Action = false;
        Selected_Ability = 0;
    }

    void CameraMove(GridNode grid, float duration, float wait)
    {
        CamQueue.Enqueue(new Data.TPoint(grid, duration, wait));
    }

    void StudentMove()
    {
        // So we just need to send current and target postion.
        // Remove student from the grid
        networkObject.SendRpc(RPC_STUDENT_MOVE, Receivers.Others, Selected.uuid, CurrGrid.GX, CurrGrid.GZ);
        StudentMoveInternals(Selected, Selected.SX, Selected.SZ, CurrGrid.GX, CurrGrid.GZ);
    }

    void StudentMoveInternals(Student student, int CX, int CZ, int TX,int TZ, bool rpced = false)
    {
        int Index = GridReference.Idx(CX, CZ); // position.
        GridNode Previous = GridReference.GridList[Index];
        Previous.student = null;
        GridReference.GridList[Index] = Previous;
        GridNode changedNode = GridReference.GridList[GridReference.Idx(TX, TZ)];

        student.Waypoints = GetPath(Previous, changedNode, 0.2f, 0.5f);
        changedNode.student = student;

        // Update XZ indexes
        student.UpdateSXZ(TX,TZ); // target?

        // Move Camera
        if (!rpced)
            CameraMove(changedNode, CamFast, 0.5f);


        // Set selection grid back
        SetGridExcluded();
    }

    public void OverwatchCheck(GridNode Grid, Student student)
    {
        if (MultiPrefab.Instance.serverSide == MPSide.MP_SERVERALONE)
        {
            if (StudentListA.Contains(student))
            {
                OverwatchCheck(Grid, StudentListA);
            }
            else
            {
                OverwatchCheck(Grid, StudentListB);
            }
        }
    }

    void OverwatchCheck(GridNode Grid, List<Student>Students)
    {
        List<int> range = GetGridsInRange(Grid, AttackRange, true);
        foreach (int i in range)
        {
            // If there is a student on the grid
            if (GridReference.GridList[i].student != null)
            {
                // If the student is not from the current team
                if (Students.Contains(GridReference.GridList[i].student) == false)
                {
                    Student temp = GridReference.GridList[i].student;
                    if (temp.sm.GetCurrentState() == "Overwatch")
                    {
                        StudentAttack(temp, Selected);
                        temp.sm.SetNextState("Normal");
                    }
                }
            }
        }
    }
	
	/*
    RPC for attack:
    We send over the particle only for rpc.
    If it hits on the server/client side
	*/
    public Dictionary<string, GameObject> projectileMapper = new Dictionary<string, GameObject>();

    void StudentAttack(Student From, Student To, bool isRPC = false, string otheruuid = "")
    {
        string uuid = Guid.NewGuid().ToString();
        if (!isRPC)
        {
            BeardedManStudios.Forge.Logging.BMSLog.Log("Send Projectile Data to Others.");
            networkObject.SendRpc(RPC_STUDENT_PROJ_SPAWN, Receivers.Others, new studserialkiller(From).ObjectToByteArray(), new studserialkiller(To).ObjectToByteArray(), uuid);
        }
        // Get team
        List<Student> Allies;
        if (StudentListA.Contains(From))
        {
            Allies = StudentListA;
        }
        else
        {
            Allies = StudentListB;
        }

        // Get direction
        Vector3 selectPos = From.gameObject.transform.position;
        Vector3 dir = (To.gameObject.transform.position - selectPos).normalized;

        AudioManager.Instance.PlayAudio(From.fireSound, 0.5f, false);

        // Instantiate Projectile
        GameObject NewObject = Instantiate(Projectile, CanvasReference.transform);

        if (otheruuid != "")
            uuid = otheruuid;
        NewObject.name = uuid;
        projectileMapper[uuid] = NewObject;
        // we just dont add a projectile manager if it is a rpc call
        if (!isRPC)
        {
            ProjectileList.Add(NewObject);

            NewObject.GetComponent<ProjectileManager>().Init(
                From.damage,
                selectPos,
                new Vector3(dir.x * ProjectileSpeed, 0.0f, dir.z * ProjectileSpeed),
                Allies);
        }
        else
        {
            NewObject.GetComponent<ProjectileManager>().enabled = false;
        }

        // Move Camera
        int Index = GridReference.Idx(To.SX, To.SZ);
        GridNode ToGrid = GridReference.GridList[Index];
        CameraMove(ToGrid, CamFast, 0.0f);
    }

    void Clean()
    {
        // Reset Values
        CurrGrid = null;
        SetGridExcluded();

    }

    void LoadSelected()
    {

        foreach (var button in ButtonReferance)
        {
            Button buttonScript = button.GetComponent<Button>();
            buttonScript.onClick.RemoveAllListeners();
            button.SetActive(false);
        }
        ButtonCancel.SetActive(false);
        if ((Turn == true && MultiPrefab.Instance.serverSide == MPSide.MP_SERVERALONE) || 
            (Turn == false && MultiPrefab.Instance.serverSide == MPSide.MP_CLIENTA))
        {
            int ctr = 0;
            // Instantiate ability buttons
            for (int i = 0; i < Selected.Abilities.Count; ++i)
            {
                int idx2 = 0;

                switch (Selected.Abilities[i])
                {
                    case Student.Ability.MOVE:
                        idx2 = 0;
                        break;
                    case Student.Ability.ATTACK:
                        idx2 = 1;
                        break;
                    case Student.Ability.OVERWATCH:
                        idx2 = 2;
                        break;
                    case Student.Ability.HUNKER:
                        idx2 = 3;
                        break;
                    case Student.Ability.PROCRASTINATE:
                        idx2 = 4;
                        break;
                }
                Button NewButton = ButtonReferance[idx2].GetComponent<Button>();
                ButtonReferance[idx2].SetActive(true);
                // ButtonReferance[idx].SetActive(true);
                // Debug.Log("Btn: " + idx + "|" + item);
                int _i = i;
                NewButton.onClick.AddListener(delegate { SelectAbility(_i); });
            }
            foreach (var item in Selected.Abilities)
            {
                ;
            }
            ButtonCancel.SetActive(true);
            Button cancelBtn = ButtonCancel.GetComponent<Button>();
            cancelBtn.onClick.RemoveAllListeners();
            cancelBtn.onClick.AddListener(delegate { EndAction(); });

            CamLeft.onClick.RemoveAllListeners();
            CamRight.onClick.RemoveAllListeners();
            CamLeft.onClick.AddListener(delegate { RotateCamera(90.0f); });
            CamRight.onClick.AddListener(delegate { RotateCamera(-90.0f); });
        }
    }

    void SelectAbility(int i)
    {
        Selected_Ability = i;
        int Index = GridReference.Idx(Selected.SX, Selected.SZ);
        GridNode Current = GridReference.GridList[Index];
        Debug.Log(i);
        switch (Selected.Abilities[Selected_Ability])
        {
            case Student.Ability.MOVE:
                {
                    SetGridExcluded();
                    SetGridDefault(GetGridsInRange(Current, Selected.speed, false));
                    Action = true;
                    break;
                }
            case Student.Ability.ATTACK:
                {
                    SetGridExcluded();
                    SetGridDefault(GetGridsInRange(Current, AttackRange, true));
                    Action = true;
                    break;
                }
            case Student.Ability.OVERWATCH:
                {
                    // Set state here
                    networkObject.SendRpc(RPC_STUDENT_OVER_WATCH, Receivers.Others, Selected.uuid);
                    Selected.sm.SetNextState("Overwatch");
                    CameraMove(Current, -1.0f, 1.5f);
                    Selected.currMoves = 0;
                    break;
                }
            case Student.Ability.HUNKER:
                {
                    networkObject.SendRpc(RPC_STUDENT_HUNKER, Receivers.Others, Selected.uuid);
                    // Set state here
                    Selected.sm.SetNextState("Hunker");
                    CameraMove(Current, -1.0f, 1.5f);
                    Selected.currMoves = 0;
                    break;
                }
			case Student.Ability.PROCRASTINATE:
				{
					CameraMove(Current, -1.0f, 1.5f);
					Selected.currMoves = 0;
					break;
				}
        }
    }

    void SetGridExcluded()
    {
        for (int i = 0; i < GridReference.GridList.Count; ++i)
        {
            GridReference.GridList[i].SetExcluded();
        }
        SelectionList.Clear();
    }

    void SetGridDefault(List<int>range)
    {
        foreach (int i in range)
        {
            GridReference.GridList[i].SetDefault();
            SelectionList.Add(GridReference.GridList[i]);
        }
    }

    void EndTurnHelper(List<Student>End, List<Student>Start, bool team)
    {
        for (int i = 0; i < End.Count; ++i)
        {
            Student temp = End[i];
            temp.currMoves = temp.moves;
            End[i] = temp;
        }
        Selected = Start[Selected_ID];

        for (int i = 0; i < Start.Count; ++i)
        {
            Student temp = Start[i];


			// Set all to normal state
			if (temp.sm != null)
            {
                temp.sm.SetNextState("Normal");
                BeardedManStudios.Forge.Logging.BMSLog.Log("NORMAL_STATE");
            }
		}

		// Add Diploma points
		for (int i = 0; i < DiplomaAreas.Count; ++i)
		{
			bool StudentFound = false;
			List<GridNode> temp = DiplomaAreas[i];

			// Loop through grids in each area
			for (int j = 0; j < temp.Count; ++j)
			{
				// For each grid, loop through all students in Start
				for (int k = 0; k < Start.Count; ++k)
				{
					if (temp[j].student == Start[k])
					{
						StudentFound = true;
						if (team == true)
						{
							++PointsA;
							topbar.UpdateDiplomaA(PointsA);
						}
						else
						{
							++PointsB;
							topbar.UpdateDiplomaB(PointsB);
						}
						break;
					}
				}
				if (StudentFound == true)
				{
					break;
				}
			}
		}
	}

    void EndTurn(bool sendRPC = false)
    {	
        if (sendRPC)
        {
            networkObject.SendRpc(RPC_STUDENT_END_TURN, Receivers.Others);
        }
        Turn = !Turn;
        Selected_ID = 0;
         if (Turn == true)
        {
            EndTurnHelper(StudentListB, StudentListA, true);
            topbar.SetTurnA();
        }
        else
        {
            EndTurnHelper(StudentListA, StudentListB, false);
            topbar.SetTurnB();
        }

        LoadFlag = true;
        Clean();
        EndAction();
        int Index = GridReference.Idx(Selected.SX, Selected.SZ);
        CameraMove(GridReference.GridList[Index], CamSlow, 0.0f);
		TurnTimeCurr = TurnTimeMax;
	}

    Student SpawnStudent(int x, int z, int t, int teamType,string uuid = "")
    {
        int Index = GridReference.Idx(x, z);
        GridNode SpawnPoint = GridReference.GridList[Index];
        // GridNode SpawnPoint = GridReference.gameLevel.GridList[Index];

        // Instantiate student
        GameObject NewObject = Instantiate(StudentReference[t], CanvasReference.transform);
        Student NewStudent = NewObject.GetComponent<Student>();
        NewStudent.t = t; // T-Poser Fix
        if (uuid == "")
            NewStudent.uuid = Guid.NewGuid().ToString();
        else
            NewStudent.uuid = uuid;
        NewStudent.teamType = teamType;
        NewStudent.UpdateSXZ(x, z);
        MeshRenderer mr = NewObject.GetComponentInChildren<MeshRenderer>();
        if (teamType == 0)
            mr.material.color = new Color(1, 0, 1); // magenta
        else if (teamType == 1)
            mr.material.color = new Color(1, 1, 0); // yellow
        // Set student pos
        Vector3 pos = SpawnPoint.gameObject.transform.position;
        NewObject.transform.position = new Vector3(pos.x, StudentHeight, pos.z);

        // Instantiate Student Stats GUI
        GameObject NewGUI = Instantiate(StudentStats, UICanvasReference.transform);
        StudentStats NewStats = NewGUI.GetComponent<StudentStats>();
        NewStats.studentObj = NewObject;
        NewStats.Init();
		if (teamType == 0)
			NewStats.SetHealthColor(new Color(1, 0, 1));
		else if (teamType == 1)
			NewStats.SetHealthColor(new Color(1, 1, 0));

		// Add student to the grid
		SpawnPoint.student = NewStudent;

        // Assign and return
        GridReference.GridList[Index] = SpawnPoint;
        return NewStudent;
    }

    void BFSHelper_Path(int next, GridNode curr, Queue<GridNode> GridQueue)
    {
        GridNode temp = GridReference.GridList[next];
        if (temp.Visited == false && temp.IsObstacle == false && temp.student == null)
        {
            GridQueue.Enqueue(temp);
            GridReference.GridList[next].Visited = true;
            GridReference.GridList[next].Previous = curr;
        }
    }
    public Stack<Data.TPoint> GetPath(
        GridNode From,
        GridNode To,
        float interval,
        float finalWait)
    {
        Stack<Data.TPoint> ret = new Stack<Data.TPoint>();

        // Initialise variables
        Queue<GridNode> GridQueue = new Queue<GridNode>();
        GridQueue.Enqueue(From);

        // Clean grids
        for (int i = 0; i < GridReference.GridList.Count; ++i)
        {
            GridReference.GridList[i].Visited = false;
            GridReference.GridList[i].Previous = null;
        }
        // BFS Limit
        while (GridQueue.Count > 0)
        {
            GridNode curr = GridQueue.Dequeue();

            // If reached end
            if (curr == To)
            {
                while (!(curr == From))
                {
                    ret.Push(new Data.TPoint(curr, interval, 0.0f));
                    curr = curr.Previous;
                }
                // ret.Push(new Data.TPoint(curr.GetPos(), interval, finalWait));
                return ret;
            }

            #region Directions
            // UP
            if (IsWithinBounds(curr.GZ + 1))
            {
                int next = GridReference.Idx(curr.GX, curr.GZ + 1);
                BFSHelper_Path(next, curr, GridQueue);
            }
            // DOWN
            if (IsWithinBounds(curr.GZ - 1))
            {
                int next = GridReference.Idx(curr.GX, curr.GZ - 1);
                BFSHelper_Path(next, curr, GridQueue);
            }
            // LEFT
            if (IsWithinBounds(curr.GX - 1))
            {
                int next = GridReference.Idx(curr.GX - 1, curr.GZ);
                BFSHelper_Path(next, curr, GridQueue);
            }
            // RIGHT
            if (IsWithinBounds(curr.GX + 1))
            {
                int next = GridReference.Idx(curr.GX + 1, curr.GZ);
                BFSHelper_Path(next, curr, GridQueue);
            }
            #endregion
        }
        return ret;
    }

    static bool SendStudentOnce = false;

    // Syncs the student list
    public override void SendStudent(RpcArgs args)
    {
        if (SendStudentOnce == true)
            throw new Exception("SentStudent Called more than once!");
        BeardedManStudios.Forge.Logging.BMSLog.Log("Receive Students");
        byte[] sA = args.GetNext<byte[]>();
        byte[] sB = args.GetNext<byte[]>();
        string ServerUsername = args.GetNext<string>();
        string ClientUsername = args.GetNext<string>();
        //byte[] gridContent = args.GetNext<byte[]>();
        List<studserialkiller> sAL = sA.ByteArrayToObject<List<studserialkiller>>();
        List<studserialkiller> sBL = sB.ByteArrayToObject<List<studserialkiller>>();
        BeardedManStudios.Forge.Logging.BMSLog.Log("ASide: "+ sAL.Count + " BSide: " + sBL.Count);
        StartCommon();

        BeardedManStudios.Forge.Logging.BMSLog.Log("Rcv Server templates of students... Team A:");
        foreach (var item in sAL)
        {
            BeardedManStudios.Forge.Logging.BMSLog.Log("T: " + item.t + " | TeamType: " + item.teamtype + " | UUID: " + item.uuid);
        }
        BeardedManStudios.Forge.Logging.BMSLog.Log("Team B:");
        foreach (var item in sBL)
        {
            BeardedManStudios.Forge.Logging.BMSLog.Log("T: " + item.t + " | TeamType: " + item.teamtype + " | UUID: " + item.uuid);
        }

        foreach (var studentMagic in sAL)
        {
            Student nStudent = instance.SpawnStudent(studentMagic.SX, studentMagic.SZ, studentMagic.t, studentMagic.teamtype, studentMagic.uuid);
            Student.SType fkin = (Student.SType)studentMagic.t;
            nStudent.studentType = fkin;
            instance.StudentListA.Add(nStudent);
        }
        foreach (var studentMagic in sBL)
        {
            Student nStudent = instance.SpawnStudent(studentMagic.SX, studentMagic.SZ, studentMagic.t, studentMagic.teamtype, studentMagic.uuid);
            Student.SType fkin = (Student.SType)studentMagic.t;
            nStudent.studentType = fkin;
            instance.StudentListB.Add(nStudent);
        }
        SceneConstant.username = ServerUsername;
        SceneConstant.usernameclient = ClientUsername;
        instance.topbar.UsernameA = SceneConstant.username;
        instance.topbar.UsernameB = SceneConstant.usernameclient;
        instance.Turn = false;
        instance.EndTurn();
        instance.recvfirstplayers = true;
        instance.WaitRoot.SetActive(false);
        instance.UICanvasReference.SetActive(true);
        var Indexer = instance.GridReference.Idx(instance.StudentListB[0].SX, instance.StudentListB[0].SZ);
        CameraMove(instance.GridReference.GridList[Indexer], CamFast, 0.5f);
    }

    public override void StudentProjSpawn(RpcArgs args)
    {
        byte[] sA = args.GetNext<byte[]>();
        byte[] sB = args.GetNext<byte[]>();
        string uuid = args.GetNext<string>();
        studserialkiller studentA = sA.ByteArrayToObject<studserialkiller>();
        studserialkiller studentB = sB.ByteArrayToObject<studserialkiller>();
        BeardedManStudios.Forge.Logging.BMSLog.Log("Spawn Projectile...");
        instance.StudentAttack(GetStudentfromUUID(studentA.uuid), GetStudentfromUUID(studentB.uuid), true, uuid);
    }

    Student GetStudentfromUUID(string UUID)
    {
        foreach (var item in StudentListA)
        {
            if (item.uuid == UUID)
                return item;
        }
        foreach (var item in StudentListB)
        {
            if (item.uuid == UUID)
                return item;
        }
        return null;
    }

    public override void StudentMove(RpcArgs args)
    {
        string uuid = args.GetNext<string>();
        int TX = args.GetNext<int>();
        int TZ = args.GetNext<int>();
        Student s = instance.GetStudentfromUUID(uuid);
        instance.StudentMoveInternals(s, s.SX, s.SZ, TX, TZ, true);
    }

    public void SendStudentHit(GameObject projectile,Student student = null, int damage = 0)
    {
        string projUUID = "";
        string studUUID = "";
        foreach (var item in projectileMapper)
        {
            if (item.Value == projectile)
            {
                projUUID = item.Key;
                break;
            }
        }
        if (student != null)
            studUUID = student.uuid;
        BeardedManStudios.Forge.Logging.BMSLog.Log("StudentHit:" + projUUID + "|"+ studUUID + "|" + damage);
        networkObject.SendRpc(RPC_STUDENT_HIT, Receivers.Others, projUUID, studUUID, damage);
    }

    public override void StudentHit(RpcArgs args)
    {
        string UUID = args.GetNext<string>();
        string studentUUID = args.GetNext<string>();
        int damage = args.GetNext<int>();
        GameObject projectile = instance.projectileMapper[UUID];
        projectile.GetComponent<ProjectileManager>().enabled = false;
        Destroy(projectile);
        Student s = instance.GetStudentfromUUID(studentUUID);
        BeardedManStudios.Forge.Logging.BMSLog.Log("StudentHitRecv:" + UUID + "|"+s.currHealth+"|"+damage);
        if (studentUUID != "")
            s.currHealth -= damage;
        // Check for health too.
        if (s.currHealth <= 0)
        {
            BeardedManStudios.Forge.Logging.BMSLog.Log("Killing Pawn");
            s.currHealth = 0;
            instance.StudentDeath(s);
        }
    }

    public override void StudentEndTurn(RpcArgs args)
    {
        instance.EndTurn();
    }

    public override void StudentOverWatch(RpcArgs args)
    {
        string UUID = args.GetNext<string>();
        Student s = instance.GetStudentfromUUID(UUID);
        s.sm.SetNextState("Overwatch");
    }

    public override void StudentHunker(RpcArgs args)
    {
        string UUID = args.GetNext<string>();
        Student s = instance.GetStudentfromUUID(UUID);
        s.sm.SetNextState("Hunker");
    }

    public override void ClientStudentPre(RpcArgs args)
    {
        if (MultiPrefab.Instance.serverSide != MPSide.MP_SERVERALONE)
        {
            Debug.LogError("Server should receive this first!");
        }
        byte[] studentscreations = args.GetNext<byte[]>();
        string ClientUsername = args.GetNext<string>();
        List<studserialkiller> studentlists = studentscreations.ByteArrayToObject<List<studserialkiller>>();

        for (int i = 0; i < 4; ++i)
        {
            int x = instance.SpawnPointB[i].GX;
            int z = instance.SpawnPointB[i].GZ;
            instance.StudentListB.Add(instance.SpawnStudent(x, z, studentlists[i].t, 1));
        }
        BeardedManStudios.Forge.Logging.BMSLog.Log("Recv Client Students:");
        foreach (var item in instance.StudentListB)
        {
            BeardedManStudios.Forge.Logging.BMSLog.Log("T: " + item.t + " | TeamType: " + item.teamType + " | UUID: " + item.uuid);
        }
        SceneConstant.usernameclient = ClientUsername;
        if (SceneConstant.username == "")
            SceneConstant.username = "Player 1";
        instance.topbar.UsernameA = SceneConstant.username;
        instance.topbar.UsernameB = SceneConstant.usernameclient;
        instance.SendClientStudents();
        //instance.clientjoined = true;

        Turn = false;
        EndTurn();
        instance.WaitRoot.SetActive(false);
        instance.UICanvasReference.SetActive(true);
    }

    bool clientjoined = false;
    // Server Send students to Cleint
    void SendClientStudents()
    {
        List<studserialkiller> liststuda = new List<studserialkiller>();
        List<studserialkiller> liststudb = new List<studserialkiller>();
        
        foreach (var student in StudentListA)
        {
            liststuda.Add(new studserialkiller(student));
        }
        foreach (var student in StudentListB)
        {
            liststudb.Add(new studserialkiller(student));
        }
        BeardedManStudios.Forge.Logging.BMSLog.Log("Sending Client templates of students... Team A:");
        foreach (var item in liststuda)
        {
            BeardedManStudios.Forge.Logging.BMSLog.Log("T: "+item.t + " | TeamType: "+ item.teamtype + " | UUID: " + item.uuid);
        }
        BeardedManStudios.Forge.Logging.BMSLog.Log("Team B:");
        foreach (var item in liststuda)
        {
            BeardedManStudios.Forge.Logging.BMSLog.Log("T: " + item.t + " | TeamType: " + item.teamtype + " | UUID: " + item.uuid);
        }
        networkObject.SendRpc(RPC_SEND_STUDENT, Receivers.Others, liststuda.ObjectToByteArray(), liststudb.ObjectToByteArray(), SceneConstant.username, SceneConstant.usernameclient);
        instance.clientjoined = true;
    }
}