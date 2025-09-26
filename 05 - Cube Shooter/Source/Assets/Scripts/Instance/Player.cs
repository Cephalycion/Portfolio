using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using System;
using DG.Tweening;

public class Player : Entity
{
	public static Player instance = null;

	public TMPro.TextMeshProUGUI UI_Powerup_Type;
	public TMPro.TextMeshProUGUI UI_Powerup_Duration;
	public Jeticle[] jeticle;
	public Shield shield;
	public Rigidbody2D body;
	public BoxCollider2D coll;
	public Camera cam;
	public Sprite camFree;
	public Sprite camLock;
	public Image camImg;
	public RectTransform cross;
	public RectTransform background;
	public RectTransform topWall;
	public Bar barFuel;
	public Bar[] barHealth;
	public float fuelVel = 100000.0f;
	public float fuelMax = 1.0f;
	public float fuelFil = 1.0f;
	public float shieldDistance = 100.0f;
	public float shieldWait = 1.0f;
	public WeaponArray weapons;

	[Header("Slowmotion Properties")]
	public Bar				barSlow;
	public RectTransform	inverseSprite;
	public float			slow_Value;
	public float			slow_Current;
	public float			slow_Maximum;
	public float			slow_Recharge;
	public float			slow_Drain;
	public float			slow_Wait;

	private bool			slow_Modifiable = true;
	private bool			slow_Activated = false;
	private float			slow_Timer = -1.0f;

	[HideInInspector]
	public Powerup activePowerup;
	[HideInInspector]
	public Vector4 savedColor = new Vector4();

	// public Transform groundCheck;
	// public float groundDistance = 1.0f;
	// public LayerMask groundMask;
	// private bool isGrounded = false;

	private Vector2 input;			// Horizontal and vertical input
	private Vector2 mousePos;		// Mouse position in world space
	private PlayerData data;		// Data obtained from DataManager
	private float camMaxBoundsX;	// Max bounds camera X
	private float camMaxBoundsY;    // Max bounds camera Y
	private float camMinBoundsX;	// Min bounds camera X
	private float topY;				// Y axis lock for top wall
	private float fuelCur;
	private float endTrigger;
	private bool cameraLock;
	private bool attached = false;
	private float healthMax;

	private bool jetpack_Activated = false;

	private Color[] CLR_SET = {
	 new Color(0.0f, 1.0f, 0.0f),	// G
	 new Color(0.0f, 0.0f, 1.0f),	// B
	 new Color(1.0f, 0.0f, 0.0f)	// R
	};

	public Vector3 position
	{
		get { return gameObject.transform.position; }
		set { gameObject.transform.localPosition = value; }
	}

	private void Awake()
	{
		if (instance == null)
		{
			instance = this;
		}
		else
		{
			Debug.LogWarning("Multiple Player instance.");
			return;
		}

		data = DataManager.instance.getPlayerData();

		// Print for debug
		// Debug.Log("Health: " + data.health);
		// Debug.Log("Shield: " + data.shield);
		// Debug.Log("Recharge: " + data.recharge);
		// Debug.Log("Speed: " + data.speed);

		fuelCur = fuelMax;
		barHealth[0].setMax(data.health);
		barHealth[1].setMax(data.health);
		healthMax = data.health;
		shield.construct(data.shield, data.recharge, shieldWait);
		cameraLock = false;
		activePowerup = new Powerup();
		isAlive = false;

		// Slow motion init
		barSlow.setMax(slow_Maximum);
		Time.timeScale = 1.0f;
		Time.fixedDeltaTime = 0.02f;

		// Set color
		Inventory inven = DataManager.instance.inventory;
		Color color = new Color(0.0f, 0.0f, 0.0f);
		for (int i = 0; i < 3; ++i)
		{
			if (inven.getElement(UPGRADE.CBE_GREEN + i) == 2)
			{
				color += CLR_SET[i];
				barFuel.fill.color = color;
			}
		}
		savedColor = barFuel.fill.color;
	}

	public void refillFuel()
	{
		fuelCur = fuelMax;
		barFuel.setValue(fuelCur);
	}

	// Update is called once per frame
	private void Update()
	{
		/*
		isGrounded = Physics2D.OverlapCircle(
			groundCheck.position,
			groundDistance, 
			groundMask
			) != null;

		if (isGrounded && currJumps < data.jumps)
		{
			Debug.Log("Jumps refreshed.");
			currJumps = data.jumps;
		}
		*/

		if (!isAlive || GameManager.isPaused)
		{
			return;
		}

		input.x = Input.GetAxisRaw("Horizontal");
		input.y = Input.GetAxisRaw("Vertical");

		if (Input.GetMouseButtonDown(0))
		{
			weapons.shootActiveWeapon();
		}
		if (Input.GetMouseButtonUp(0))
		{
			weapons.releaseActiveWeapon();
		}

		if (Input.GetMouseButtonDown(1))
		{
			shield.activate();
		}

		if (Input.GetMouseButtonUp(1))
		{
			shield.deactivate();
		}

		// if (Input.GetKeyDown(KeyCode.R))
		// {
		// 	weapons.reloadActiveWeapon();
		// }

		if (Input.GetKeyDown(KeyCode.Q))
		{
			if (slow_Modifiable)
			{
				slow_Activated = !slow_Activated;
				updateSlowStatus();
			}
		}

		if (Input.GetKeyDown(KeyCode.E))
		{
			cameraLock = !cameraLock;

			if (cameraLock)
			{
				camImg.sprite = camLock;
			}
			else
			{
				camImg.sprite = camFree;
			}
		}

		if (Input.GetKey(KeyCode.Space))
		{
			input.y = 1;
		}

		if (position.x > endTrigger)
		{
			GameManager.instance.playerEnd();
			end();
		}

		if (activePowerup.duration > 0.0f)
		{
			activePowerup.duration -= Time.deltaTime;
			TimeSpan ts = TimeSpan.FromSeconds(activePowerup.duration);
			UI_Powerup_Duration.text = ts.ToString("ss\\:ff");
		}
		else
		{
			activePowerup.type = POWERUP.NONE;
			UI_Powerup_Type.gameObject.SetActive(false);
		}
	}

	private void LateUpdate()
	{
		if (!isAlive || GameManager.isPaused)
		{
			return;
		}

		Vector2 dir = (mousePos - body.position).normalized;
		float angle = Mathf.Atan2(dir.y, dir.x) * Mathf.Rad2Deg;

		weapons.move(gameObject.transform.position);
		weapons.rotate(angle);

		shield.move(gameObject.transform.position + (Vector3)(dir * shieldDistance));
		shield.rotate(angle);

		// Predict ray
		// weapons.predictActiveWeapon();
	}

	private void FixedUpdate()
	{
		if (GameManager.isPaused)
		{
			return;
		}

		// Move cross
		mousePos = cam.ScreenToWorldPoint(Input.mousePosition);
		cross.localPosition = new Vector3(mousePos.x, mousePos.y, 0);

		// Move camera
		Vector3 currPos;
		if (cameraLock)
		{
			currPos = body.position;
		}
		else
		{
			currPos = (mousePos + body.position) * 0.5f;
		}
		currPos.z = -10.0f;
		if (currPos.x < camMinBoundsX)
		{
			currPos.x = camMinBoundsX;
		}
		if (currPos.x > camMaxBoundsX)
		{
			currPos.x = camMaxBoundsX;
		}
		if (currPos.y < 0)
		{
			currPos.y = 0;
		}
		if (currPos.y > camMaxBoundsY)
		{
			currPos.y = camMaxBoundsY;
		}
		cam.transform.localPosition = currPos;

		// Update top wall
		topWall.position = new Vector2(currPos.x, topY);

		// Move background
		currPos.z = 0.0f;
		background.localPosition = currPos;

		// Alive check
		if (!isAlive)
		{
			return;
		}

		// Slowmotion
		if (slow_Activated)
		{
			slow_Current = Mathf.Max(slow_Current - (slow_Drain * Time.unscaledDeltaTime));
			barSlow.setValue(slow_Current);

			if (slow_Current <= 0.0f)
			{
				slow_Activated = false;
				updateSlowStatus();
			}
		}
		if (slow_Timer <= 0.0f)
		{
			if (slow_Current < slow_Maximum && slow_Activated == false)
			{
				slow_Current += slow_Recharge * Time.deltaTime;
				barSlow.setValue(slow_Current);
			}
		}
		else
		{
			slow_Timer -= Time.deltaTime;
		}

		// Update Jeticles
		for (int i = 0; i < jeticle.Length; ++i)
		{
			jeticle[i].setRotation(gameObject.transform.rotation.eulerAngles.z);
		}

		// Move player
		body.AddForce(new Vector2(input.x, 0) * Time.unscaledDeltaTime * data.speed);
		if (input.y > 0 && fuelCur > 0)
		{
			if (activePowerup.type != POWERUP.INF_FUEL)
			{
				fuelCur -= Time.deltaTime;
				barFuel.setValue(fuelCur);
			}
			body.AddForce(new Vector2(0, 1) * Time.unscaledDeltaTime * fuelVel);
			updateJetpackStatus(true);
		}
		else
		{
			updateJetpackStatus(false);
		}

		// Jeticle management
		for (int i = 0; i < jeticle.Length; ++i)
		{
			Vector2 dir = jeticle[i].direction;
			if (input.y > 0 && fuelCur > 0 && dir.y > 0.2f)
			{
				jeticle[i].play(jeticle[i].direction.y);
			}
			else
			{
				jeticle[i].play(0);
			}
		}

		// Fuel recharge
		if (attached)
		{
			attached = false;
			if (fuelCur < fuelMax)
			{
				fuelCur = Mathf.Clamp(fuelCur + (Time.deltaTime * fuelFil), 0.0f, fuelMax);
				barFuel.setValue(fuelCur);
			}
		}
	}

	private void updateJetpackStatus(bool status)
	{
		if (jetpack_Activated == status)
		{
			return;
		}
		jetpack_Activated = status;
		if (jetpack_Activated)
		{
			AudioManager.instance.play("Jetpack", true);
		}
		else
		{
			AudioManager.instance.stop("Jetpack");
		}
	}

	private void updateSlowStatus()
	{
		slow_Modifiable = false;
		if (slow_Activated)
		{
			AudioManager.instance.pitchAll(0.5f);

			Time.timeScale = slow_Value;
			Time.fixedDeltaTime = Time.timeScale * 0.02f;

			inverseSprite.DOScale(new Vector3(260.0f, 260.0f, 1.0f), 0.5f)
				.OnComplete(() => { slow_Modifiable = true; });
		}
		else
		{
			AudioManager.instance.pitchAll(1.0f);

			Time.timeScale = 1.0f;
			Time.fixedDeltaTime = 0.02f;
			slow_Timer = slow_Wait;

			inverseSprite.DOScale(new Vector3(0.0f, 0.0f, 1.0f), 0.5f)
				.OnComplete(() => { slow_Modifiable = true; });
		}
	}

	public void finishGenerationProtocol(float _camMaxBoundsX, float _camMaxBoundsY)
	{
		camMaxBoundsX = _camMaxBoundsX - DataManager.instance.s_width / 2;
		camMaxBoundsY = _camMaxBoundsY - DataManager.instance.s_height;
		topY = _camMaxBoundsY - DataManager.instance.s_height / 2;
		GameManager.instance.timing = true;
		endTrigger = _camMaxBoundsX;
		isAlive = true;
	}

	public void updateCamMinBoundsX(float leftWallPositionX)
	{
		camMinBoundsX = leftWallPositionX + DataManager.instance.s_width / 2;
	}

	private void OnCollisionStay2D(Collision2D collision)
	{
		// If collided with environment layer
		if (collision.gameObject.layer == LayerMask.NameToLayer("Environment"))
		{
			attached = true;
		}
	}

	public override void takeDamage(float damage)
	{
		shine();
		if (activePowerup.type == POWERUP.INF_HEALTH)
		{
			AudioManager.instance.play("ImpactShield");
			return;
		}
		if (!isAlive)
		{
			return;
		}

		data.health = Mathf.Max(0, data.health - damage);
		barHealth[0].setValue(data.health);
		barHealth[1].setValue(data.health);

		AudioManager.instance.play("ImpactPlayer");

		if (data.health == 0)
		{
			end();
			AudioManager.instance.play("PlayerDeath");
			Color color = barFuel.fill.color;
			color.a = 0.0f;
			DOTween.To(() => barFuel.fill.color, x => barFuel.fill.color = x, color, 1)
				.OnComplete(() => GameManager.instance.playerDied());
		}
	}

	private void end()
	{
		isAlive = false;
		shield.gameObject.SetActive(false);
		weapons.releaseActiveWeapon();
		weapons.gameObject.SetActive(false);
		Reloader.instance.gameObject.SetActive(false);
		for (int i = 0; i < jeticle.Length; ++i)
		{
			jeticle[i].play(0);
		}
	}

	public override void gainHealth(float gain)
	{
		if (!isAlive || activePowerup.type == POWERUP.INF_HEALTH)
		{
			return;
		}

		data.health = Mathf.Min(healthMax, data.health + gain);
		barHealth[0].setValue(data.health);
		barHealth[1].setValue(data.health);
	}
}
