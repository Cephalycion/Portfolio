using UnityEngine;
using System.Collections;
/// <summary>
/// For use with the actual game, from Main Menu Transition.
/// </summary>
public class GameBootStrapper : MonoBehaviour
{

    // Use this for initialization
    void Start()
    {
        if (SceneConstant.creationSide == MPSide.MP_SERVERALONE)
        {
            BeardedManStudios.Forge.Logging.BMSLog.Log("Bootstrap Data Found to be Server.");
            MultiPrefab.Instance.port = SceneConstant.port.ToString();
            MultiPrefab.Instance.ipAddress = SceneConstant.ipAddress;
            MultiPrefab.Instance.hideDebug = true;
            MultiPrefab.StartServer();
        }
        else if (SceneConstant.creationSide == MPSide.MP_CLIENTA)
        {
            BeardedManStudios.Forge.Logging.BMSLog.Log("Bootstrap Data Found to be Client.");
            MultiPrefab.Instance.port = SceneConstant.port.ToString();
            MultiPrefab.Instance.ipAddress = SceneConstant.ipAddress;
            MultiPrefab.Instance.hideDebug = true;
            MultiPrefab.Connect();

        }
        else
        {
            BeardedManStudios.Forge.Logging.BMSLog.Log("No Bootstrap Info from Inventory!");
        }
    }
}
