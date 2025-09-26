using UnityEngine;
using System.Collections;
using System;

/// <summary>
/// Ryan's Student class doesn't allow me to send it over the network.
/// So here is a class that allows me to do it.
/// </summary>
[Serializable]
public struct studserialkiller
{
    public int SX;
    public int SZ;
    public int t;
    public int teamtype;
    public string uuid;

    public studserialkiller(Student s)
    {
        SX = s.SX;
        SZ = s.SZ;
        t = s.t;
        teamtype = s.teamType;
        uuid = s.uuid;
    }
}
