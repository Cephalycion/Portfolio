using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class SettingsMenu : MonoBehaviour
{
    public Slider VolumeSlider;
    public Slider SFXVolumeSlider;
    public Toggle DisableShaders;
    public Slider BloomSlider;

    public bool SettingsChanged { get; set; } = false;

    public void Save()
    {
       // if(SettingsChanged)
            StartCoroutine(waiter());
        Settings.Instance.Save();
       // yield new WaitForSeconds(5);
    }


    IEnumerator waiter()
    {
        yield return new WaitForSecondsRealtime(5);
        //Wait for 5 seconds
    }
private void Start()
    {
        Settings.Instance.Load();
        VolumeSlider.value = Settings.Instance.settings.Volume;
        SFXVolumeSlider.value = Settings.Instance.settings.SFXVolume;
        DisableShaders.isOn = Settings.Instance.settings.DisableAllShaders;
        BloomSlider.value = Settings.Instance.settings.BloomIntensity;
    }

    private void Update()
    {
        Settings.Instance.settings.Volume = VolumeSlider.value;
        Settings.Instance.settings.SFXVolume = SFXVolumeSlider.value;
        Settings.Instance.settings.DisableAllShaders = DisableShaders.isOn;
        Settings.Instance.settings.BloomIntensity = BloomSlider.value;
    }

    public void HideSettings()
    {

    }
}
