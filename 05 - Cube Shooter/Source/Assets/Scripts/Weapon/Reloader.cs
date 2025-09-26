using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Reloader : MonoBehaviour
{
	public static Reloader instance = null;
	public GameObject circlePrefab = null;
	public GameObject barPrefab = null;
	public GameObject reloading = null;

	private List<CircleFill> circleFills = new List<CircleFill>();
	private Bar reloadBar = null;

	private void Awake()
	{
		if (instance == null)
		{
			instance = this;
		}
		else
		{
			Debug.LogWarning("Multiple Reloader instance.");
			return;
		}
	}

	private void LateUpdate()
	{
		gameObject.transform.position = Player.instance.position;
	}

	#region BAR TYPE
	public void constructBar(int maxAmmo)
	{
		reloadBar = Instantiate(barPrefab, gameObject.transform).GetComponent<Bar>();
		reloadBar.setMax(maxAmmo);
	}

	public void b_setAmmo(int ammo)
	{
		reloadBar.setValue(ammo);
	}
	#endregion

	#region CIRCLE TYPE
	public void constructCircle(Vector3 pos)
	{
		GameObject temp = Instantiate(circlePrefab, gameObject.transform);
		temp.transform.localPosition = pos;
		CircleFill cf = temp.GetComponent<CircleFill>();
		circleFills.Add(cf);
	}

	public void c_fill(int idx)
	{
		circleFills[idx].setFill();
	}

	public void c_unfill(int idx)
	{
		circleFills[idx].setUnfill();
	}
	#endregion
}
