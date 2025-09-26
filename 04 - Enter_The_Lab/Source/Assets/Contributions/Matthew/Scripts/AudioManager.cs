using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AudioManager : MonoBehaviour
{
    private static AudioManager instance = null;
    public List<GameObject> Players;
    public AudioClip MissingAudioClip;
    private static AudioClip _tempClip;
    public AudioClip TempClip
    {
        get
        {
            return _tempClip;
        }
        set
        {
            _tempClip = value;
        }
    }
    public float TempVolume { get; set; } = 1.0f;
    public static AudioManager Instance
    {
        get
        {
            return instance;
        }
    }
    void Awake()
    {
        if (instance != null && instance != this)
        {
            Destroy(gameObject);
            return;
        }
        else
        {
            instance = this;
        }
        DontDestroyOnLoad(this.gameObject);
        BeardedManStudios.Forge.Logging.BMSLog.Log("Initted AudioManager");
    }
    public void PlayAudio(AudioClip Clip, float volume = 1.0f, bool persistent = true, bool repeat = false)
    {
        if(Clip == null)
        {
            if (MissingAudioClip == null)
                return;
            else
                PlayAudio(MissingAudioClip);
            return;
        }
        GameObject tempPlayer = new GameObject();
        if (persistent) 
        DontDestroyOnLoad(tempPlayer);
        tempPlayer.gameObject.name = "tempPlayer";
        AudioSource tempAS;
        Players.Add(tempPlayer);
        tempAS = tempPlayer.AddComponent<AudioSource>();
        tempPlayer.AddComponent<AudioObject>();
        tempAS.clip = Clip;
        tempAS.loop = repeat;
        tempAS.volume = volume * Settings.Instance.settings.Volume;
        tempAS.Play();
    }
    public void PlayAudio(float volume = 1.0f, bool persistent = true, bool repeat = false)
    {
        if (TempClip == null)
        {
            if (MissingAudioClip == null)
                return;
            else
                PlayAudio(MissingAudioClip);
            return;
        }
        else
        {
            PlayAudio(TempClip, volume, persistent, repeat);
        }
    }
    public void PlayAudio()
    {
        if (TempClip == null)
        {
            if (MissingAudioClip == null)
                return;
            else
                PlayAudio(MissingAudioClip);
            return;
        }
        else
        {
            PlayAudio(TempClip);
        }
    }
    public void PlayFadeAudio(AudioClip Clip, float FadeTime = 1.0f,float volume = 1.0f, bool persistent = true, bool repeat = false)
    {
        if (Clip == null)
        {
            if (MissingAudioClip == null)
                return;
            else
                PlayAudio(MissingAudioClip);
            return;
        }
        GameObject tempPlayer = new GameObject();
        if (persistent)
            DontDestroyOnLoad(tempPlayer);
        tempPlayer.gameObject.name = "tempPlayer";
        AudioSource tempAS;
        Players.Add(tempPlayer);
        tempAS = tempPlayer.AddComponent<AudioSource>();
        tempAS.clip = Clip;
        tempAS.loop = repeat;
        tempAS.volume = volume * Settings.Instance.settings.Volume;
        AudioObject tempAO = tempPlayer.AddComponent<AudioObject>();
        tempAO.FadeTime = FadeTime;
        tempAO.Fade = true;

        tempAS.Play();
    }
    public bool IsPlaying(AudioClip clip)
    {
        AudioSource tempAS;
        for (int i = 0; i < Players.Count; ++i)
        {
            if (!Players[i])
            {
                Players.Remove(Players[i]);
                --i;
                continue;
            }
            if (Players[i].GetComponent<AudioSource>())
            {
                if (Players[i].GetComponent<AudioSource>().clip == clip)
                {
                    tempAS = Players[i].GetComponent<AudioSource>();
                    if (tempAS.isPlaying)
                        return true;
                }
            }
        }
        return false;
    }

    private void Update()
    {
        for (int i = 0; i < Players.Count; ++i)
        {
            if (!Players[i])
            {
                Players.Remove(Players[i]);
                --i;
                continue;
            }
            if (!Players[i].GetComponent<AudioSource>())
            {
                Players.Remove(Players[i]);
                return;
            }
            else if (!Players[i].GetComponent<AudioSource>().isPlaying)
            {
                Destroy(Players[i]);
                Players.Remove(Players[i]);
                return;
            }
        }

    }
}
