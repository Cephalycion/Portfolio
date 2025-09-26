using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ShopManager : MonoBehaviour
{
	public static ShopManager instance = null;
	public TMPro.TextMeshProUGUI starAmount;
	public int coreCost = 50;
	public Button AYS_Button = null;
	public MenuItem[] menuItems;

	private UPGRADE lastSelectedType = UPGRADE.NONE;

	private void Awake()
	{
		if (instance == null)
		{
			instance = this;
		}
		else
		{
			Debug.LogWarning("Multiple ShopManager instance.");
			return;
		}
	}

    // Start is called before the first frame update
    private void Start()
    {
		starAmount.text = DataManager.instance.inventory.stars.ToString();

		for (int i = 0; i < menuItems.Length; ++i)
		{
			int temp = i;
			menuItems[temp].button.onClick.AddListener(() => clicked(menuItems[temp]));
			menuItems[temp].infoButton.onClick.AddListener(() => infoClicked(menuItems[temp]));
		}
		AYS_Button.onClick.AddListener(() => menuItemProtocol());
		updateDisplays();
    }

	public bool corePipProtocol(UPGRADE type, bool purchase)
	{
		bool success = false;
		Inventory temp = DataManager.instance.inventory;

		// Do purchase
		if (purchase)
		{
			if (temp.stars - coreCost >= 0)
			{
				if (temp.setElement(type, temp.getElement(type) + 1))
				{
					success = true;
					temp.stars -= coreCost;
				}
			}
		}
		else
		{
			if (temp.setElement(type, temp.getElement(type) - 1))
			{
				success = true;
				temp.stars += coreCost;
			}
		}

		// Update
		if (success)
		{
			starAmount.text = temp.stars.ToString();
			DataManager.instance.inventory = temp;
			DataManager.instance.SaveInventory();
		}

		return success;
	}

	public void menuItemProtocol()
	{
		int cost = findCostOfUpgrade(lastSelectedType);
		bool success = false;
		Inventory temp = DataManager.instance.inventory;

		if (temp.stars - cost >= 0)
		{
			temp.setElement(lastSelectedType, 2);
			success = true;
			temp.stars -= cost;
		}

		// Update
		if (success)
		{
			starAmount.text = temp.stars.ToString();
			DataManager.instance.inventory = temp;
			DataManager.instance.SaveInventory();
			PopupManager.instance.holder.SetActive(false);
			updateDisplays();
		}
	}

	public int findCostOfUpgrade(UPGRADE type)
	{
		for (int i = 0; i < menuItems.Length; ++i)
		{
			MenuItem item = menuItems[i];
			if (item.type == type)
			{
				return item.cost;
			}
		}
		Debug.LogWarning("Attempted to buy nonexistent upgrade.");
		return 0;
	}

	public void clicked(MenuItem item)
	{
		lastSelectedType = item.type;
		Inventory temp = DataManager.instance.inventory;

		switch (DataManager.instance.inventory.getElement(lastSelectedType))
		{
			// If unowned, prompt player to buy
			case 0:
				{
					PopupManager.instance.setPopup(item.cost, item.description);
					break;
				}
			// If owned, equip it
			case 1:
				{
					temp.setElement(lastSelectedType, 2);
					break;
				}
			// If equipped, unequip it
			case 2:
				{
					// Cannot manually unequip a weapon
					if (lastSelectedType >= UPGRADE.WEP_PISTOL && 
						lastSelectedType <= UPGRADE.WEP_SHOTGUN)
					{
						return;
					}
					temp.setElement(lastSelectedType, 1);
					break;
				}
		}
		DataManager.instance.inventory = temp;
		DataManager.instance.SaveInventory();
		updateDisplays();
	}

	public void infoClicked(MenuItem item)
	{
		PopupManager.instance.setInfo(item.description);
		AudioManager.instance.play("Select");
	}

	public void updateDisplays()
	{
		for (int i = 0; i < menuItems.Length; ++i)
		{
			MenuItem item = menuItems[i];
			switch (DataManager.instance.inventory.getElement(item.type))
			{
				case 0:
					{
						item.selector.SetActive(false);
						break;
					}
				case 1:
					{
						item.selector.SetActive(false);
						item.costDisplay.SetActive(false);
						break;
					}
				case 2:
					{
						item.selector.SetActive(true);
						item.costDisplay.SetActive(false);
						break;
					}
			}
		}
	}
}
