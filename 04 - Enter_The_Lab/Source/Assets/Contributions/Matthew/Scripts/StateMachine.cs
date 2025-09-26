using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class StateMachine : MonoBehaviour
{
    private Dictionary<string, State> m_stateMap = new Dictionary<string, State>();
    State m_currState = null;
    State m_nextState = null;

    public void AddState(State newState)
    {
        foreach (KeyValuePair<string, State> state in m_stateMap)
        {
            if (state.Value.StateID == newState.StateID)
                return;
        }
        m_stateMap.Add(newState.StateID, newState);
        if (m_currState == null)
            m_currState = m_nextState = m_stateMap[newState.StateID];
    }

    public void SetNextState(string nextStateID)
    {
        if(m_stateMap.ContainsKey(nextStateID))
        {
            m_nextState = m_stateMap[nextStateID];
        }
    }

    public string GetCurrentState()
    {
        if (m_currState != null)
            return m_currState.StateID;
        return "<No States>";
    }
    // Update is called once per frame
    public void Update()
    {
        if (m_nextState != m_currState)
        {
            m_currState.Exit();
            m_currState = m_nextState;
            m_currState.Enter();
        }
        m_currState.Update(Time.deltaTime);
    }
}
