using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerData
{
	public float health;
	public float shield;
	public float recharge;
	public float speed;

	public void construct(
		float _health, 
		float _shield, 
		float _recharge, 
		float _speed)
	{
		health = _health;
		shield = _shield;
		recharge = _recharge;
		speed = _speed;
	}
}
