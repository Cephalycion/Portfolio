using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Shield : Entity
{
	public static Shield instance;
	public Bar barShield;
	public Image image;

	public bool activated;

	public float shieldCurrent;
	private float shieldMaximum;
	private float shieldRecharge;
	private float shieldWait;

	private float timer;
	private float alpha;

	private void Awake()
	{
		if (instance == null)
		{
			instance = this;
		}
		else
		{
			Debug.LogWarning("Multiple Shield instance.");
			return;
		}
	}

	public void construct(float sMax, float sRecharge, float sWait)
	{
		shieldCurrent = sMax;
		shieldMaximum = sMax;
		shieldRecharge = sRecharge;
		shieldWait = sWait;

		barShield.setMax(sMax);
		activated = false;
		alpha = 1.0f;
	}

	public override void takeDamage(float damage)
	{
		shine();
		shieldCurrent = Mathf.Max(0, shieldCurrent -= damage);
		barShield.setValue(shieldCurrent);

		AudioManager.instance.play("ImpactShield");

		if (shieldCurrent == 0)
		{
			timer = shieldWait;
			alpha = 0.3f;

			Color temp = image.color;
			temp.a = alpha;
			image.color = temp;
		}
	}

	private void Update()
	{
		if (timer <= 0.0f)
		{
			if (shieldCurrent < shieldMaximum)
			{
				if (activated == false)
				{
					shieldCurrent = Mathf.Min(shieldCurrent + (shieldRecharge * Time.deltaTime), shieldMaximum);
					barShield.setValue(shieldCurrent);
					alpha = 1.0f;
				}
			}
		}
		else
		{
			timer -= Time.deltaTime;
		}
	}

	public void activate()
	{
		Color temp = image.color;
		temp.a = alpha;
		image.color = temp;
		activated = true;
	}

	public void deactivate()
	{
		Color temp = image.color;
		temp.a = 0;
		image.color = temp;
		timer = shieldWait;
		activated = false;
	}

	public void move(Vector3 pos)
	{
		gameObject.transform.position = pos;
	}

	public void rotate(float z)
	{
		Vector3 rotation = Vector3.zero;
		rotation.z = z;
		gameObject.transform.eulerAngles = rotation;
	}

	public override void gainHealth(float gain)
	{
		shieldCurrent = Mathf.Min(shieldCurrent + gain, shieldMaximum);
		barShield.setValue(shieldCurrent);
		alpha = 1.0f;
	}
}
