using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;

public class SettingsWrapper
{
    public float Volume;
    public float SFXVolume;
    public bool DisableAllShaders;
    public bool DisableTVEffect;
    public float BloomIntensity;
    public SettingsWrapper()
    {
        Volume = 1.0f;
        SFXVolume = 1.0f;
        DisableAllShaders = false;
        DisableTVEffect = false;
        BloomIntensity = 1.0f;
    }
}

public class Settings : MonoBehaviour
{
    public static Settings Instance;
    public string settingsFileName = "Settings_settings";
    public SettingsWrapper settings;

    void Awake()
    {
        if (Instance != null && Instance != this)
        {
            Destroy(gameObject);
            return;
        }
        DontDestroyOnLoad(this);
        settings = new SettingsWrapper();
        Load();
        Instance = this;
    }

    private string GetFilePath(string path)
    {
        BeardedManStudios.Forge.Logging.BMSLog.Log(Application.dataPath + "/" + path + ".json");
        return Application.dataPath + "/" + path + ".json";
    }

    public void Load()
    {
        string filePath = GetFilePath(settingsFileName);
        if (File.Exists(filePath))
        {
            using (StreamReader reader = new StreamReader(filePath))
            {
                string json = reader.ReadToEnd();
                BeardedManStudios.Forge.Logging.BMSLog.Log("File found:" + json);
                SettingsWrapper newSettings = JsonUtility.FromJson<SettingsWrapper>(json);
                settings = newSettings;
            }
            File.Delete(filePath);
        }
        else
        {
            BeardedManStudios.Forge.Logging.BMSLog.LogWarning("File Not Found!!!");
            Save();
        }

    }

    public void Save()
    {
        string filePath = GetFilePath(settingsFileName);
        FileStream file;
        SettingsWrapper tempsettings = settings;

        if (File.Exists(filePath))
            file = File.OpenWrite(filePath);
        else
            file = new FileStream(filePath, FileMode.Create);

        string json = JsonUtility.ToJson(tempsettings, true);

        using (StreamWriter writer = new StreamWriter(file))
        {
            writer.Write(json);
        }

    }

}

