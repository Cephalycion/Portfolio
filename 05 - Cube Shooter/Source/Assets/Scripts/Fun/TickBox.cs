using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TickBox : MonoBehaviour
{
	public GameObject fill;
	public bool status = true;

	public void toggle()
	{
		status = !status;
		updateStatus();
	}

	public void updateStatus()
	{
		fill.SetActive(!status);
	}
}
