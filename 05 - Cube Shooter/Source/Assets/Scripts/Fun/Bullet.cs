using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using DG.Tweening;

public class Bullet : MonoBehaviour
{
	public BULLET type;
	public Rigidbody2D rb;
	public Image image;
	public TrailRenderer trail;
	public float lifetime;

	public AudioSource normal;
	public AudioSource missile;

	private List<int> dontCollide = new List<int>();
	private float _lifetime;
	private Vector3 dir;
	private float speed;
	private float damage;

	private float deflectSpeedMultiplier	= 2.0f;
	private float deflectDamage				= 100.0f;
	private bool bulletActive = true;

	private Tween holder;

	private void Start()
	{
		normal.volume *= DataManager.instance.volume;
		missile.volume *= DataManager.instance.volume;
	}

	public void contruct(Vector3 dir, float speed, float damage, BULLET type)
	{
		this.dir = dir;
		this.speed = speed;
		this.damage = damage;
		this.type = type;

		switch (type)
		{
			case BULLET.NORMAL:
				{
					normal.Play();
					image.color = DataManager.instance.RED_COLOR;
					trail.colorGradient = DataManager.instance.RED_GRADIENT;
					break;
				}
			case BULLET.MISSILE:
				{
					missile.Play();
					image.color = DataManager.instance.ORANGE_COLOR;
					trail.colorGradient = DataManager.instance.ORANGE_GRADIENT;
					break;
				}
		}

		dontCollide.Clear();
		dontCollide.Add(LayerMask.NameToLayer("Enemy"));
		trail.Clear();
		trail.time = 0.5f;
		trail.enabled = true;
		bulletActive = true;
		Color color = image.color;
		color.a = 1.0f;
		image.color = color;
		transform.localScale = new Vector3(1, 1, 1);

		rb.velocity = dir * speed;
		_lifetime = 0.0f;
	}

	private void Update()
	{
		if (_lifetime < lifetime)
		{
			_lifetime += Time.deltaTime;

			if (type == BULLET.MISSILE)
			{
				Vector3 dir = (Player.instance.position - gameObject.transform.position).normalized;
				this.dir = dir;
				rb.velocity = dir * speed;
			}
		}
		else
		{
			bulletDeath();
		}
	}

	private void OnTriggerEnter2D(Collider2D collision)
	{
		if (!bulletActive)
		{
			return;
		}

		// Don't collide with whatever
		if (dontCollide.Contains(collision.gameObject.layer))
		{
			return;
		}

		Bullet bullet = collision.gameObject.GetComponent<Bullet>();
		if (bullet != null)
		{
			return;
		}

		Shield shield = collision.gameObject.GetComponent<Shield>();
		if (shield != null)
		{
			// Pass through a zeroed shield or a deactivated shield
			if (shield.shieldCurrent <= 0.0f || shield.activated == false)
			{
				return;
			}

			// Pass through an active shield with deflect on
			if (DataManager.instance.inventory.getElement(UPGRADE.AUX_DEFLECT) == 2)
			{
				shield.takeDamage(damage);
				dontCollide.Clear();
				dontCollide.Add(LayerMask.NameToLayer("Player"));
				dontCollide.Add(LayerMask.NameToLayer("Ignore Raycast"));

				type = BULLET.NORMAL;
				rb.velocity = dir * -speed * deflectSpeedMultiplier;
				_lifetime = 0.0f;
				damage = deflectDamage;

				image.color = DataManager.instance.AMBER_COLOR;
				trail.colorGradient = DataManager.instance.AMBER_GRADIENT;
				trail.time /= deflectSpeedMultiplier;

				return;
			}
		}

		// Apply damage to entity
		Entity entity = collision.gameObject.GetComponent<Entity>();
		if (entity != null)
		{
			entity.takeDamage(damage);
		}

		// Play sound if the bullet hits a wall
		if (collision.gameObject.layer == LayerMask.NameToLayer("Environment"))
		{
			AudioManager.instance.play("ImpactEnvironment");
		}

		// Destroy
		if (collision.gameObject.layer != LayerMask.NameToLayer("Powerup"))
		{
			bulletDeath();
		}
	}

	private void bulletDeath()
	{
		if (!bulletActive)
		{
			return;
		}

		trail.enabled = false;
		bulletActive = false;
		Color color = image.color;
		color.a = 0.0f;
		holder = DOTween.To(() => image.color, x => image.color = x, color, 1.0f)
			.SetEase(Ease.OutCubic);
		transform.DOScale(new Vector3(10.0f, 10.0f, 1.0f), 1.0f)
			.OnComplete(() => { destroyBullet(); });
	}

	private void destroyBullet()
	{
		holder.Kill();
		DOTween.Kill(gameObject.transform);
		gameObject.SetActive(false);
	}

	private void OnDestroy()
	{
		destroyBullet();
	}
}
