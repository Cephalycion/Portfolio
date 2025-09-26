using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using DG.Tweening;

public class StudentStats : MonoBehaviour
{
	// Append to student
	[HideInInspector]
	public GameObject studentObj;
	public float height;

	// Scaling
	public GameObject Health_BG;
	public GameObject Health_Max;
	public GameObject Health_Curr;
	public GameObject Moves_BG;
	public GameObject Moves_Max;
	public GameObject Moves_Curr;
	public GameObject State_Overwatch;
	public GameObject State_Hunker;

	private Camera maincam;
	private Student student;

	// Keep track of previous stats
	private int prevHealth;
	private int prevMoves;

	public GameObject TextType;
	public GameObject Select;

	// Start is called before the first frame update
	public void Init()
	{
		maincam = GameObject.Find("Main Camera").GetComponent<Camera>();
		gameObject.transform.SetAsFirstSibling();
		student = studentObj.GetComponent<Student>();

		// Health_BG.transform.DOScaleX(student.health + 0.3f, 1.0f);
		// Health_Max.transform.DOScaleX(student.health, 1.0f);
		// Moves_BG.transform.DOScaleX(student.moves + 0.3f, 1.0f);
		// Moves_Max.transform.DOScaleX(student.moves, 1.0f);

		prevHealth = student.currHealth;
		prevMoves = student.currMoves;

		switch (student.studentType)
		{
			case Student.SType.AFIQ:
				{
					SetTextType("A");
					break;
				}
			case Student.SType.SPY:
				{
					SetTextType("S");
					break;
				}
			case Student.SType.RISTELLISE:
				{
					SetTextType("R");
					break;
				}
			case Student.SType.MBRINE:
				{
					SetTextType("M");
					break;
				}
			default:
				{
					SetTextType("Z");
					break;
				}
		}
	}

	void SetTextType(string t)
	{
		TextType.GetComponent<TMPro.TextMeshProUGUI>().text = t;
	}

	// Update is called once per frame
	void FixedUpdate()
	{
		if (student == null)
		{
			Destroy(gameObject);
			return;
		}
		Vector3 pos = maincam.WorldToScreenPoint(student.transform.position);
		gameObject.transform.position = new Vector3(pos.x, pos.y + height, pos.z);

		if (prevHealth != student.currHealth)
		{
			Health_Curr.transform.DOScaleX((float)student.currHealth / (float)student.health, 0.5f);
			prevHealth = student.currHealth;
		}
		
		if (prevMoves != student.currMoves)
		{
			Moves_Curr.transform.DOScaleX((float)student.currMoves / (float)student.moves, 0.5f);
			prevMoves = student.currMoves;
		}

		if (student.sm != null)
		{
			if (student.sm.GetCurrentState() == "Overwatch")
			{
				State_Overwatch.SetActive(true);
				State_Hunker.SetActive(false);
			}
			else if (student.sm.GetCurrentState() == "Hunker")
			{
				State_Overwatch.SetActive(false);
				State_Hunker.SetActive(true);
			}
			else
			{
				State_Overwatch.SetActive(false);
				State_Hunker.SetActive(false);
			}
		}

		if (GameLogicMP.instance.Selected == student)
		{
			Select.SetActive(true);
		}
		else
		{
			Select.SetActive(false);
		}
	}

	public void SetHealthColor(Color color)
	{
		Health_Curr.GetComponentInChildren<Image>().color = color;
	}
}
