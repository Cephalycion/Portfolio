using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Pistol : Weapon
{
	public float c_startHeight;
	public float c_distance;

	private void Start()
	{
		float angle = 0;
		Vector3 center = new Vector3(0.0f, c_startHeight, 0.0f);

		for (int i = 0; i < ammoMaximum; ++i)
		{
			Vector3 pos = center + (new Vector3(Mathf.Cos(angle), Mathf.Sin(angle), 0.0f) * c_distance);
			Reloader.instance.constructCircle(pos);
			angle += Mathf.PI * 2 / ammoMaximum;
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
			shootRay();

			AudioManager.instance.play("PistolBang");
		}
		else
		{
			AudioManager.instance.play("PistolDry");
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

				AudioManager.instance.play("PistolLoad");

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
