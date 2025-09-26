using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class PopupManager : MonoBehaviour
{
	public static PopupManager instance = null;
	public GameObject holder = null;
	public TMPro.TextMeshProUGUI holderCost;
	public TMPro.TextMeshProUGUI holderDesc;

	public GameObject info = null;
	public TMPro.TextMeshProUGUI infoDesc;

	public PlayFabLinker playFabLinker = null;

	private void Awake()
	{
		if (instance == null)
		{
			instance = this;
		}
		else
		{
			Debug.LogWarning("Multiple AreYouSure instance.");
			Destroy(gameObject);
			return;
		}
	}

	public void setLinker(int stars)
	{
		playFabLinker.gameObject.SetActive(true);
		playFabLinker.stars = stars;
	}

	public void setPopup(int c, string d)
	{
		holder.SetActive(true);
		holderCost.text = c.ToString("0");
		holderDesc.text = d;
	}

	public void setInfo(string d)
	{
		info.SetActive(true);
		infoDesc.text = d;
	}
}
