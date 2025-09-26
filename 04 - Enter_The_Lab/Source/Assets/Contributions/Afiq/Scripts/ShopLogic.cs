using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using PlayFab;
using PlayFab.ClientModels;

public class ShopLogic : MonoBehaviour
{
    public Text tunaText;
    private float BounceTime;
    private bool buyPhase = false;
    private List<int> costs = new List<int>();
    private List<string> names = new List<string>();
    //public GameObject confirmUI;
    //public Text itemName;

    int tunaAmount = 0;
    string shopItemName;
    int shopCost;

    #region UI
    //public Button BuyTunaButton;
    //public Button BuyItemButton;
    public GameObject[] confirmPanels;
    public GameObject BuyOptionsPanel;

    #endregion

    // Start is called before the first frame update
    void Start()
    {
        BounceTime = 0.02f;
        if (PlayFabClient.GetInstance().isReady == true)
        {
            //PlayFabClient.GetInstance().AddTuna(1, addSuccess, error => Debug.LogError(error.GenerateErrorReport()));
            PlayFabClient.GetInstance().GetItems(getTuna, error => Debug.LogError(error.GenerateErrorReport()));
            PlayFabClient.GetInstance().getCatalogItems(null, getPrices, error => Debug.LogError(error.GenerateErrorReport()));
        }
        else
        {
            PlayFabClient.GetInstance().Login("Afiq", loginSuccess, error => Debug.LogError(error.GenerateErrorReport()));
        }
        for (int i = 0; i < confirmPanels.Length; ++i)
        {
            confirmPanels[i].SetActive(false);

        }
        BuyOptionsPanel.SetActive(true);

    }

    // Update is called once per frame
    void Update()
    {
        BounceTime -= Time.deltaTime;
        if (BounceTime <= 0.0f && PlayFabClient.GetInstance().isReady)
        {
            PlayFabClient.GetInstance().GetItems(getTuna, error => Debug.LogError(error.GenerateErrorReport()));
            BounceTime = 5.0f;
        }
    }

    void loginSuccess(LoginResult _result)
    {
        PlayFabClient.GetInstance().isReady = true;
        Debug.Log("Logged in");

        //PlayFabClient.GetInstance().AddTuna(1, addSuccess, error => Debug.LogError(error.GenerateErrorReport()));
        PlayFabClient.GetInstance().GetItems(getTuna, error => Debug.LogError(error.GenerateErrorReport()));
        PlayFabClient.GetInstance().getCatalogItems(null, getPrices, error => Debug.LogError(error.GenerateErrorReport()));
    }

    void getTuna(GetUserInventoryResult _result)
    {
        tunaAmount = _result.VirtualCurrency["TB"];
        tunaText.text = tunaAmount.ToString();
    }

    void addSuccess(ModifyUserVirtualCurrencyResult _result)
    {
        Debug.Log("Tuna Added");
    }

    public void BuyItem(int panelIndex)
    {
        PlayFabClient.GetInstance().BuyItem(names[panelIndex], costs[panelIndex], "TB", BuySuccess, error => Debug.LogError(error.GenerateErrorReport()));
    }

    public void BuyTuna(int amount)
    {
        PlayFabClient.GetInstance().GetItems(getTuna, error => Debug.LogError(error.GenerateErrorReport()));
    }

    public void ItemPressed(int panelIndex)
    {
        if (costs[panelIndex] <= tunaAmount)
        {
            BuyOptionsPanel.SetActive(false);
            for (int i = 0; i < confirmPanels.Length; ++i)
            {
                confirmPanels[i].SetActive(false);
            }
            confirmPanels[panelIndex].SetActive(true);
        }
        else
        {

        }
    }

    public void CloseConfirmUI()
    {
        for (int i = 0; i < confirmPanels.Length; ++i)
        {
            confirmPanels[i].SetActive(false);
        }
        BuyOptionsPanel.SetActive(true);
    }

    void BuySuccess(PurchaseItemResult _result)
    {
        Debug.Log("Bought");
        CloseConfirmUI();
    }

    void getPrices(GetCatalogItemsResult _result)
    {
        for (int i = 0; i < _result.Catalog.Count; ++i)
        {
            costs.Add((int)_result.Catalog[i].VirtualCurrencyPrices["TB"]);
            names.Add(_result.Catalog[i].ItemId);
        }
    }

    public void AddTuna()
    {
        PlayFabClient.GetInstance().AddTuna(1, addSuccess, error => Debug.LogError(error.GenerateErrorReport()));
    }
}
