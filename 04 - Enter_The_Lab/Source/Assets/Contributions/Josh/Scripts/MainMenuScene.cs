using UnityEngine;
using System.Collections;
using TMPro;

public class MainMenuScene : MonoBehaviour
{
    public TextMeshProUGUI WelcomeText;
    private void OnEnable()
    {
        WelcomeText.text = "Welcome: " + SceneConstant.username;
    }
    public void SetText()
    {
        WelcomeText.text = "Welcome: " + SceneConstant.username;
    }
}
