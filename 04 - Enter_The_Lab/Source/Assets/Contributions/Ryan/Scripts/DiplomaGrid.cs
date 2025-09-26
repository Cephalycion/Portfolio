using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DiplomaGrid : MonoBehaviour
{
	public int m_DX;
	public int m_DZ;

	public GameObject W1;
	public GameObject W2;

	public void SetW1()
	{
		W1.SetActive(true);
		W2.SetActive(false);
	}

	public void SetW2()
	{
		W1.SetActive(false);
		W2.SetActive(true);
	}

	public int DX
	{
		get { return m_DX; }
		set { m_DX = value; }
	}

	public int DZ
	{
		get { return m_DZ; }
		set { m_DZ = value; }
	}
}
