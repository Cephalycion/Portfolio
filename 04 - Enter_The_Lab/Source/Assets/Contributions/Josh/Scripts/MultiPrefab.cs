using UnityEngine;
using BeardedManStudios.Forge.Networking;
using BeardedManStudios.Forge.Networking.Unity;
using BeardedManStudios.Forge.Networking.Generated;

public enum MPSide
{
    MP_NONE,
    MP_CLIENTA,
    MP_SERVERALONE
}
public class MultiPrefab : MonoBehaviour
{
    public string ipAddress = "127.0.0.1";
    public string port = "25565";
    public bool useTCP = false;

    public GameObject networkManagerObject = null;
    private NetWorker server;

    public static MultiPrefab Instance = null;

    public bool enableProbe = false;
    public GameLogicBehavior behavior = null;
    public MPSide serverSide = MPSide.MP_NONE;
    public bool hideDebug = false;
    protected MultiPrefab() { }
    private void Awake()
    {
        //DontDestroyOnLoad(gameObject);
        //networkManager = gameObject;
        Instance = this;
    }
    public static void StartServer()
    {
        NetWorker.PingForFirewall(ushort.Parse(Instance.port));
        Rpc.MainThreadRunner = MainThreadManager.Instance;
        if (Instance.useTCP)
        {
            Instance.server = new TCPServer(2);
            ((TCPServer)Instance.server).Connect();
        }
        else
        {
            Debug.Log("0.0.0.0 | " + Instance.port);
            Instance.server = new UDPServer(2);
            ((UDPServer)Instance.server).Connect("0.0.0.0", ushort.Parse(Instance.port));
        }

        Connected(Instance.server);
        Instance.serverSide = MPSide.MP_SERVERALONE;
        if (Instance.behavior == null)
            Instance.behavior = NetworkManager.Instance.InstantiateGameLogic();
        else
            BeardedManStudios.Forge.Logging.BMSLog.Log("Logic Inited! Attempt to Init Twice!");
    }

    public static void Connect()
    {
        Rpc.MainThreadRunner = MainThreadManager.Instance;
        ushort port;
        if (!ushort.TryParse(Instance.port, out port))
        {
            BeardedManStudios.Forge.Logging.BMSLog.Log("The supplied port number is not within the allowed range 0-" + ushort.MaxValue);
            return;
        }
        NetWorker client;

        if (Instance.useTCP)
        {
            client = new TCPClient();
            ((TCPClient)client).Connect(Instance.ipAddress, port);
        }
        else
        {
            client = new UDPClient();
            ((UDPClient)client).Connect(Instance.ipAddress, port);
        }
        //client.disc
        Connected(client);
        Instance.serverSide = MPSide.MP_CLIENTA;
    }
    string guiipaddress = "";
    private void OnGUI()
    {
        if (hideDebug)
            return;
        guiipaddress = GUI.TextField(new Rect(200, 50, 200, 20), guiipaddress, 25);
        if (GUI.Button(new Rect(500, 150, 100, 25), "Client"))
        {
            if (guiipaddress != "")
                Instance.ipAddress = guiipaddress;
            if (Instance.serverSide == MPSide.MP_NONE)
                Connect();
            else
            {
                BeardedManStudios.Forge.Logging.BMSLog.Log("Server Started Already. Stopping...");
                if (Instance.enableProbe)
                    NetWorker.EndSession();
                if (Instance.server != null) Instance.server.Disconnect(true);
                Instance.serverSide = MPSide.MP_NONE;
            }
        }
        
        if (GUI.Button(new Rect(500, 100, 100, 25), "Server"))
        {
            if (Instance.serverSide == MPSide.MP_NONE)
                StartServer();
            else
            {
                BeardedManStudios.Forge.Logging.BMSLog.Log("Server Started Already. Stopping...");
                if (Instance.enableProbe)
                    NetWorker.EndSession();
                if (Instance.server != null) Instance.server.Disconnect(true);
                Instance.serverSide = MPSide.MP_NONE;
            }
        }
    }
    public static void Connected(NetWorker networker)
    {
        if (!networker.IsBound)
        {
            BeardedManStudios.Forge.Logging.BMSLog.Log("NetWorker failed to bind");
            return;
        }
        if (Instance.networkManagerObject == null)
        {
            BeardedManStudios.Forge.Logging.BMSLog.Log("Prefabmissing");
            return;
        }
        NetworkManager nwm = Instance.networkManagerObject.GetComponent<NetworkManager>();
        nwm.Initialize(networker);
        if (networker is IClient)
            nwm.Networker.serverAccepted += Networker_serverAccepted;
        if (networker is IServer)
            NetworkObject.Flush(networker);
        else
            BeardedManStudios.Forge.Logging.BMSLog.Log("Client OK");
    }

    private static void Networker_serverAccepted(NetWorker sender)
    {
        NetworkObject.Flush(sender);
        BeardedManStudios.Forge.Logging.BMSLog.Log("Server Accepted. Flush done and Ready.");
    }

    private void OnApplicationQuit()
    {
        if (enableProbe)
            NetWorker.EndSession();

        if (server != null) server.Disconnect(true);
    }
    
    public void Stop()
    {
        if (enableProbe)
            NetWorker.EndSession();
        if (server != null) server.Disconnect(true);
        //if (client)
    }
}