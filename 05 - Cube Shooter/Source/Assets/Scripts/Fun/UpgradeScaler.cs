using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UpgradeScaler : MonoBehaviour
{
	public UPGRADE type = UPGRADE.NONE;
	public UpgradeUnit[] upgradeUnits;
	private int idx = 0;

	private void Start()
	{
		idx = DataManager.instance.inventory.getElement(type);

		for (int i = 0; i < idx; ++i)
		{
			upgradeUnits[i].setTrue();
		}
	}
	
    public void minusPressed()
	{
		if (ShopManager.instance.corePipProtocol(type, false))
		{
			idx -= 1;
			upgradeUnits[idx].setFalse();
		}
	}

	public void plusPressed()
	{
		if (ShopManager.instance.corePipProtocol(type, true))
		{
			upgradeUnits[idx].setTrue();
			idx += 1;
		}
	}
}
