using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;
using DG.Tweening;

public class GameLogic : MonoBehaviour
{
	public static GameLogic instance = null;

	void Awake()
	{
		instance = this;
	}

	Queue<Data.TPoint> CamQueue = new Queue<Data.TPoint>();

	public GameObject CanvasReference;
	public List<GameObject> StudentReference = new List<GameObject>();

    private List<Student> StudentListA = new List<Student>(); // Team A
    private List<Student> StudentListB = new List<Student>(); // Team B
	private List<GridNode> SpawnPointA = new List<GridNode>();
	private List<GridNode> SpawnPointB = new List<GridNode>();

    private bool Turn; // [ T = A / F = B ]
    private bool Action;
    private Student Selected;
    private int Selected_ID;
    private GameObject CameraReference;
    private GridBuilder GridReference;
    private Vector3 MouseDelta;
    private Vector3 MousePos;
	private bool LoadFlag;
	public GameObject WinDisplay;
	private bool IsRunning;
	public int WinnerEarnings;
	public int LoserEarnings;
	private int PointsA;
	private int PointsB;
	public int WinThreshold;
	public int DiplomaRange;
	List<List<GridNode>> DiplomaAreas = new List<List<GridNode>>();

	// Camera
	public Vector3 CameraOffset;
	public Vector3 CameraRotation;
	public float PanSpeedX;
	public float PanSpeedZ;
	public float CamFast;
	public float CamSlow;
	private float CamTimer;
	private Vector3 TouchStart;

	// Student
	public float StudentHeight;
    private List<Button> ButtonList = new List<Button>();
    public GameObject UICanvasReference;
    public GameObject StudentStats;

    // Projectile
    public GameObject Projectile;
    public float ProjectileSpeed;
    public int AttackRange;
	private List<GameObject> ProjectileList = new List<GameObject>();

    #region Dev
    // Debug UI
    public GameObject UI_Turn;
    public GameObject UI_Action;
    public GameObject UI_Ability;
    public GameObject UI_Moves;
    public GameObject UI_Health;
    public GameObject UI_TEAM_A;
    public GameObject UI_TEAM_B;
    public GameObject ButtonReference;
    public float ButtonSpacing;

    // Debug Controller
    private int Selected_Ability;
    #endregion

    // Grid Selection
    public float MouseUpdate;
    private float BounceTime;
    public LayerMask Layer;
    private GridNode CurrGrid;
    private List<GridNode> SelectionList = new List<GridNode>();

    // Start is called before the first frame update
    void Start()
    {
		#region Init GridBuilder
		GridReference = GameObject.Find("GridBuilder").GetComponent<GridBuilder>();
        GridReference.Init();
		GridReference.GenerateLevel("M413_b.json");

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

        // SpawnStudent t parameter legend
        // 0 = Afiq
        // 1 = Spy
        // 2 = Shinon
        // 3 = Mbrine

		// Iterate through spawnpoints
		for (int i = 0; i < 4; ++i)
		{
			int x = SpawnPointA[i].GX;
			int z = SpawnPointA[i].GZ;
			StudentListA.Add(SpawnStudent(x, z, i, 0));
		}
		for (int i = 0; i < 4; ++i)
		{
			int x = SpawnPointB[i].GX;
			int z = SpawnPointB[i].GZ;
			StudentListB.Add(SpawnStudent(x, z, i, 1));
		}
		#endregion

		LoadFlag = false;
		CamTimer = 0.0f;
		BounceTime = MouseUpdate;
		IsRunning = true;
		PointsA = 0;
		PointsB = 0;

		// Get Camera Reference
		CameraReference = GameObject.Find("Main Camera");
        CameraReference.transform.Rotate(CameraRotation);

        // Start the game like as if Team B turn ended
        Turn = false;
        EndTurn();

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
				DiplomaAreas.Add(NewList);
			}
		}
	}

	// Update is called once per frame
	void Update()
    {
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
			if (Turn == true)
			{
				DoTurn(StudentListA);
			}
			else
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
		}
	}

	void WinCondition(bool team)
	{
		Debug.Log("WIN_CONDITION");
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

    Vector3 totalPan = new Vector3(0, 0, 0);
	void DoTurn(List<Student>Students)
	{
		#region Mouse pan (Legacy)
		// if (Input.GetMouseButtonDown(0))
		// {
		// 	MousePos = Input.mousePosition;
		// 	totalPan.Set(0, 0, 0);
		// }
		// if (Input.GetMouseButton(0))
		// {
		// 	MouseDelta = MousePos - Input.mousePosition;
		// 	MousePos = Input.mousePosition;
		// 	totalPan += MouseDelta;
		// 
		// 	// Change mouse delta from XY format to XZ format
		// 	Vector3 Delta = new Vector3(MouseDelta.x * PanSpeedX, 0.0f, MouseDelta.y * PanSpeedZ);
		// 
		// 	// Rotate by camera rotation
		// 	Delta = Quaternion.Euler(CameraRotation) * Delta;
		// 
		// 	// Assign
		// 	CameraReference.transform.position += new Vector3(Delta.x, 0.0f, Delta.z) * Time.deltaTime;
		// }
		#endregion

		// Mouse Pan
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
			#region Grid Selection
			if (BounceTime > 0.0f)
			{
				BounceTime -= Time.deltaTime;
			}
			else
			{
				// Tracking mouse
				BounceTime = MouseUpdate;

                Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
                RaycastHit hit;

                // Set selection grids to default state
                for (int i = 0; i < SelectionList.Count; ++i)
                {
                    SelectionList[i].SetDefault();
                }

                // If Raycast hit gameObject
                if (Physics.Raycast(ray, out hit, 9001.0f, Layer))
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
            #endregion

            if (Input.GetMouseButtonUp(0) && CurrGrid != null && !IsMouseOverUI()&& totalPan.magnitude < 2.0f)
            {
                switch (Selected.Abilities[Selected_Ability])
                {
                    case Student.Ability.MOVE:
                        {
                            // If the selected grid has no student
                            if (CurrGrid.student == null)
                            {
                                StudentMove(Students);
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
                                    break;
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
				EndTurn();
			}
		}
		#endregion
	}

	Vector3 GetWorldPosition(float z)
	{
		Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
		Plane ground = new Plane(Vector3.up, new Vector3(0, 0, z));
		float distance;
		ground.Raycast(ray, out distance);
		return ray.GetPoint(distance);
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

    void StudentMove(List<Student> Students)
    {
        // Remove student from the grid
        int Index = GridReference.Idx(Selected.SX, Selected.SZ);
        GridNode Previous = GridReference.GridList[Index];
        Previous.student = null;
        GridReference.GridList[Index] = Previous;

        // Put student in new grid
        Vector3 GridPos = CurrGrid.GetPos();
		// Selected.UpdatePos(GridPos.x, GridPos.z);
		Selected.Waypoints = GetPath(Previous, CurrGrid, 0.2f, 0.5f);
        CurrGrid.student = Selected;

        // Update XZ indexes
        Selected.UpdateSXZ(CurrGrid.GX, CurrGrid.GZ);

		// Move Camera
		CameraMove(CurrGrid, CamFast, 0.5f);

        // Remove Attack button
        for (int i = 0; i < ButtonList.Count; ++i)
        {
            if (Selected.Abilities[i] == Student.Ability.ATTACK)
            {
                ButtonList[i].onClick.RemoveAllListeners();
                Destroy(ButtonList[i].gameObject);
                ButtonList.RemoveAt(i);
                break;
            }
        }

        // Set selection grid back
        SetGridExcluded();
	}

	public void OverwatchCheck(GridNode Grid, Student student)
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

    void StudentAttack(Student From, Student To)
    {
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
		ProjectileList.Add(NewObject);

        NewObject.GetComponent<ProjectileManager>().Init(
            From.damage, 
            selectPos, 
            new Vector3(dir.x * ProjectileSpeed, 0.0f, dir.z * ProjectileSpeed),
            Allies);

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

        // Clean Button list
        for (int i = 0; i < ButtonList.Count; ++i)
        {
            ButtonList[i].onClick.RemoveAllListeners();
            Destroy(ButtonList[i].gameObject);
        }
        ButtonList.Clear();
        // Debug.Log("BUTTON_LIST : " + ButtonList.Count);
	}

	void LoadSelected()
	{
		int j;
		// Instantiate ability buttons
		for (j = 0; j < Selected.Abilities.Count; ++j)
		{
			GameObject NewObject = Instantiate(ButtonReference, UICanvasReference.transform);
			Vector3 DefaultPos = NewObject.transform.position;
			NewObject.transform.position = new Vector3(DefaultPos.x, DefaultPos.y - (j * ButtonSpacing), 0.0f);
			NewObject.GetComponentInChildren<Text>().text = "" + Selected.Abilities[j];
			Button NewButton = NewObject.GetComponent<Button>();
			ButtonList.Add(NewButton);

            // Add onclick listener
            int Index = j;
            NewButton.onClick.AddListener(delegate { SelectAbility(Index); });
        }

        // Instantiate cancel button
        {
            GameObject NewObject = Instantiate(ButtonReference, UICanvasReference.transform);
            Vector3 DefaultPos = NewObject.transform.position;
            NewObject.transform.position = new Vector3(DefaultPos.x, DefaultPos.y - (j * ButtonSpacing), 0.0f);
            NewObject.GetComponentInChildren<Text>().text = "CANCEL";
            Button NewButton = NewObject.GetComponent<Button>();
            ButtonList.Add(NewButton);

            // Add onclick listener
            NewButton.onClick.AddListener(delegate { EndAction(); });
        }
    }

    void SelectAbility(int i)
    {
        Selected_Ability = i;
		int Index = GridReference.Idx(Selected.SX, Selected.SZ);
		GridNode Current = GridReference.GridList[Index];
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
					Selected.sm.SetNextState("Overwatch");
					CameraMove(Current, -1.0f, 1.5f);
					Selected.currMoves = 0;
					break;
                }
            case Student.Ability.HUNKER:
                {
					// Set state here
					Selected.sm.SetNextState("Hunker");
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

			#region Add Diploma points (Legacy)
			// int Index = GridReference.Idx(temp.SX, temp.SZ);
			// if (GridReference.GridList[Index].TileType == GridNode.TYPE.T_DIPLOMA)
			// {
			// 	if (team == true)
			// 	{
			// 		++PointsA;
			// 	}
			// 	else
			// 	{
			// 		++PointsB;
			// 	}
			// }
			#endregion

			// Set all to normal state
			if (temp.sm != null)
			{
				temp.sm.SetNextState("Normal");
				Debug.Log("NORMAL_STATE");
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
						}
						else
						{
							++PointsB;
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

	void EndTurn()
	{	
		Turn = !Turn;
		Selected_ID = 0;
		if (Turn == true)
		{
			EndTurnHelper(StudentListB, StudentListA, true);
		}
		else
		{
			EndTurnHelper(StudentListA, StudentListB, false);
		}

		LoadFlag = true;
		Clean();
		EndAction();
		int Index = GridReference.Idx(Selected.SX, Selected.SZ);
		CameraMove(GridReference.GridList[Index], CamSlow, 0.0f);
	}

    Student SpawnStudent(int x, int z, int t, int teamType)
    {
        int Index = GridReference.Idx(x, z);
        GridNode SpawnPoint = GridReference.GridList[Index];
        // GridNode SpawnPoint = GridReference.gameLevel.GridList[Index];

        // Instantiate student
        GameObject NewObject = Instantiate(StudentReference[t], CanvasReference.transform);
        Student NewStudent = NewObject.GetComponent<Student>();
        NewStudent.UpdateSXZ(x, z);
        MeshRenderer mr = NewObject.GetComponentInChildren<MeshRenderer>();
        if (teamType == 0)
            mr.material.color = new Color(1, 0, 1);
        else if (teamType == 1)
            mr.material.color = new Color(1, 1, 0);
        // Set student pos
        Vector3 pos = SpawnPoint.gameObject.transform.position;
        NewObject.transform.position = new Vector3(pos.x, StudentHeight, pos.z);

        // Instantiate Student Stats GUI
        GameObject NewGUI = Instantiate(StudentStats, UICanvasReference.transform);
        StudentStats NewStats = NewGUI.GetComponent<StudentStats>();
        NewStats.studentObj = NewObject;
        NewStats.Init();

        // Add student to the grid
        SpawnPoint.student = NewStudent;

        // Assign and return
        GridReference.GridList[Index] = SpawnPoint;
        return NewStudent;
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
}
