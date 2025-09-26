using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Line : MonoBehaviour
{
	public float height = 5.0f;

	public void construct(Vector3 start, Vector3 end)
	{
		gameObject.transform.position = start;
		Vector3 dir = end - start;
		float angle = Mathf.Atan2(dir.y, dir.x) * Mathf.Rad2Deg;
		RectTransform rect = gameObject.GetComponent<RectTransform>();
		rect.sizeDelta = new Vector2(dir.magnitude, height);
		rect.eulerAngles = new Vector3(0.0f, 0.0f, angle);
	}

	public void destruct()
	{
		Destroy(gameObject);
	}
}
