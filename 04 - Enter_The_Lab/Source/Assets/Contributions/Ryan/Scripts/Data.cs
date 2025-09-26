using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Data : MonoBehaviour
{
	public struct TPoint
	{
		public GridNode grid;
		public float duration;
		public float time;

		public TPoint(GridNode g, float d, float t)
		{
			grid = g;
			duration = d;
			time = t;
		}
	};
}
