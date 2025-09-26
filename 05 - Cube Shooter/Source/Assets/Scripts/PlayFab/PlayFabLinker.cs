using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class PlayFabLinker : MonoBehaviour
{
	public TMPro.TMP_InputField nameInput;
	public Button submit;
	public int stars;

	private void Awake()
	{
		submit.onClick.AddListener(() => submitToManager());
	}

	private void submitToManager()
	{
		PlayFabManager.instance.SubmitLogin(nameInput.text, stars);
	}
}
