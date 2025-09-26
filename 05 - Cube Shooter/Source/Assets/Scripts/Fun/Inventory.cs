using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[System.Serializable]
public class Inventory
{
	public float version = 1.01f;
	public int stars = 0;
	public int[] unlockedUpgrades;
	public float[][] matchHistory;
	public string[] customMaps;
	public bool unread = false;

	public int getElement(UPGRADE i)
	{
		return unlockedUpgrades[(int)i];
	}

	public bool setElement(UPGRADE i, int j)
	{
		switch (i)
		{
			case UPGRADE.CORE_HEALTH:
			case UPGRADE.CORE_RECHARGE:
			case UPGRADE.CORE_SHIELD:
			case UPGRADE.CORE_SPEED:
				{
					if (j < 0 || j > 5)
					{
						return false;
					}
					break;
				}
			case UPGRADE.AUX_FUELVAMP:
			case UPGRADE.AUX_PIERCING:
			case UPGRADE.AUX_DEFLECT:
			case UPGRADE.WEP_PISTOL:
			case UPGRADE.WEP_RIFLE:
			case UPGRADE.WEP_SHOTGUN:
			case UPGRADE.CBE_GREEN:
			case UPGRADE.CBE_BLUE:
			case UPGRADE.CBE_RED:
				{
					// 0 is unowned
					// 1 is owned
					// 2 is equipped
					if (j < 0 || j > 2)
					{
						return false;
					}
					break;
				}
		}

		// If weapon set element, make sure to unequip what is equipped first
		if (i >= UPGRADE.WEP_PISTOL &&
			i <= UPGRADE.WEP_SHOTGUN)
		{
			for (int k = (int)UPGRADE.WEP_PISTOL; k <= (int)UPGRADE.WEP_SHOTGUN; ++k)
			{
				if (unlockedUpgrades[k] == 2)
				{
					unlockedUpgrades[k] = 1;
				}
			}
		}

		unlockedUpgrades[(int)i] = j;
		return true;
	}

	public Inventory()
	{
		// Initialise memory for arrays
		unlockedUpgrades = new int[(int)UPGRADE.LENGTH];
		unlockedUpgrades[(int)UPGRADE.WEP_PISTOL] = 2;
		matchHistory = new float[5][]
		{
			new float[18],
			new float[18],
			new float[18],
			new float[18],
			new float[18],
		};
		for (int i = 0; i < 5; ++i)
		{
			for (int j = 0; j < 18; ++j)
			{
				matchHistory[i][j] = -1.0f;
			}
		}
		customMaps = new string[5];
		for (int i = 0; i < 5; ++i)
		{
			customMaps[i] = "";
		}
	}
}
