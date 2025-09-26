using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public interface State
{
    string StateID { get; }
    //State(string stateID)
    //{
    //    StateID = stateID;
    //}

    void Enter();
    void Update(float dt);
    void Exit();

    // Update is called once per frame
}
