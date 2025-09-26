using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class SceneTransit : MonoBehaviour
{
    public AudioClip SceneTransitionSound;
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void ChangeScene(string _sceneName)
    {
        AudioManager.Instance.PlayAudio(SceneTransitionSound);
        SceneManager.LoadScene(_sceneName);
        //nextScene = _sceneName;
        //changing = true;

    }
}
