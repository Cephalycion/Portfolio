using UnityEngine.Audio;
using UnityEngine;
using System;
using DG.Tweening;

public class AudioManager : MonoBehaviour
{
	public static AudioManager instance;
	public Sound[] sounds;

	private void Awake()
	{
		if (instance == null)
		{
			instance = this;
		}
		else
		{
			Debug.LogWarning("Multiple AudioManager instance.");
			return;
		}

		foreach(Sound s in sounds)
		{
			s.source = gameObject.AddComponent<AudioSource>();
			s.source.clip = s.clip;
			s.source.volume = s.volume * DataManager.instance.volume;
		}
	}

	public void updateVolume()
	{
		foreach (Sound s in sounds)
		{
			s.source.volume = s.volume * DataManager.instance.volume;
		}
	}

	// Overloaded function for buttons
	public void play(string name)
	{
		Sound s = Array.Find(sounds, sound => sound.name == name);
		if (s == null)
		{
			Debug.LogWarning("Sound not found: " + name);
			return;
		}
		s.source.loop = false;
		s.source.Play();
	}

	public void play(string name, bool loop)
	{
		Sound s = Array.Find(sounds, sound => sound.name == name);
		if (s == null)
		{
			Debug.LogWarning("Sound not found: " + name);
			return;
		}
		s.source.loop = loop;
		s.source.Play();
	}

	public void stop(string name)
	{
		Sound s = Array.Find(sounds, sound => sound.name == name);
		if (s == null)
		{
			Debug.LogWarning("Sound not found: " + name);
			return;
		}
		s.source.Stop();
	}

	public void pitchAll(float value)
	{
		for (int i = 0; i < sounds.Length; ++i)
		{
			AudioSource a = sounds[i].source;
			DOTween.To(() => a.pitch, x => a.pitch = x, value, 0.2f);
		}
	}

	public void stopFade(string name)
	{
		// Fade out BGM
		Sound s = Array.Find(sounds, sound => sound.name == name);
		if (s == null)
		{
			Debug.LogWarning("Sound not found: " + name);
			return;
		}
		AudioSource a = s.source;
		DOTween.To(() => a.volume, x => a.volume = x, 0.0f, 1.0f)
		.OnComplete(() => { a.Stop(); });

		// Mute all other sounds
		for (int i = 0; i < sounds.Length; ++i)
		{
			// Except BGM and Select (Yes I know a little hard-code-ish but it works)
			if (sounds[i].name != s.name &&
				sounds[i].name != "Select")
			{
				sounds[i].source.volume = 0.0f;
			}
		}
	}
}
