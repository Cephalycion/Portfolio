using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LoadingScreen : MonoBehaviour
{
    public float loadingTimer = 5.0f;
    public AudioClip LoadingMusic;
    // Start is called before the first frame update
    void Start()
    {
        AudioManager.Instance.PlayAudio(LoadingMusic);
    }

    // Update is called once per frame
    void Update()
    {
        loadingTimer -= Time.deltaTime;
        if (loadingTimer <= 0.0f)
        {
            Initiate.Fade("Start", new Color(0.0f, 0.0f, 0.0f), 1.0f);
        }
    }
}
