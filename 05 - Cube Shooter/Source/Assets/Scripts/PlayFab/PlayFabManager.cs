using PlayFab;
using PlayFab.ClientModels;
using System.Collections.Generic;
using UnityEngine;

public class PlayFabManager : MonoBehaviour
{
	public static PlayFabManager instance;

	public GameObject leaderboard;
	public Transform listingContainer;
	public GameObject listingPrefab;

	private string username;
	private int stars;

	private void Start()
	{
		InitLogin("DefaultUser");
	}

	private void Awake()
	{
		if (instance == null)
		{
			instance = this;
		}
		else
		{
			Debug.LogWarning("Multiple PlayFabManager instance.");
			return;
		}
	}

	// Standalone Get Leaderboard
	public void GetLeaderboard()
	{
		var requestLB = new GetLeaderboardRequest { StartPosition = 0, StatisticName = "Stars", MaxResultsCount = 20 };
		PlayFabClientAPI.GetLeaderboard(requestLB, OnGetLeaderboard, OnFailure);
	}

	private void OnGetLeaderboard(GetLeaderboardResult result)
	{
		foreach (PlayerLeaderboardEntry player in result.Leaderboard)
		{
			// Debug.Log(player.DisplayName + ": " + player.StatValue);
			GameObject temp = Instantiate(listingPrefab, listingContainer);
			LBListing listing = temp.GetComponent<LBListing>();
			listing.username.text = player.DisplayName;
			listing.stars.text = player.StatValue.ToString();
		}
	}

	public void CloseLeaderboard()
	{
		leaderboard.SetActive(false);
		for(int i = listingContainer.childCount - 1; i >= 0; --i)
		{
			Destroy(listingContainer.GetChild(i).gameObject);
		}
	}

	// Initial Login Pipeline
	public void InitLogin(string name)
	{
		username = name;
		if (string.IsNullOrEmpty(PlayFabSettings.staticSettings.TitleId))
		{
			PlayFabSettings.staticSettings.TitleId = "AF209";
		}
		var request = new LoginWithCustomIDRequest { CustomId = name, CreateAccount = true };
		PlayFabClientAPI.LoginWithCustomID(request, OnInitLoginSuccess, OnFailure);
	}

	private void OnInitLoginSuccess(LoginResult result)
	{
		Debug.Log("Login successful.");
		PlayFabClientAPI.UpdateUserTitleDisplayName(
			new UpdateUserTitleDisplayNameRequest { DisplayName = username },
			OnInitDisplayName, OnFailure);
	}

	private void OnInitDisplayName(UpdateUserTitleDisplayNameResult result)
	{
		Debug.Log("Logged in to " + result.DisplayName);
	}

	// Submission Login Pipeline
	public void SubmitLogin(string name, int input)
	{
		username = name;
		stars = input;
		if (string.IsNullOrEmpty(PlayFabSettings.staticSettings.TitleId))
		{
			PlayFabSettings.staticSettings.TitleId = "AF209";
		}
		var request = new LoginWithCustomIDRequest { CustomId = name, CreateAccount = true };
		PlayFabClientAPI.LoginWithCustomID(request, OnSubmitLoginSuccess, OnFailure);
	}

	private void OnSubmitLoginSuccess(LoginResult result)
	{
		Debug.Log("Login successful.");
		PlayFabClientAPI.UpdateUserTitleDisplayName(
			new UpdateUserTitleDisplayNameRequest { DisplayName = username },
			OnSubmitDisplayName, OnFailure);
	}

	private void OnSubmitDisplayName(UpdateUserTitleDisplayNameResult result)
	{
		Debug.Log("Logged in to " + result.DisplayName);

		PlayFabClientAPI.UpdatePlayerStatistics(new UpdatePlayerStatisticsRequest
		{
			// request.Statistics is a list, so multiple StatisticUpdate objects can be defined if required.
			Statistics = new List<StatisticUpdate> {
				new StatisticUpdate { StatisticName = "Stars", Value = stars },
			}
		},
		success => { Debug.Log("User statistics updated"); },
		error => { Debug.Log(error.GenerateErrorReport()); });
	}

	// Standalone On Failure
	private void OnFailure(PlayFabError error)
	{
		Debug.LogWarning("PlayFab failure.");
		Debug.Log(error.GenerateErrorReport());
	}
}