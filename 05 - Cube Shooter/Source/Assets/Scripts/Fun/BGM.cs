using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BGM : MonoBehaviour
{
	public string music;

    // Start is called before the first frame update
    void Start()
    {
		AudioManager.instance.play(music, true);
    }
}
