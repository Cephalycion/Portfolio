using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class HostJoin : MonoBehaviour
{
    public GameObject containerRoot;
    public GameObject CanvasRoot;
    public GameObject GameCanvasRoot;
    public TMPro.TMP_InputField IPContainerInput;
    public SceneTransit transitor;

    public bool ContainsAny(string haystack, params string[] needles)
    {
        foreach (string needle in needles)
        {
            if (haystack.Contains(needle))
                return true;
        }

        return false;
    }
    public void IPContainerOnEdit()
    {
        if (IPContainerInput.text.Length > 0)
        {
            int asciiidx = IPContainerInput.text[IPContainerInput.text.Length - 1];
            if (!((asciiidx >= 48 && asciiidx <= 57) || asciiidx == 46))
            {
                IPContainerInput.text = IPContainerInput.text.Substring(0, IPContainerInput.text.Length - 1);
            }
        }
    }

    public void HostInput()
    {
        HostGame();
    }

    public void JoinInput()
    {
        if (ContainsAny(IPContainerInput.text,"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"))
        {
            return;
        }
        JoinGame(IPContainerInput.text);
    }

    public void HostGame()
    {
        SceneConstant.PlaySelection = Inventory.InvItems;
        SceneConstant.creationSide = MPSide.MP_SERVERALONE;
        SceneConstant.ipAddress = "0.0.0.0";
        SceneConstant.port = 25565;
        transitor.ChangeScene("Game");
    }

    public void ReturnToMenu()
    {
        int c = Inventory.HeldItems.Count;
        for (int i = 0; i < c; i++)
        {
            Inventory.HeldToInv(0);
            BeardedManStudios.Forge.Logging.BMSLog.Log("D: " + Inventory.HeldItems.Count.ToString());
        }
        SceneConstant.returnFromGame = true;
        transitor.ChangeScene("Start");
    }

    public void JoinGame(string ipAddress = "127.0.0.1", int port = 25565)
    {
        SceneConstant.PlaySelection = Inventory.InvItems;
        SceneConstant.creationSide = MPSide.MP_CLIENTA;
        SceneConstant.ipAddress = ipAddress;
        SceneConstant.port = port;
        transitor.ChangeScene("Game");
    }

    public void OpenJoinUI()
    {
        containerRoot.SetActive(true);
        CanvasRoot.SetActive(false);
        GameCanvasRoot.SetActive(false);
    }
    public void CloseJoinUI()
    {
        containerRoot.SetActive(false);
        CanvasRoot.SetActive(true);
        GameCanvasRoot.SetActive(true);
    }
}

