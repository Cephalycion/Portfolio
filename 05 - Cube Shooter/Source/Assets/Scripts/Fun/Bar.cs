using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Bar : MonoBehaviour
{
	public RectTransform rt;
	public Slider slider;
	public Image fill;

    public void changeLength(float length)
	{
		rt.sizeDelta = new Vector2(length, rt.rect.width);
	}

	public void changeColor(Color color)
	{
		fill.color = color;
	}

	public void setMax(float max)
	{
		slider.maxValue = max;
		slider.value = max;
	}

	public void setValue(float value)
	{
		slider.value = value;
	}
}
