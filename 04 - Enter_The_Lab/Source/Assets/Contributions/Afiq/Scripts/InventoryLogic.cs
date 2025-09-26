using System.Collections;
using System.Collections.Generic;
using System.Globalization;
using TMPro;
using UnityEngine;
using UnityEngine.UI;



public class InventoryLogic : MonoBehaviour
{
    public GameObject CanvasReference;
    public GameObject GridReference;

    public float MouseUpdate;
    private float bounceTime;

    //private GridBuilder Grid;
    public LayerMask Layer;
    private GridNode CurrGrid;
    private List<GameObject> itemList = new List<GameObject>();
    private List<GameObject> heldItemList = new List<GameObject>();
    private List<GridNode> grid = new List<GridNode>();
    public Canvas MainCanvas;
    public GridNode[] heldGrid;
    public TextMeshProUGUI iSelect;
    public TextMeshProUGUI desc;
    public GameObject SwordArtOnline;
    public GameObject[] Abilities;
    private int selectIndex = 0;


    private bool takenInv = false;
    // Start is called before the first frame update
    void Start()
    {
        itemList = new List<GameObject>();
        heldItemList = new List<GameObject>();
        //Grid = gameObject.GetComponent<GridBuilder>();
        //Grid.Init();
        for(int i = 0; i < 10; ++i)
        {
            for(int a = 0; a < 10; ++a)
            {
                spawnGrid(a, i, GridReference);
            }

        }


        bounceTime = MouseUpdate;
        for(int i = 0; i < heldGrid.Length; ++i)
        {
            heldGrid[i].SetDefault();
        }
        //for (int i = 0; i < Grid.GridList.Count; ++i)
        //{
        //    Grid.GridList[i].Text_ID.SetActive(false);
        //    Grid.GridList[i].Text_XZ.SetActive(false);
        //}
        for (int i = 0; i < grid.Count; ++i)
        {
            grid[i].Text_ID.SetActive(false);
            grid[i].Text_XZ.SetActive(false);
        }

    }
    int lastHoverIndex = -1;
    int ccount = 0;
    // Update is called once per frame
    void Update()
    {
        // Grid Selection
        if (bounceTime > 0.0f)
        {
            bounceTime -= Time.deltaTime;
        }
        else
        {
            // Tracking mouse
            bounceTime = MouseUpdate;

            Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
            RaycastHit hit;

            // Set all grids to default state
            for (int i = 0; i < grid.Count; ++i)
            {
                grid[i].SetDefault();
            }
            for (int i = 0; i < heldGrid.Length; ++i)
            {
                heldGrid[i].SetDefault();
            }
            if (Physics.Raycast(ray, out hit, 9001.0f, Layer))
            {
                if (hit.collider.gameObject.name == "GridObject(Clone)" || hit.collider.gameObject.name == "GridObject")
                {
                    CurrGrid = hit.collider.gameObject.GetComponent<GridNode>();
                    CurrGrid.SetSelected();
                }
            }
        }
        #region HoverSelector
        if (CurrGrid != null)
        {
            int hoverIndex = CurrGrid.GX + CurrGrid.GZ * 10;
            if (hoverIndex != lastHoverIndex)
            {
                lastHoverIndex = hoverIndex;
                if (hoverIndex < itemList.Count)
                {
                    GameObject i = itemList[hoverIndex];
                    if (i.activeInHierarchy)
                        SetHoverStatistics(i.GetComponent<Student>());
                    else
                        SetHoverStatistics(null);
                }
                else
                    SetHoverStatistics(null);
            }
        }
        #endregion
        if (Input.GetMouseButtonDown(0) && CurrGrid != null)
        {
            BeardedManStudios.Forge.Logging.BMSLog.Log("Down!");
            if (CurrGrid.GX < 10)
            {
                BeardedManStudios.Forge.Logging.BMSLog.Log("In!");
                selectIndex = CurrGrid.GX + CurrGrid.GZ * 10;
                if (selectIndex < itemList.Count && ccount < 4)
                {
                    BeardedManStudios.Forge.Logging.BMSLog.Log("CCount OK");
                    if (itemList[selectIndex].activeInHierarchy)
                    {
                        BeardedManStudios.Forge.Logging.BMSLog.Log("Click!");
                        BeardedManStudios.Forge.Logging.BMSLog.Log(itemList[selectIndex].GetComponent<Student>().studentType.ToString());
                        Inventory.InvToHeld(selectIndex, ccount);
                        ReloadHeldItems();
                        ccount++;
                        //itemList[selectIndex].SetActive(false);
                    }
                }
            }
        }

        if (!takenInv && Inventory.isReady)
        {
            for (int i = 0; i < Inventory.InvItems.Count; ++i)
            {
                itemList.Add(spawnItem(i % 10, i / 10, i, Inventory.InvItems[i].item));
            }
            for(int i = 0; i < Inventory.HeldItems.Count; ++i)
            {
                heldItemList.Add(SpawnHeldtItem(i, Inventory.HeldItems[i].item));
            }
            takenInv = true;
            Inventory.isReady = false;
        }
    }

    GameObject spawnItem(int x, int z, int ID, GameObject itemRef)
    {
        int Index = x + z * 10;
        GridNode SpawnPoint = grid[Index];

        GameObject NewObject = Instantiate(itemRef, CanvasReference.transform);

        Vector3 pos = SpawnPoint.gameObject.transform.position;
        NewObject.transform.position = new Vector3(pos.x, 0.0f, pos.z - 0.8f);
        NewObject.transform.localScale = new Vector3(0.4f, 0.4f, 0.4f);
        NewObject.transform.Rotate(new Vector3(1.0f, 0.0f, 0.0f), 90);
        MeshRenderer mr = NewObject.GetComponentInChildren<MeshRenderer>();
        SpawnPoint.student = NewObject.GetComponentInChildren<Student>();
        mr.gameObject.layer = 8;
        mr.material.color = new Color(1, 0.5f, 0); // magenta
        grid[Index] = SpawnPoint;
        return NewObject;
    }

    GameObject SpawnHeldtItem(int z, GameObject itemRef)
    {
        int Index = z;
        GridNode SpawnPoint = heldGrid[Index];

        GameObject NewObject = Instantiate(itemRef, CanvasReference.transform);

        Vector3 pos = SpawnPoint.gameObject.transform.position;
        NewObject.transform.position = new Vector3(pos.x, 0.0f, pos.z - 0.8f);
        NewObject.transform.localScale = new Vector3(0.4f, 0.4f, 0.4f);
        NewObject.transform.Rotate(new Vector3(1.0f, 0.0f, 0.0f), 90);
        MeshRenderer mr = NewObject.GetComponentInChildren<MeshRenderer>();
        mr.gameObject.layer = 8;
        mr.material.color = new Color(1, 0.84f, 0);
        grid[Index].Text_ID.SetActive(false);
        grid[Index].Text_XZ.SetActive(false);
        SpawnPoint.student = NewObject.GetComponentInChildren<Student>();

        return NewObject;
    }

    void spawnGrid(int x, int z, GameObject itemRef)
    {
        GameObject NewObject = Instantiate(itemRef, CanvasReference.transform);
        GridNode NewNode = NewObject.GetComponent<GridNode>();
        NewNode.SetXZ(x, z);
        NewNode.UpdatePos(x * 2, z * 2, 0);
        grid.Add(NewNode);
    }

    // The below code is added By Josh as part of the GUI Overhaul.
    private TextInfo textInfo = new CultureInfo("en-US", false).TextInfo;
    void SetHoverStatistics(Student studentHover)
    {
        if (studentHover == null)
        {

            iSelect.text = "";
            for (int i = 0; i < 4; i++)
            {
                GameObject item = Abilities[i];
                item.GetComponent<Image>().CrossFadeColor(new Color(0.5f, 0.5f, 0.5f, 1), 0.2f, false, true);
            }
            desc.text = "";
        }
        else
        {
            iSelect.text = textInfo.ToTitleCase(studentHover.studentType.ToString());
            for (int i = 0; i < 4; i++)
            {
                GameObject item = Abilities[i];
                if (studentHover.Abilities.Contains((Student.Ability)i))
                {
                    item.GetComponent<Image>().CrossFadeColor(new Color(1, 1, 1, 1), 0.2f, false, true);
                }
                else
                {
                    item.GetComponent<Image>().CrossFadeColor(new Color(0.5f, 0.5f, 0.5f, 1), 0.2f, false, true);
                }

            }
            desc.text = "Health: " + studentHover.health + "\nDamage: " + studentHover.damage + "\nRange: " + studentHover.speed;
        }
    }
    void ReloadHeldItems()
    {

        for (int i = itemList.Count - 1; i >= 0; --i)
        {
            Destroy(itemList[i]);
            itemList.Remove(itemList[i]);
        }
        for (int i = 0; i < Inventory.InvItems.Count; ++i)
        {
            //Debug.Log(Inventory.InvItems[i].item.GetComponent<Student>().studentType + "|" + i);
            itemList.Add(spawnItem(i % 10, i / 10, i, Inventory.InvItems[i].item));
        }

        for (int i = heldItemList.Count - 1; i >= 0; --i)
        {
            Destroy(heldItemList[i]);
            heldItemList.Remove(heldItemList[i]);
        }
        for (int i = 0; i < Inventory.HeldItems.Count; ++i)
        {
            Debug.Log(Inventory.HeldItems[i].item.GetComponent<Student>().studentType + "|" + i);
            heldItemList.Add(SpawnHeldtItem(i, Inventory.HeldItems[i].item));
        }
    }


}
