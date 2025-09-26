using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class WinManager : MonoBehaviour
{
	public GameObject MainDisplay;
	public GameObject UsernameA;
	public GameObject UsernameB;
	public GameObject EarningsA;
	public GameObject EarningsB;
	public GameObject Countdown;

	public float StartTimer;
	private float counter;

	public void SetWinner(string username)
	{
		MainDisplay.GetComponent<Text>().text = username + " Win!";
	}

	public void SetData(bool team, string username, int earnings)
	{
		if (team == true)
		{
			UsernameA.GetComponent<Text>().text = username;
			EarningsA.GetComponent<Text>().text = "+" + earnings + "TB";
		}
		else
		{
			UsernameB.GetComponent<Text>().text = username;
			EarningsB.GetComponent<Text>().text = "+" + earnings + "TB";
		}
	}

	void Start()
	{
		counter = StartTimer;
	}

	void FixedUpdate()
	{
		counter -= Time.fixedDeltaTime;
		Countdown.GetComponent<Text>().text = "Returning to Menu in " + counter;

		if (counter <= 0.0f)
		{
			SceneManager.LoadScene("Start");
		}
	}
}
