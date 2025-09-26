using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Rifle : Weapon
{
	public float bangInterval;

	private float timer;
	private bool isShooting;

    // Start is called before the first frame update
    private void Start()
    {
		timer = 0.0f;
		isShooting = false;
		Reloader.instance.constructBar(ammoMaximum);
    }

	public override void refill()
	{
		isReloading = false;
		Reloader.instance.reloading.SetActive(false);
		reloadTimer = 0.0f;

		ammoCurrent = ammoMaximum;
		Reloader.instance.b_setAmmo(ammoCurrent);
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
			isShooting = true;
		}
		else
		{
			AudioManager.instance.play("RifleDry");
			// reload();
		}
	}

	public override void release()
	{
		timer = 0.0f;
		isShooting = false;
	}

	public override bool reload()
	{
		AudioManager.instance.play("RifleLoad");
		return base.reload();
	}

	// Update is called once per frame
	private void Update()
    {
        if (isShooting && ammoCurrent > 0)
		{
			if (timer > 0.0f)
			{
				timer -= Time.deltaTime;
			}
			else
			{
				timer = bangInterval;
				autoReload = 0.0f;
				shootRay();

				AudioManager.instance.play("RifleBang");

				if (Player.instance.activePowerup.type != POWERUP.INF_AMMO)
				{
					--ammoCurrent;
					Reloader.instance.b_setAmmo(ammoCurrent);
				}
			}
		}

		if (isReloading)
		{
			if (reloadTimer < reloadTime)
			{
				reloadTimer += Time.deltaTime;
			}
			else
			{
				ammoCurrent = ammoMaximum;
				Reloader.instance.b_setAmmo(ammoCurrent);
				reloadTimer = 0.0f;
				isReloading = false;
				Reloader.instance.reloading.SetActive(false);
			}
		}
		// else
		// {
		// 	autoReloadWeapon(Time.deltaTime);
		// }
    }
}
