using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using System.IO;
using System;
using System.Reflection;

[Serializable]
public class Level
{
    public List<GridNode> GridList = new List<GridNode>();
    public List<SaveGrid> getSaveData()
    {
        List<SaveGrid> tempList = new List<SaveGrid>();
        foreach(GridNode grid in GridList)
        {
            SaveGrid sgrid = new SaveGrid();
            List<string> gridData = grid.GetData();
            sgrid.BaseName = gridData[0];
            sgrid.TopName = gridData[1];
            sgrid.Rotation = ushort.Parse(gridData[2]);
            sgrid.Obstacle = bool.Parse(gridData[3]);
            sgrid.type = (GridNode.TYPE)Enum.Parse(typeof(GridNode.TYPE), gridData[4]);
            sgrid.FloorType = gridData[5];
            tempList.Add(sgrid);
        }
        return tempList;
    }
}

[Serializable]
public class SaveGrid
{
    public ushort Rotation;
    public string TopName;
    public string BaseName;
    public bool Obstacle;
    public GridNode.TYPE type;
    public string FloorType;
    public SaveGrid()
    {
        Rotation = 0;
        TopName = BaseName = "none";
        Obstacle = false;
        type = GridNode.TYPE.T_NONE;
        FloorType = "FloorWood";
    }
}
[Serializable]
public class SaveGridWrapper
{
    public List<SaveGrid> data;
}

public class GridBuilder : MonoBehaviour
{
    public GameObject CanvasReference;
    public GameObject GridReference;
    public int GridSize;
    public float RenderMultiplier;

    public Dictionary<string, Mesh> TileObjectMeshes;
    public List<string> TileMeshNames;	// Names which will be linked to each mesh
    public List<Mesh> TileMeshes;		// Mesh
    public List<Material> TileMats;		// Materials
    public List<bool> IsObstacle;		// Obstacles

    public Dictionary<string, Mesh> TopObjectMeshes;
    public List<string> TopMeshNames;	// Names which will be linked to each Table Top Mesh
    public List<Mesh> TopMeshes;		// Mesh
    public List<Material> TopMats;		// Materials

    public List<Material> FloorMats;

    public InputField saveName;

    Level _levelReference = new Level();

    public List<GridNode> GridList
    {
        get
        {
            return _levelReference.GridList;
        }
        set
        {
            _levelReference.GridList = value;
        }
    }

    private int lastID;

    public void Init()
    {
        TileObjectMeshes = new Dictionary<string, Mesh>();
        TopObjectMeshes = new Dictionary<string, Mesh>();
        for (int i = 0; i < TileMeshNames.Count; ++i)
        {
            if (i == TileMeshes.Count)
                break;
            TileObjectMeshes.Add(TileMeshNames[i], TileMeshes[i]);
        }
        for (int i = 0; i < TopMeshNames.Count; ++i)
        {
            if (i == TopMeshes.Count)
                break;
            TopObjectMeshes.Add(TopMeshNames[i], TopMeshes[i]);
        }
        GridList = new List<GridNode>();

        // gameLevel = new Level();
        // Generate grids
        GenerateLevel(-1);

        #region Dev
        // Set Camera position
        // Vector3 pos1 = GridList[0].GetPos();
        // Vector3 pos2 = GridList[lastID - 1].GetPos();
        // Vector3 pos3 = new Vector3((pos1.x + pos2.x) / 2, 0.0f, (pos1.z + pos2.z) / 2);
        // GameObject.Find("Main Camera").transform.position = new Vector3(pos3.x, CameraHeight, pos3.z);
        // Note: [ X65 / Y45 / Z0 ] rotation looks pretty good. Need to test for offset later.
        #endregion
    }

    public void Init(int level)
    {
        for (int i = 0; i < TileMeshNames.Count; ++i)
        {
            if (i == TileMeshes.Count)
                break;
            TileObjectMeshes.Add(TileMeshNames[i], TileMeshes[i]);
        }
        for (int i = 0; i < TopMeshNames.Count; ++i)
        {
            if (i == TopMeshes.Count)
                break;
            TopObjectMeshes.Add(TopMeshNames[i], TopMeshes[i]);
        }

        // Generate grids
        lastID = 0;
        for (int z = 0; z < GridSize; ++z)
        {
            for (int x = 0; x < GridSize; ++x)
            {
                GameObject NewGrid = Instantiate(GridReference, CanvasReference.transform);
                GridNode NewNode = NewGrid.GetComponent<GridNode>();
                NewNode.SetID(lastID);
                NewNode.SetXZ(x, z);
                NewNode.UpdatePos(x * RenderMultiplier, z * RenderMultiplier, 0);
                GridList.Add(NewNode);

                ++lastID;
            }
        }

        // Generate grids
        GenerateLevel(level);

        #region Dev
        // Set Camera position
        // Vector3 pos1 = GridList[0].GetPos();
        // Vector3 pos2 = GridList[lastID - 1].GetPos();
        // Vector3 pos3 = new Vector3((pos1.x + pos2.x) / 2, 0.0f, (pos1.z + pos2.z) / 2);
        // GameObject.Find("Main Camera").transform.position = new Vector3(pos3.x, CameraHeight, pos3.z);
        // Note: [ X65 / Y45 / Z0 ] rotation looks pretty good. Need to test for offset later.
        #endregion
    }

    public void GenerateLevel(int level)
    {
        lastID = 0;
        //if (level == -1)
        {
            for (int z = 0; z < GridSize; ++z)
            {
                for (int x = 0; x < GridSize; ++x)
                {
                    GameObject NewGrid = Instantiate(GridReference, CanvasReference.transform);
                    GridNode NewNode = NewGrid.GetComponent<GridNode>();
                    NewNode.SetID(lastID);
                    NewNode.SetXZ(x, z);
                    NewNode.UpdatePos(x * RenderMultiplier, z * RenderMultiplier, 0);
                    NewNode.IsObstacle = false;
                    NewNode.SetFloor(FloorMats[0], FloorMats[0].name);
                    GridList.Add(NewNode);
                    // gameLevel.GridList.Add(NewNode);
                    if (x < 4 && z == 0)
                        NewNode.TileType = GridNode.TYPE.T_SPAWN_A;
                    else if (x >= GridSize - 4 && z == GridSize - 1)
                        NewNode.TileType = GridNode.TYPE.T_SPAWN_B;
                    ++lastID;
                }
            }
        }
    }

    public void GenerateLevel(List<SaveGrid> newGrid)
    {
        lastID = 0;
        GridSize = (int)Math.Sqrt(newGrid.Count);
        {
            for (int z = 0; z < GridSize; ++z)
            {
                for (int x = 0; x < GridSize; ++x)
                {
                    GameObject NewGrid = Instantiate(GridReference, CanvasReference.transform);
                    GridNode NewNode = NewGrid.GetComponent<GridNode>();
                    NewNode.SetID(lastID);
                    NewNode.SetXZ(x, z);
                    NewNode.UpdatePos(x * RenderMultiplier, z * RenderMultiplier, 0);
                    NewNode.rotation = newGrid[z * GridSize + x].Rotation;
                    NewNode.tileBName = newGrid[z * GridSize + x].BaseName;
                    NewNode.tileTName = newGrid[z * GridSize + x].TopName;
                    NewNode.TileType = newGrid[z * GridSize + x].type;
                    NewNode.floorName = newGrid[z * GridSize + x].FloorType;
                    //Set Meshes
                    string meshBaseRef = (NewNode.tileBName);
                    int meshBaseNum = 0;
                    for (int i = 0; i < TileMeshNames.Count; ++i)
                    {
                        if (meshBaseRef == TileMeshNames[i])
                        {
                            meshBaseNum = i;
                        }
                    }

                    string meshTopRef = (NewNode.tileTName);
                    int meshTopNum = 0;
                    for (int i = 0; i < TopMeshNames.Count; ++i)
                    {
                        if (meshTopRef == TopMeshNames[i])
                        {
                            meshTopNum = i;
                        }
                    }

                    string floorRef = (NewNode.floorName);
                    int floorNum = 0;
                    for (int i = 0; i < FloorMats.Count; ++i)
                    {
                        if (floorRef == FloorMats[i].name)
                        {
                            floorNum = i;
                        }
                    }

                    Material _baseMat = null;
                    if (meshBaseNum < TileMats.Count)
                    {
                        _baseMat = TileMats[meshBaseNum];
                    }
                    Material _topMat = null;
                    if (meshTopNum < TopMats.Count)
                    {
                        _topMat = TopMats[meshTopNum];
                    }

                    Material _floorMat = null;
                    if (floorNum < FloorMats.Count)
                    {
                        _floorMat = FloorMats[floorNum];
                    }

                    // Debug.Log(meshBaseNum);
                    NewNode.SetMesh(
                        TileObjectMeshes[TileMeshNames[meshBaseNum]],
                        _baseMat,
                        TileMeshNames[meshBaseNum],
                         TopObjectMeshes[TopMeshNames[meshTopNum]],
                        _topMat,
                        NewNode.tileTName);

                    if (meshBaseNum < IsObstacle.Count)
                        NewNode.IsObstacle = IsObstacle[meshBaseNum];
                    else
                        NewNode.IsObstacle = false;

                    NewNode.SetFloor(_floorMat, floorRef);

                    GridList.Add(NewNode);
                    ++lastID;
                }
            }
        }
    }
    public void GenerateLevel(string fileName)
    {
        SaveGridWrapper sgdwrpr = ReadFromFile(fileName);
        if (sgdwrpr == null)
        {
            BeardedManStudios.Forge.Logging.BMSLog.Log(fileName + " does not exist!!!");
            return;
        }
        else
            BeardedManStudios.Forge.Logging.BMSLog.Log(fileName + " found");
        foreach (GridNode grid in GridList)
        {
            Destroy(grid.gameObject);
        }
        GridList.Clear();
        GenerateLevel(sgdwrpr.data);

    }

    public void SaveLevel(string fileName)
    {
        string filePath = GetFilePath(fileName);
        Debug.Log(filePath);
        FileStream file;
        Level templvl = _levelReference;
        SaveGridWrapper sgrdwrpr = new SaveGridWrapper();
        sgrdwrpr.data = templvl.getSaveData();
        if (File.Exists(filePath))
            file = File.OpenWrite(filePath);
        else
            file = new FileStream(filePath, FileMode.Create);

        string json = JsonUtility.ToJson(sgrdwrpr, true);
        Debug.Log("JSON: " + json);
        using (StreamWriter writer = new StreamWriter(file))
        {
            writer.Write(json);
        }
    }

    private string GetFilePath(string path)
    {
        BeardedManStudios.Forge.Logging.BMSLog.Log(Application.dataPath + "/Resources/Text" + path);
        BeardedManStudios.Forge.Logging.BMSLog.Log(Path.GetDirectoryName(Assembly.GetExecutingAssembly().CodeBase));
        return Application.dataPath + "/Resources/Text" + path;
    }

    public SaveGridWrapper ReadFromFile(string path)
    {
        Debug.Log("Text/" + path);
        TextAsset textAsset = Resources.Load<TextAsset>("Text/" + path);
        SaveGridWrapper sgrdwrpr = JsonUtility.FromJson<SaveGridWrapper>(textAsset.text);
        return sgrdwrpr;
    }

    public int Idx(int x, int z)
    {
        return (z * GridSize + x);
    }

    public void ChangeGridSize(int newGridSize)
    {
        if (newGridSize < GridSize)
        {
            List<GridNode> removalList = new List<GridNode>();
            foreach (GridNode grid in GridList)
            {
                if (grid.GX >= newGridSize || grid.GZ >= newGridSize)
                {
                    removalList.Add(grid);
                }
            }
            foreach (GridNode grid in removalList)
            {
                GridList.Remove(grid);
                Destroy(grid.gameObject);
            }
        }
        else if (newGridSize > GridSize)
        {
            for (int z = GridSize - 1; z >= 0; --z)
            {
                for (int x = GridSize; x < newGridSize; ++x)
                {
                    GameObject NewGrid = Instantiate(GridReference, CanvasReference.transform);
                    GridNode NewNode = NewGrid.GetComponent<GridNode>();
                    NewNode.SetID(0);//Will be changed for all grids later
                    NewNode.SetXZ(x, z);
                    NewNode.UpdatePos(x * RenderMultiplier, z * RenderMultiplier, 0);
                    NewNode.rotation = 0;
                    NewNode.tileBName = "none";
                    NewNode.tileTName = "none";
                    NewNode.TileType = GridNode.TYPE.T_NONE;
                    NewNode.SetFloor(FloorMats[0], FloorMats[0].name);


                    GridList.Insert(Idx(x, z), NewNode);
                }
            }
            for (int z = GridSize; z < newGridSize; ++z)
            {
                for (int x = 0; x < newGridSize; ++x)
                {
                    GameObject NewGrid = Instantiate(GridReference, CanvasReference.transform);
                    GridNode NewNode = NewGrid.GetComponent<GridNode>();
                    NewNode.SetID(0);//Will be changed for all grids later
                    NewNode.SetXZ(x, z);
                    NewNode.UpdatePos(x * RenderMultiplier, z * RenderMultiplier, 0);
                    NewNode.rotation = 0;
                    NewNode.tileBName = "none";
                    NewNode.tileTName = "none";
                    NewNode.TileType = GridNode.TYPE.T_NONE;
                    NewNode.SetFloor(FloorMats[0], FloorMats[0].name);
                    GridList.Add(NewNode);
                }
            }

        }
        GridSize = newGridSize;
        lastID = 0;
        foreach (GridNode grid in GridList)
        {
            grid.SetID(lastID);
            ++lastID;
        }
    }
}
