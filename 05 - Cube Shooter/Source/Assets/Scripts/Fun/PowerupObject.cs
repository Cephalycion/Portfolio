using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PowerupObject : MonoBehaviour
{
	public POWERUP type = POWERUP.NONE;
	public float duration = -1.0f;

	private void OnTriggerEnter2D(Collider2D collision)
	{
		// If collided with powerup
		if (collision.gameObject == Player.instance.gameObject)
		{
			AudioManager.instance.play("Powerup");
			Player.instance.activePowerup.type = type;
			Player.instance.activePowerup.duration = duration;
			Player.instance.UI_Powerup_Type.gameObject.SetActive(true);
			switch (Player.instance.activePowerup.type)
			{
				case POWERUP.INF_HEALTH:
					{
						Player.instance.UI_Powerup_Type.text = "INVULERABLE";
						Player.instance.UI_Powerup_Type.color = new Color(1, 0, 1);
						Player.instance.UI_Powerup_Duration.color = new Color(1, 0, 1);
						break;
					}
				case POWERUP.INF_AMMO:
					{
						Player.instance.UI_Powerup_Type.text = "INFINITE AMMO";
						Player.instance.UI_Powerup_Type.color = new Color(1, 1, 0);
						Player.instance.UI_Powerup_Duration.color = new Color(1, 1, 0);
						Player.instance.weapons.refillActiveWeapon();
						break;
					}
				case POWERUP.INF_FUEL:
					{
						Player.instance.UI_Powerup_Type.text = "INFINITE FUEL";
						Player.instance.UI_Powerup_Type.color = new Color(0, 1, 1);
						Player.instance.UI_Powerup_Duration.color = new Color(0, 1, 1);
						Player.instance.refillFuel();
						break;
					}
			}
			Destroy(gameObject);
		}
	}
}
