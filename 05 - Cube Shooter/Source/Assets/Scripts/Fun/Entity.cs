using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Entity : MonoBehaviour
{
	public bool isAlive;

	public virtual void takeDamage(float damage)
	{
		;
	}

	public virtual void gainHealth(float gain)
	{
		;
	}

	public virtual void shine()
	{
		Shine shine = Instantiate(DataManager.instance.shine, gameObject.transform).GetComponent<Shine>();
		shine.rt.sizeDelta = GetComponent<RectTransform>().sizeDelta;
	}
}
