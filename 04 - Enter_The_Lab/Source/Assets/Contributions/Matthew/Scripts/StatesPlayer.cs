using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class StateHunker : State
{
    public string StateID { get; }
    public StateHunker(string stateID)
    {
        StateID = stateID;
    }
    public void Enter()
    {

    }
    public void Update(float dt)
    {

    }
    public void Exit()
    {

    }
}

public class StateNormal : State
{
    public string StateID { get; }
    public StateNormal(string stateID)
    {
        StateID = stateID;
    }
    public void Enter()
    {

    }
    public void Update(float dt)
    {

    }
    public void Exit()
    {

    }
}

public class StateOverwatch : State
{
    public string StateID { get; }
    public StateOverwatch(string stateID)
    {
        StateID = stateID;
    }
    public void Enter()
    {

    }
    public void Update(float dt)
    {

    }
    public void Exit()
    {

    }
}
