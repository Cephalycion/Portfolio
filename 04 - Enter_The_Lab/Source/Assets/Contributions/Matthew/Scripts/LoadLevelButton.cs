using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class LoadLevelButton : MonoBehaviour
{
    public Text textReference;
    // Start is called before the first frame update
    void Start()
    {
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void Clicked()
    {
        EditorLogic.Instance.Load(textReference.text);
        EditorLogic.Instance.LoadLevelPrompt.SetActive(false);
    }
}
