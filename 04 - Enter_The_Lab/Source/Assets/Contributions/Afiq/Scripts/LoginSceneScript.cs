using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using PlayFab;
using PlayFab.ClientModels;
using TMPro;

public class LoginSceneScript : MonoBehaviour
{
    public GameObject LoginDetailScreen;
    public GameObject LoginRoot;
    public GameObject MenuGame;
    public GameObject RegisterDetailScreen;
    public GameObject ChoicesScreen;
    public InputField[] email;
    public InputField[] password;
    public InputField PlayerName;
    public MainMenuScene mms;
    public TextMeshProUGUI[] errorBoxes;

    // Start is called before the first frame update
    void Start()
    {
        if (SceneConstant.returnFromGame)
        {
            LoginRoot.SetActive(false);
            MenuGame.SetActive(true);
            SceneConstant.returnFromGame = false;
        }
    }

    // Update is called once per frame
    void Update()
    {

    }

    void LoginSuccess(LoginResult _result)
    {
        PlayFabClient.GetInstance().isReady = true;
        SceneConstant.username = _result.InfoResultPayload.AccountInfo.Username;
        Debug.Log("Logged in");
        LoginDetailScreen.SetActive(false);
        RegisterDetailScreen.SetActive(false);
        ChoicesScreen.SetActive(false);
        LoginRoot.SetActive(false);
        MenuGame.SetActive(true);
        //EnterButton.SetActive(true);
    }

   void RegisterSuccess(RegisterPlayFabUserResult _result)
    {
        Debug.Log("Registered");
        SceneConstant.username = _result.Username;
        CloseChoice();
    }

    public void setErrorMessage(PlayFabError error,int index)
    {
        errorBoxes[index].GetComponent<CanvasRenderer>().SetAlpha(0);
        errorBoxes[index].CrossFadeAlpha(1.0f,0.5f,false);
        string vstring = "";
        Debug.Log(error.ErrorMessage);
        if (error.ErrorDetails == null)
        {
            vstring = error.ErrorMessage;
        }
        else
        {
            foreach (var item in error.ErrorDetails)
            {
                if (item.Key != "")
                    vstring += item.Value[0] + "\n";
            }
        }
        errorBoxes[index].text = vstring;
    }

    public void LoginButtonPressed()
    {
        //Debug.Log(email[0].text);
        PlayFabClient.GetInstance().EmailLogin(email[0].text, password[0].text, LoginSuccess, error => setErrorMessage(error,0));
    }

    public void RegisterButtonPressed()
    {
        if(PlayerName.text == "Afiq")
        {
            PlayFabClient.GetInstance().Login("Afiq", LoginSuccess, error => Debug.LogError(error.GenerateErrorReport()));
        }
        else
        {
            Debug.Log(email[1].text);
            Debug.Log(password[1].text);
            Debug.Log(PlayerName.text);
            PlayFabClient.GetInstance().EmailRegister(email[1].text, password[1].text, PlayerName.text, RegisterSuccess, error => setErrorMessage(error, 1));
        }

    }

    public void ChoiceLogin()
    {
        ChoicesScreen.SetActive(false);
        LoginDetailScreen.SetActive(true);
    }

    public void ChoiceRegister()
    {
        ChoicesScreen.SetActive(false);
        RegisterDetailScreen.SetActive(true);
    }

    public void CloseChoice()
    {
        LoginDetailScreen.SetActive(false);
        RegisterDetailScreen.SetActive(false);
        ChoicesScreen.SetActive(true);
        foreach (var item in errorBoxes)
        {
            item.text = "";
        }
    }
}
