using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AudioObject : MonoBehaviour
{
    public bool Fade { get; set; } = false;
    public float FadeTime = 0.0f;
    // Start is called before the first frame update
    void Start()
    {
        _audioSource = gameObject.GetComponent<AudioSource>();
        _clipLength = _audioSource.clip.length;
    }

    private AudioSource _audioSource;
    private float _clipLength;
    // Update is called once per frame
    void FixedUpdate()
    {
        if(Fade)
        {
            _audioSource.volume = Mathf.Min((_clipLength - _audioSource.time) / FadeTime, 1.0f);
        }
    }
}
