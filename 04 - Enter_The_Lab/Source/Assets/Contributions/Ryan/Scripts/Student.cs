using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using DG.Tweening;
using System;

[SerializeField]
public class Student : MonoBehaviour
{
	public enum Ability
	{
		MOVE = 0,
		ATTACK,
		OVERWATCH,
		HUNKER,
		PROCRASTINATE
	};

	public enum SType
	{
		AFIQ = 0,
		SPY,
		RISTELLISE,
		MBRINE
	};

	public int health;  // Default values. Do not change during gameplay!
	public int damage;	// Default values. Do not change during gameplay!
	public int speed;	// Default values. Do not change during gameplay!
	public int moves;	// Default values. Do not change during gameplay!

    public AudioClip fireSound;
    public AudioClip deathSound;

    public List<AudioClip> footstepSounds;

    public StateMachine sm;

	[HideInInspector]
	public int currMoves;
	[HideInInspector]
	public int currHealth;

	public string uuid = "";
	public int SX;
	public int SZ;
	public int t; // Ryan didn't store it lol.
	public int teamType; // Matt didn't store it lol.

	public SType studentType;
	public List<Ability> Abilities = new List<Ability>();
	public Stack<Data.TPoint> Waypoints = 
		new Stack<Data.TPoint>();
	private float WayTimer;

	void Start()
	{
		sm = gameObject.AddComponent<StateMachine>();
        sm.AddState(new StateHunker("Hunker"));
        sm.AddState(new StateNormal("Normal"));
        sm.AddState(new StateOverwatch("Overwatch"));
        sm.SetNextState("Normal");
		currMoves = moves;
		currHealth = health;
		WayTimer = 0.0f;
	}

	void Update()
	{
		if (WayTimer > 0.0f)
		{
			WayTimer -= Time.deltaTime;
		}
		else if (Waypoints.Count > 0)
		{
			// Tween to next waypoint
			Data.TPoint nextWaypoint = Waypoints.Pop();
			gameObject.transform.DOMove(
				nextWaypoint.grid.GetPos(), 
				nextWaypoint.duration);
            if (footstepSounds != null && footstepSounds.Count > 0)
                AudioManager.Instance.PlayAudio(footstepSounds[UnityEngine.Random.Range(0, footstepSounds.Count)]);
            else
                AudioManager.Instance.PlayAudio(null);
			WayTimer = nextWaypoint.duration + nextWaypoint.time;

			// Check overwatch every step
			GameLogicMP.instance.OverwatchCheck(nextWaypoint.grid, this);
		}
	}

    public void UpdatePos(float x, float z)
	{
		gameObject.transform.position = new Vector3(x, 0.0f, z);
	}

	public void UpdateSXZ(int x, int z)
	{
		SX = x;
		SZ = z;
	}
}
