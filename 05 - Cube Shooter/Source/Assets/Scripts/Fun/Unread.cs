using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Unread : MonoBehaviour
{
	public GameObject alert;

    public void checkUnread()
	{
		alert.SetActive(DataManager.instance.inventory.unread);
	}

	public void read()
	{
		Inventory inven = DataManager.instance.inventory;
		inven.unread = false;
		DataManager.instance.inventory = inven;
		DataManager.instance.SaveInventory();
		checkUnread();
	}
}
