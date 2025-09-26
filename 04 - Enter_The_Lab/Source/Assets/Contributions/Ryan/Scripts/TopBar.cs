using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using DG.Tweening;
using TMPro;

public class TopBar : MonoBehaviour
{
	public GameObject Turn;
	public GameObject Timer;
	public TextMeshProUGUI playerA;
	public TextMeshProUGUI playerB;

	public string m_UsernameA = "<UsernameA>";
	public string m_UsernameB = "<UsernameB>";

	private int prevTime = 0;
	private int currTime = 0;

	public string UsernameA
	{
		get { return m_UsernameA; }
		set { m_UsernameA = value; }
	}

	public string UsernameB
	{
		get { return m_UsernameB; }
		set { m_UsernameB = value; }
	}

	public void SetTurnA()
	{
		Turn.GetComponent<TMPro.TextMeshProUGUI>().text = m_UsernameA + "'s Turn";
	}

	public void SetTurnB()
	{
		Turn.GetComponent<TMPro.TextMeshProUGUI>().text = m_UsernameB + "'s Turn";
	}

	public void UpdateTimer(float time)
	{
		currTime = (int)time;
		if (currTime != prevTime)
		{
			prevTime = currTime;
			Timer.GetComponent<TMPro.TextMeshProUGUI>().text = "" + currTime;
		}
	}

	public void UpdateDiplomaA(int score)
	{
		playerA.text = score.ToString();
	}

	public void UpdateDiplomaB(int score)
	{
		playerB.text = score.ToString();
	}
}
