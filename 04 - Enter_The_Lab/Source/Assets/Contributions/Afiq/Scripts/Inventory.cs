using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using PlayFab;
using PlayFab.ClientModels;

public struct Inventory_Item
{
    public GameObject item;
    public int count;
    public int itemIndex;

    public Inventory_Item(GameObject tempItem, int index)
    {
        item = tempItem;
        count = 1;
        itemIndex = index;
    }
}

public class Inventory : MonoBehaviour
{
    // Start is called before the first frame update

    //private PlayFabClient player;
    public GameObject[] itemReferences;
    public static List<Inventory_Item> InvItems = new List<Inventory_Item>();
    public static List<Inventory_Item> HeldItems = new List<Inventory_Item>();

    private static bool ready = false;

    private float timer = 0.0f;

    void Start()
    {
        //InvItems = new List<Item>();
        //HeldItems = new List<Item>();
        if(!PlayFabClient.GetInstance().isReady)
        {
            PlayFabClient.GetInstance().Login("Afiq", loginSuccess, error => Debug.LogError(error.GenerateErrorReport()));
        }
        else
        {
            PlayFabClient.GetInstance().GetItems(SetItems, error => Debug.LogError(error.GenerateErrorReport()));
        }
        
    }

    // Update is called once per frame
    void Update()
    {
    }



    public void addToInv(GameObject tempItem, int index)
    {
        
        Inventory_Item tempItems = new Inventory_Item(tempItem, index);
        InvItems.Add(tempItems);
    }

    public void removeInvItem(int index)
    {
        InvItems.Remove(InvItems[index]);
    }

    void SetItems(GetUserInventoryResult _result)
    {
        
        if(InvItems.Count + HeldItems.Count < _result.Inventory.Count)
        {
            for (int i = 0; i < _result.Inventory.Count; ++i)
            {
                int a = 0;
                switch (_result.Inventory[i].ItemId)
                {
                    case "Student_00":
                        a = 0;
                        break;
                    case "Student_01":
                        a = 1;
                        break;
                    case "Student_02":
                        a = 2;
                        break;
                    case "Student_03":
                        a = 3;
                        break;
                }
                GameObject tempItem = itemReferences[a];
                addToInv(tempItem, a);
            }
            ready = true;
        }

    }

    void loginSuccess(LoginResult _result)
    {
        PlayFabClient.GetInstance().isReady = true;
        Debug.Log("Logged in");
        //PlayFabClient.GetInstance().AddTuna()

        PlayFabClient.GetInstance().GetItems(SetItems, error => Debug.LogError(error.GenerateErrorReport()));
    }

    public static bool isReady
    {
        get
        {
            return ready;
        }
        set
        {
            ready = value;
        }
    }

    public static void InvToHeld(int InvIndex, int HeldIndex)
    {
        if(HeldIndex < HeldItems.Count)
        {
            InvItems.Add(HeldItems[HeldIndex]);
            HeldItems[HeldIndex] = InvItems[InvIndex];
            InvItems.Remove(InvItems[InvIndex]);
        }
        else
        {
            HeldItems.Add(InvItems[InvIndex]);
            InvItems.Remove(InvItems[InvIndex]);
        }
        ready = true;
    }
    public static void HeldToInv(int HeldIndex)
    {
        InvItems.Add(HeldItems[HeldIndex]);
        HeldItems.Remove(HeldItems[HeldIndex]);
        ready = true;
    }
}
