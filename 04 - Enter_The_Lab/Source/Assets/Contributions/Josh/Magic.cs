using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class Magic : MonoBehaviour
{
    public bool console = false;
    public string t = "";
    public GameObject credits;
    public GameObject Instructions;
    private void OnGUI()
    {
        if (console)
        {
            t = GUI.TextField(new Rect(10, 10, 500, 20), t);
            if(GUI.Button(new Rect(10, 30, 100,30), "Execute"))
            {
                if (t == "cowlevel")
                {
                    SceneManager.LoadScene("LevelEditor");
                }
                else if (t == "credits")
                {
                    credits.SetActive(!credits.activeInHierarchy);
                }
                else
                {
                    t = "Invalid command: \"cowlevel\",\"credits\"";
                }
            }
        }
    }

    private void FixedUpdate()
    {
        if (Input.GetKeyDown("`"))
            console = !console;
    }

    public void ShowInstructions()
    {
        Instructions.SetActive(!Instructions.activeInHierarchy);
    }
}
