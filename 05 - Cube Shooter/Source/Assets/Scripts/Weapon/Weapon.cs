using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Weapon : MonoBehaviour
{
	public Transform firePoint;
	public float reloadTime;
	public int ammoMaximum;
	public int ammoCurrent;
	public float damage;

	protected bool isReloading;
	protected float reloadTimer;
	protected float autoReload;

	[HideInInspector]
	public GameObject lineObject;
	[HideInInspector]
	public GameObject lineObjectWhite;

	protected Line prediction;
	protected float predictionRange = 500.0f;

	protected void Awake()
	{
		ammoCurrent = ammoMaximum;
		isReloading = false;
		reloadTimer = 0.0f;
	}

	// protected void autoReloadWeapon(float dt)
	// {
	// 	if (autoReload >= 1.0f)
	// 	{
	// 		if (ammoCurrent < ammoMaximum)
	// 		{
	// 			reload();
	// 		}
	// 	}
	// 	else
	// 	{
	// 		autoReload += dt;
	// 	}
	// }

	protected void LateUpdate()
	{
		// Auto reload
		if (!isReloading)
		{
			if (autoReload >= 1.0f)
			{
				if (ammoCurrent < ammoMaximum)
				{
					reload();
				}
			}
			else
			{
				autoReload += Time.deltaTime;
			}
		}
	}

	public virtual void constructPrediction()
	{
		prediction = Instantiate(lineObjectWhite, transform.root).GetComponent<Line>();
	}

	public virtual void predict()
	{
		contructPrediction();
	}

	public virtual void refill()
	{
		return;
	}

	public virtual void shoot()
	{
		return;
	}

	public virtual void release()
	{
		return;
	}

	public virtual bool reload()
	{
		if (isReloading)
		{
			Debug.Log("This should never call. By the way.");
			return true;
		}
		if (ammoCurrent < ammoMaximum)
		{
			isReloading = true;
			Reloader.instance.reloading.SetActive(true);
			return true;
		}
		else
		{
			return false;
		}
	}

	public void contructPrediction()
	{
		Vector3 cross = Camera.main.ScreenToWorldPoint(Input.mousePosition);
		Vector3 dir = (cross - firePoint.position).normalized;
		prediction.construct(firePoint.position, firePoint.position + dir * predictionRange);
	}

	public void shootRay()
	{
		Vector3 cross = Camera.main.ScreenToWorldPoint(Input.mousePosition);
		Vector3 dir = (cross - firePoint.position).normalized;
		castRay(dir);
	}

	public void shootRaySpread(float angle, int num)
	{
		Vector3 cross = Camera.main.ScreenToWorldPoint(Input.mousePosition);
		Vector3 dir = (cross - firePoint.position).normalized;

		float direct = Mathf.Atan2(dir.y, dir.x) * Mathf.Rad2Deg;

		float increment = angle / num;

		for (int i = num; i >= -num; --i)
		{
			float angleCur = (direct + increment * i) * Mathf.Deg2Rad;
			Vector3 dirCur = new Vector3(Mathf.Cos(angleCur), Mathf.Sin(angleCur), 0.0f);
			castRay(dirCur);
		}
	}

	private void castRay(Vector3 dir)
	{
		Line temp = Instantiate(lineObject, transform.root).GetComponent<Line>();

		// AUX_PIERCING IMPLEMENTATION
		if (DataManager.instance.inventory.getElement(UPGRADE.AUX_PIERCING) == 2)
		{
			RaycastHit2D[] hits = Physics2D.RaycastAll(firePoint.position, dir, 5000.0f, LayerMask.GetMask("Environment") | LayerMask.GetMask("Enemy"));

			if (hits.Length > 0)
			{
				for (int i = 0; i < hits.Length; ++i)
				{
					RaycastHit2D hit = hits[i];

					if (hit.collider.gameObject.layer == LayerMask.NameToLayer("Environment"))
					{
						temp.construct(firePoint.position, hit.point);
						break;
					}

					if (i == hits.Length - 1)
					{
						temp.construct(firePoint.position, hit.point);
					}

					Entity entity = hit.transform.GetComponent<Entity>();

					if (entity != null)
					{
						entity.takeDamage(damage);
					}
				}
			}
			else
			{
				temp.construct(firePoint.position, firePoint.position + dir * 5000.0f);
			}
		}
		else
		{
			RaycastHit2D hit = Physics2D.Raycast(firePoint.position, dir, 5000.0f, LayerMask.GetMask("Environment") | LayerMask.GetMask("Enemy"));

			if (hit)
			{
				// Debug.Log("Raycast hit: " + hit.transform.name);
				temp.construct(firePoint.position, hit.point);

				Entity entity = hit.transform.GetComponent<Entity>();

				if (entity != null)
				{
					entity.takeDamage(damage);
				}
			}
			else
			{
				temp.construct(firePoint.position, firePoint.position + dir * 5000.0f);
			}
		}
	}
}
