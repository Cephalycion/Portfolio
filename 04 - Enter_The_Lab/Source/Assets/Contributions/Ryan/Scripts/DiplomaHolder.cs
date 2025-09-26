using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DiplomaHolder : MonoBehaviour
{
	public GameObject DiplomaGrid;
	public float offset;
	private GameObject CanvasReference;

	[HideInInspector]
	public List<GridNode> Area = new List<GridNode>();
	private Dictionary<int, DiplomaGrid> Display = new Dictionary<int, DiplomaGrid>();

	public void Init(List<GridNode>Input, GameObject Canvas)
	{
		Area = Input;
		CanvasReference = Canvas;

		for (int i = 0; i < Area.Count; ++i)
		{
			GridNode Temp = Area[i];
			GameObject NewObject = Instantiate(DiplomaGrid, CanvasReference.transform);
			NewObject.transform.position = Temp.gameObject.transform.position + new Vector3(0, offset, 0);
			DiplomaGrid NewGrid = NewObject.GetComponent<DiplomaGrid>();
			NewGrid.DX = Temp.GX;
			NewGrid.DZ = Temp.GZ;
			NewGrid.SetW1();
			Display[i] = NewGrid;
		}
	}

    // Update is called once per frame
    void Update()
    {
        for (int i = 0; i < Area.Count; ++i)
		{
			if (Area[i].student != null)
			{
				Display[i].SetW2();
			}
			else
			{
				Display[i].SetW1();
			}
		}
    }
}
