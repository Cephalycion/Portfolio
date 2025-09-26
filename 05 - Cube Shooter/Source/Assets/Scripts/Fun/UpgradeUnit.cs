using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class UpgradeUnit : MonoBehaviour
{
	public Image fill = null;

	public void setTrue()
	{
		Color temp = fill.color;
		temp.a = 0.0f;
		fill.color = temp;
	}

	public void setFalse()
	{
		Color temp = fill.color;
		temp.a = 255.0f;
		fill.color = temp;
	}
}
