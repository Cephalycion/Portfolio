using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class InstructionsPage : MonoBehaviour
{
    public GameObject[] pages;
    // Start is called before the first frame update
    void Start()
    {
        pages[0].SetActive(true);
        pages[1].SetActive(false);
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void changePage(int page)
    {
        pages[0].SetActive(false);
        pages[1].SetActive(false);
        pages[page].SetActive(true);
    }
}
