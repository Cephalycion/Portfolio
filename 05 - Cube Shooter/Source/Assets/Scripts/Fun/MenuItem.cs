using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class MenuItem : MonoBehaviour
{
	public GameObject costDisplay;
	public GameObject selector;
	public Button button;
	public Button infoButton;
	public UPGRADE type;
	public int cost;
	public string description;

	private void Awake()
	{
		costDisplay.GetComponent<TMPro.TextMeshProUGUI>().text = cost.ToString();
	}
}
