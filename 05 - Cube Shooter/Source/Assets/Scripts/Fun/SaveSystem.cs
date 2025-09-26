using UnityEngine;
using System.IO;
using System.Runtime.Serialization.Formatters.Binary;

public static class SaveSystem
{
	public static string path = Application.persistentDataPath + "/inventory.dat";

    public static void saveInventory (Inventory inventory)
	{
		Debug.Log("Saving inventory...\n" + path);
		BinaryFormatter formatter = new BinaryFormatter();
		FileStream stream = new FileStream(path, FileMode.Create);
		formatter.Serialize(stream, inventory);
		stream.Close();
	}

	public static Inventory loadInventory()
	{
		Inventory data = null;
		if (File.Exists(path))
		{
			Debug.Log("Loading inventory...\n" + path);
			BinaryFormatter formatter = new BinaryFormatter();
			FileStream stream = new FileStream(path, FileMode.Open);
			data = (Inventory)formatter.Deserialize(stream);
			stream.Close();

			Inventory n = new Inventory();
			if (data.version < n.version)
			{
				Debug.Log("Outdated version, creating new file.");
				saveInventory(new Inventory());
				data = loadInventory();
			}
		}
		else
		{
			Debug.Log("No savefile, creating new file.");
			saveInventory(new Inventory());
			data = loadInventory();
		}
		return data;
	}

	public static void resetInventory()
	{
		saveInventory(new Inventory());
	}
}
