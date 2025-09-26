using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Jeticle : MonoBehaviour
{
	public ParticleSystem jet;
	public Vector2 direction;
	private Vector2 defaultDir;

	private void Start()
	{
		defaultDir = direction;
	}

	public void setRotation(float degrees)
	{
		float radians = degrees * Mathf.Deg2Rad;
		float sin = Mathf.Sin(radians);
		float cos = Mathf.Cos(radians);

		float tx = defaultDir.x;
		float ty = defaultDir.y;

		direction = new Vector2(cos * tx - sin * ty, sin * tx + cos * ty).normalized;
	}

	public void play(float percentage)
	{
		if (percentage == 0.0f && jet.isPlaying)
		{
			jet.Stop();
			return;
		}

		var main = jet.main;
		main.startSpeed = 500 * percentage;
		jet.Play();
	}
}
