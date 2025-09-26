using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Directory : MonoBehaviour
{
	public TMPro.TextMeshProUGUI directory;

	private void Start()
	{
		directory.text = SaveSystem.path;
	}
}
