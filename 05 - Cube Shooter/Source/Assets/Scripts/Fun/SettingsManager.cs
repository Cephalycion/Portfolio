using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class SettingsManager : MonoBehaviour
{
	public Slider volumeSlider;
	public TickBox bloom;
	public TickBox vignette;

	private void Start()
	{
		volumeSlider.value = DataManager.instance.volume;

		bloom.status = DataManager.instance.bloom;
		bloom.updateStatus();

		vignette.status = DataManager.instance.vignette;
		vignette.updateStatus();
	}

	public void deltaVolume(float input)
	{
		DataManager.instance.volume = input;
		PlayerPrefs.SetFloat("volume", DataManager.instance.volume);
		PlayerPrefs.Save();

		AudioManager.instance.updateVolume();
	}

	public void deltaBloom()
	{
		DataManager.instance.bloom = bloom.status;
		PlayerPrefs.SetInt("bloom", DataManager.instance.bloom ? 1 : 0);
		PlayerPrefs.Save();

		PPManager.instance.updatePostProcessing();
	}

	public void deltaVignette()
	{
		DataManager.instance.vignette = vignette.status;
		PlayerPrefs.SetInt("vignette", DataManager.instance.vignette ? 1 : 0);
		PlayerPrefs.Save();

		PPManager.instance.updatePostProcessing();
	}
}
