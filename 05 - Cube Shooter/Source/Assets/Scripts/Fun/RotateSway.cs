using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using DG.Tweening;

public class RotateSway : MonoBehaviour
{
	public float duration = 0.0f;
	public float limitL = 0.0f;
	public float limitR = 0.0f;
	public Ease ease = Ease.Flash;

    // Update is called once per frame
    private void Start()
    {
		rotL();
    }

	private void rotL()
	{
		gameObject.transform.DORotate(new Vector3(0.0f, 0.0f, limitL), duration)
			.SetEase(ease)
			.OnComplete(rotR);
	}

	private void rotR()
	{
		gameObject.transform.DORotate(new Vector3(0.0f, 0.0f, limitR), duration)
			.SetEase(ease)
			.OnComplete(rotL);
	}

	private void OnDestroy()
	{
		DOTween.Kill(gameObject.transform);
	}
}
