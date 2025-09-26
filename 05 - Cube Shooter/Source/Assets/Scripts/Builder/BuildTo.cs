using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class BuildTo : MonoBehaviour
{
	public ButtonV2 button;
	public Image image;
	public GameObject remove;
	public GameObject trash;
	public int ID;

	[HideInInspector]
	public char letter;
	private GameObject chunk;
	private bool hovering = false;

	public void construct()
	{
		button.onClick.AddListener(() => onclick());
		button.onEnter.AddListener(() => onEnter());
		button.onExit.AddListener(() => onExit());
	}

	private void onclick()
	{
		if (chunk != null)
		{
			// remove.SetActive(true);
			// Destroy(chunk);
			chunk = null;
			Builder.instance.removeChunk(this);
			AudioManager.instance.play("Select");
		}
	}

	private void onEnter()
	{
		hovering = true;
	}

	private void onExit()
	{
		hovering = false;
		trash.SetActive(false);
	}

	private void Update()
	{
		if (hovering)
		{
			if (Builder.instance.holdingChunk)
			{
				if (Input.GetMouseButtonUp(0))
				{
					AudioManager.instance.play("BuildTo");
					setChunk();
					Builder.instance.saveLevel();
				}
			}
			else
			{
				if (chunk != null)
				{
					trash.SetActive(true);
				}
			}
		}
	}

	public void setChunk()
	{
		Destroy(chunk);
		remove.SetActive(false);
		chunk = Builder.instance.GenerateChunk(this);
		letter = Builder.instance.letterCur;
	}

	public void destruct()
	{
		button.onClick.RemoveListener(() => onclick());
		button.onEnter.RemoveListener(() => onEnter());
		button.onExit.RemoveListener(() => onExit());
	}
}
