using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;
using System;
using UnityEngine.UI;

[Serializable]
public class SavedLevel
{
    public string name;
    public GameObject _Object;
}

public class LoadLevelManager : MonoBehaviour
{
    public GameObject loadPanel, levelObject;

    [SerializeField]
    List<SavedLevel> levelsList = new List<SavedLevel>();
    public static LoadLevelManager Instance;

    void Awake()
    {
        Instance = this;
        UpdateLevels();
    }

    public void AddLevel(string name)
    {
        BeardedManStudios.Forge.Logging.BMSLog.Log("Added Level with " + name);
        Debug.Log("Added Level with " + name);
        SavedLevel newMessage = new SavedLevel();

        newMessage.name = name;

        GameObject messageObject = Instantiate(levelObject, loadPanel.transform);
        newMessage._Object = levelObject;
        newMessage._Object.GetComponentInChildren<Text>().text = newMessage.name;

        levelsList.Add(newMessage);
    }

    public void UpdateLevels()
    {
        TextAsset[] levels = Resources.LoadAll<TextAsset>("Text");
        foreach (TextAsset file in levels)
        {
            string fullname = file.name;
            if (!fullname.Contains(".meta"))
            {
                bool exists = false;
                foreach (SavedLevel level in levelsList)
                {
                    if (level.name == fullname)
                    {
                        exists = true;
                        break;
                    }
                }
                if (!exists)
                {
                    AddLevel(fullname);
                }
            }
            else
                continue;
            
        }
    }

}
