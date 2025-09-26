using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Shotgun : Weapon
{
	public float c_startHeight;
	public float c_width;
	public float c_gap;
	public float s_angle;
	public int s_num;

	private void Start()
	{
		float xOffset = ((c_width + c_gap) * (ammoMaximum - 1)) * 0.5f;
		Vector3 center = new Vector3(-xOffset, c_startHeight, 0.0f);

		for (int i = 0; i < ammoMaximum; ++i)
		{
			Vector3 pos = center + new Vector3(c_width + c_gap, 0.0f, 0.0f) * i;
			Reloader.instance.constructCircle(pos);
		}
	}

	public override void refill()
	{
		isReloading = false;
		Reloader.instance.reloading.SetActive(false);
		reloadTimer = 0.0f;

		for (int i = ammoCurrent; i < ammoMaximum; ++i)
		{
			Reloader.instance.c_fill(ammoCurrent);
			++ammoCurrent;
		}
	}

	public override void shoot()
	{
		if (ammoCurrent > 0)
		{
			if (isReloading)
			{
				isReloading = false;
				Reloader.instance.reloading.SetActive(false);
				reloadTimer = 0.0f;
			}

			if (Player.instance.activePowerup.type != POWERUP.INF_AMMO)
			{
				--ammoCurrent;
				Reloader.instance.c_unfill(ammoCurrent);
			}
			autoReload = 0.0f;
			shootRaySpread(s_angle, s_num);
			AudioManager.instance.play("ShotgunBang");
		}
		else
		{
			AudioManager.instance.play("ShotgunDry");
			// reload();
		}
	}

	public override bool reload()
	{
		return base.reload();
	}

	private void Update()
	{
		if (isReloading)
		{
			if (reloadTimer < reloadTime)
			{
				reloadTimer += Time.deltaTime;
			}
			else
			{
				Reloader.instance.c_fill(ammoCurrent);
				++ammoCurrent;
				reloadTimer = 0.0f;

				AudioManager.instance.play("ShotgunLoad");

				if (ammoCurrent >= ammoMaximum)
				{
					isReloading = false;
					Reloader.instance.reloading.SetActive(false);
				}
			}
		}
		// else
		// {
		// 	autoReloadWeapon(Time.deltaTime);
		// }
	}
}
