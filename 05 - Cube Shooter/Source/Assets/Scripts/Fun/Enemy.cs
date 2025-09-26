using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Enemy : Entity
{
	public BULLET type;
	public Bar bar;
	public float health = 100;
	public float cooldown = 2.0f;
	public float visionRadius = 500.0f;
	public float castMin = 0.2f;
	public float castMax = 0.5f;
	public float bulletSpeed;
	public float bulletDamage;
	public Line debugRay;

	private float cdTimer;
	private float caTimer;

	private void Start()
	{
		if (DataManager.instance.inventory.getElement(UPGRADE.CBE_RED) == 2)
		{
			cooldown = 1.0f;
		}
		bar.setMax(health);
		cdTimer = cooldown;
		caTimer = Random.Range(castMin, castMax);
		isAlive = true;
	}

	private void Update()
	{
		if (!Player.instance.isAlive)
		{
			return;
		}

		if (cdTimer > 0.0f)
		{
			cdTimer -= Time.deltaTime;
		}
		else
		{
			Vector3 toPlayer = Player.instance.position - gameObject.transform.position;

			// If the player is within vision range, raycast every caTimer
			if (toPlayer.magnitude < visionRadius)
			{
				if (caTimer > 0.0f)
				{
					caTimer -= Time.deltaTime;
				}
				else
				{
					Vector3 dir = toPlayer.normalized;
					RaycastHit2D hit = Physics2D.CircleCast(gameObject.transform.position, 15.0f, dir, 5000.0f, LayerMask.GetMask("Environment") | LayerMask.GetMask("Player"));

					if (hit)
					{
						// debugRay.construct(gameObject.transform.position, hit.point);
						Player player = hit.transform.GetComponent<Player>();

						if (player != null)
						{
							cdTimer = cooldown;
							GameObject temp = GameManager.instance.getPooledObject();

							if (temp != null)
							{
								temp.transform.position = gameObject.transform.position;
								temp.SetActive(true);
								Bullet bullet = temp.GetComponent<Bullet>();
								bullet.contruct(dir, bulletSpeed, bulletDamage, type);
							}
							else
							{
								Debug.LogWarning("Insufficient pooled projectiles!");
							}
						}
					}
					caTimer = Random.Range(castMin, castMax);
				}
			}
		}
	}

	public override void takeDamage(float damage)
	{
		shine();
		health = Mathf.Max(0, health - damage);
		bar.setValue(health);

		if (health == 0)
		{
			if (Player.instance.isAlive && isAlive)
			{
				isAlive = false;
				GameManager.instance.increaseKills();
				AudioManager.instance.play("EnemyDeath");

				// AUX_FUELVAMP IMPLEMENTATION
				if (DataManager.instance.inventory.getElement(UPGRADE.AUX_FUELVAMP) == 2)
				{
					Player.instance.refillFuel();
				}

				// CBE_GREEN IMPLEMENTATION
				if (DataManager.instance.inventory.getElement(UPGRADE.CBE_GREEN) == 2)
				{
					Player.instance.gainHealth(10.0f);
				}

				// CBE_BLUE IMPLEMENTATION
				if (DataManager.instance.inventory.getElement(UPGRADE.CBE_BLUE) == 2)
				{
					Shield.instance.gainHealth(10.0f);
				}
			}
			Destroy(gameObject, 0.15f);
		}
	}
}
