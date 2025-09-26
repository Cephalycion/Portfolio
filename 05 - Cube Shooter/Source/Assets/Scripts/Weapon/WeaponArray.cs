using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class WeaponArray : MonoBehaviour
{
	public GameObject linePrefab;
	public GameObject linePrefabWhite;
	public Weapon[] weapons;

	private Weapon activeWeapon;

	private void Start()
	{
		UPGRADE weapon = UPGRADE.NONE;
		int chosenWeapon = 0;

		for (int i = (int)UPGRADE.WEP_PISTOL; i <= (int)UPGRADE.WEP_SHOTGUN; ++i)
		{
			weapon = (UPGRADE)i;
			if (DataManager.instance.inventory.getElement(weapon) == 2)
			{
				break;
			}
		}
		switch (weapon)
		{
			case UPGRADE.WEP_PISTOL:
				{
					chosenWeapon = 0;
					break;
				}
			case UPGRADE.WEP_RIFLE:
				{
					chosenWeapon = 1;
					break;
				}
			case UPGRADE.WEP_SHOTGUN:
				{
					chosenWeapon = 2;
					break;
				}
		}
		activeWeapon = weapons[chosenWeapon];
		activeWeapon.gameObject.SetActive(true);

		for (int i = 0; i < weapons.Length; ++i)
		{
			weapons[i].lineObject = linePrefab;
			weapons[i].lineObjectWhite = linePrefabWhite;
		}
		// activeWeapon.constructPrediction();
	}

	public void move(Vector3 pos)
	{
		gameObject.transform.position = pos;
	}

	public void rotate(float z)
	{
		Vector3 rotation = Vector3.zero;

		if (z < -90.0f || z > 90.0f)
		{
			rotation.y = 180.0f;
			z = -z + 180.0f;
		}
		else
		{
			rotation.y = 0.0f;
		}
		rotation.z = z;

		gameObject.transform.eulerAngles = rotation;
	}

	public void shootActiveWeapon()
	{
		activeWeapon.shoot();
	}

	public void releaseActiveWeapon()
	{
		activeWeapon.release();
	}

	public void reloadActiveWeapon()
	{
		activeWeapon.reload();
	}

	public void refillActiveWeapon()
	{
		activeWeapon.refill();
	}

	public void predictActiveWeapon()
	{
		activeWeapon.predict();
	}
}
