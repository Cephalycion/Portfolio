using UnityEngine;
using System.Collections;
using PlayFab;
using PlayFab.ClientModels;

/// <summary>
/// PlayFabClient
/// This contains all the classes related to Playfab.
/// This is a C++ styled static class, meaning,
/// it should be treated as a singleton.
/// </summary>
public class PlayFabClient
{
    private bool ready = false;
    static PlayFabClient instance;
    private string pfID = "";

    /// <summary>
    /// Gets the Static/Global Instance
    /// </summary>
    /// <returns></returns>
    public static PlayFabClient GetInstance() {
        if (instance == null)
            instance = new PlayFabClient();

        return instance;
    }

    private PlayFabClient() { }

    /// <summary>
    /// Logs in the user.
    /// </summary>
    /// <param name="userID"></param>
    public void Login(
        string userID,
        System.Action<LoginResult> Callback,
        System.Action<PlayFabError> ErrorCallback)
    {
        PlayFabSettings.staticSettings.TitleId = "269F6";
        var request = new LoginWithCustomIDRequest { CustomId = userID, CreateAccount = true };
        PlayFabClientAPI.LoginWithCustomID(request, Callback, ErrorCallback);
    }

    public void EmailLogin(
        string UserEmail, 
        string UserPassword,
        System.Action<LoginResult> Callback,
        System.Action<PlayFabError> ErrorCallback
        )
    {
        PlayFabSettings.staticSettings.TitleId = "269F6";

        GetPlayerCombinedInfoRequestParams requestParams = new GetPlayerCombinedInfoRequestParams
        {
            GetUserAccountInfo = true
        };
        var request = new LoginWithEmailAddressRequest { Email = UserEmail, Password = UserPassword, InfoRequestParameters = requestParams };
        PlayFabClientAPI.LoginWithEmailAddress(request, Callback, ErrorCallback);
    }

    public void EmailRegister(
        string UserEmail,
        string UserPassword,
        string Name,
        System.Action<RegisterPlayFabUserResult> Callback,
        System.Action<PlayFabError> ErrorCallback
        )
    {
        Debug.Log(UserEmail + "|" + UserPassword + "|" + Name);
        var registerRequest = new RegisterPlayFabUserRequest { Email = UserEmail, Password = UserPassword, Username = Name };
        PlayFabClientAPI.RegisterPlayFabUser(registerRequest, Callback, ErrorCallback);
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="result"></param>
    private void OnLoginSuccess(LoginResult result)
    {
        BeardedManStudios.Forge.Logging.BMSLog.Log("Login Success.");
        pfID = result.PlayFabId;
        ready = true;
    }

    /// <summary>
    /// Buys an item with an ItemID.
    /// </summary>
    /// <param name="ItemID">ItemID of the item you're purchasing</param>
    /// <param name="customCallback">Callback to action result.</param>
    public void BuyItem(
        string ItemID, int Price,
        string Currency,
        System.Action<PurchaseItemResult> customCallback,
        System.Action<PlayFabError> ErrorCallback)
    {
        PurchaseItemRequest request = new PurchaseItemRequest { ItemId = ItemID, Price = Price, VirtualCurrency = Currency };
        PlayFabClientAPI.PurchaseItem(request, customCallback, ErrorCallback);
    }

    public void getCatalogItems(string catalogVer,
        System.Action<GetCatalogItemsResult> customCallback,
        System.Action<PlayFabError> ErrorCallback)
    {
        GetCatalogItemsRequest request = new GetCatalogItemsRequest { CatalogVersion = catalogVer };
        PlayFabClientAPI.GetCatalogItems(request, customCallback, ErrorCallback);
    }

    /// <summary>
    /// Get's the User/Player's ItemBox / Inventory.
    /// </summary>
    /// <param name="callback">A function that has a GetUserInventoryResult as its input param to recieve the stuff.</param>
    /// <param name="customCallback">Callback to action result.</param>
    public void GetItems(
        System.Action<GetUserInventoryResult> callback,
        System.Action<PlayFabError> ErrorCallback)
    {
        GetUserInventoryRequest request = new GetUserInventoryRequest { };
        PlayFabClientAPI.GetUserInventory(request, callback, ErrorCallback);
    }

    /// <summary>
    /// Adds TunaBites to the user's current Tuna collection.
    /// </summary>
    /// <param name="amount"></param>
    /// <param name="customCallback"></param>
    /// <param name="ErrorCallback"></param>
    /// <returns></returns>
    public bool AddTuna(
        int amount,
        System.Action<ModifyUserVirtualCurrencyResult> customCallback,
        System.Action<PlayFabError> ErrorCallback)
    {
        if (ready)
        {
            AddUserVirtualCurrencyRequest request = new AddUserVirtualCurrencyRequest { Amount = amount, VirtualCurrency = "TB" };
            PlayFabClientAPI.AddUserVirtualCurrency(request, customCallback, ErrorCallback);
            return true;
        }
        else
        {
            BeardedManStudios.Forge.Logging.BMSLog.LogWarning("Not Ready!");
            return false;
        }
    }

    /// <summary>
    /// Consumate item.
    /// </summary>
    /// <param name="itemInstance"></param>
    /// <param name="Count"></param>
    /// <param name="customCallback"></param>
    /// <param name="ErrorCallback"></param>
    /// <returns></returns>
    public bool Consumate(string itemInstance, int Count,
        System.Action<ConsumeItemResult> customCallback,
        System.Action<PlayFabError> ErrorCallback)
    {
        ConsumeItemRequest request = new ConsumeItemRequest { ConsumeCount = Count, ItemInstanceId = itemInstance };
        PlayFabClientAPI.ConsumeItem(request, customCallback, ErrorCallback);
            return true;
    }

    public bool isReady
    {
        get
        {
            return ready;
        }
        set
        {
            ready = value;
        }
    }
}
