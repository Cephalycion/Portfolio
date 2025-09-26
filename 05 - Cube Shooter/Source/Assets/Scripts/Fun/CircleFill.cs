using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CircleFill : MonoBehaviour
{
	public GameObject fill;

	public void setFill()
	{
		fill.SetActive(true);
	}

	public void setUnfill()
	{
		fill.SetActive(false);
	}
}
