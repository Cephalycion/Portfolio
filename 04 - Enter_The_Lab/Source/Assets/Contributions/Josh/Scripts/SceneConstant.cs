using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public static class SceneConstant
{
    public static MPSide creationSide = MPSide.MP_NONE;
    public static string ipAddress = "0.0.0.0";
    public static string usernameclient = "";
    public static string username = "";
    public static string Map = "Level_L3";
    public static int port = 25565;
    public static bool returnFromGame = false;
    public static List<Inventory_Item> PlaySelection = new List<Inventory_Item>();
}
