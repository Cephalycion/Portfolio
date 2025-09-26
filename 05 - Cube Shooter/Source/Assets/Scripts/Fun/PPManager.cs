using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Rendering.PostProcessing;

public class PPManager : MonoBehaviour
{
	public static PPManager instance;
	public PostProcessVolume PPV;

	private Bloom bloom = null;
	private Vignette vignette = null;

	private void Awake()
	{
		if (instance == null)
		{
			instance = this;
		}
		else
		{
			Debug.LogWarning("Multiple PPManager instance.");
			return;
		}
	}

	private void Start()
	{
		PPV.profile.TryGetSettings(out bloom);
		PPV.profile.TryGetSettings(out vignette);
		updatePostProcessing();
	}

	public void updatePostProcessing()
	{
		bloom.enabled.value = DataManager.instance.bloom;
		vignette.enabled.value = DataManager.instance.vignette;
	}
}
