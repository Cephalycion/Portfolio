using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class SceneLoader : MonoBehaviour
{
	public static SceneLoader instance = null;

	public Animator transition;
	public float transitionTime = 1.0f;
	private bool loading = false;

	private void Awake()
	{
		if (instance == null)
		{
			instance = this;
		}
		else
		{
			Debug.LogWarning("Multiple SceneLoader instance.");
			return;
		}
	}

	public void LoadScene(string name)
	{
		if (loading == false)
		{
			loading = true;
			StartCoroutine(LoadSceneRoutine(name));
		}
	}

	private IEnumerator LoadSceneRoutine(string name)
	{
		transition.SetTrigger("Start");
		yield return WaitForRealSeconds(transitionTime);
		SceneManager.LoadScene(name);
		Time.timeScale = 1.0f;
	}

	public IEnumerator WaitForRealSeconds(float time)
	{
		float start = Time.realtimeSinceStartup;
		while (Time.realtimeSinceStartup < start + time)
		{
			yield return null;
		}
	}
}
