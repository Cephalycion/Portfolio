using UnityEngine;
using System.Collections;
using UnityEngine.UI;
using System.Collections.Generic;
using DG.Tweening;
using TMPro;

public class DakaraAruiteIkou : MonoBehaviour
{
    public TextMeshProUGUI textScript;
    public float cycleSpeed;
    private float cTime = 0.0f;
    public float ChangeSpeed;
    public float heightSwap;
    private List<string> textCycler = new List<string>();
    public RectTransform CanvasTransform = null;
    private float retY;
    // Use this for initialization

    private void OnDisable()
    {
        CanvasTransform.anchoredPosition = new Vector2(CanvasTransform.anchoredPosition.x, retY);
    }

    void Start()
    {
        textCycler.Add("Supervisor: Mr Weng Jun Xuan");
        textCycler.Add("Programming: Ryan Chan");
        textCycler.Add("Programming: Woo Josh");
        textCycler.Add("Programming: Matthew Chan ");
        textCycler.Add("Programming: Muhammad Afiq");
        textCycler.Add("Lecturers: Mr Izan Lim");
        textCycler.Add("Lecturers: Mr Toh Da Jun");
        textCycler.Add("Lecturers: Mr Tang Wen Sheng");
        textCycler.Add("Lecturers: Ms Tan Siew Lan");
        textCycler.Add("Lecturers: Mr Mario Toon");
        textCycler.Add("Shoutout: Wake Up, Girls!");
        textCycler.Add("Shoutout: TrySail");
        textCycler.Add("Shoutout: Run Girls, Run!");
        textCycler.Add("Shoutout: Alexander Neo");
        retY = CanvasTransform.anchoredPosition.y;
    }
    
    // Update is called once per frame
    void FixedUpdate()
    {

        if (cTime + Time.fixedDeltaTime > cycleSpeed)
        {
            cTime = 0.0f;
            CanvasTransform.DOAnchorPosY(retY - heightSwap, ChangeSpeed).SetEase(Ease.InOutCubic).OnComplete(() => 
            {
                textScript.text = textCycler[Random.Range(0, textCycler.Count-1)];
                CanvasTransform.DOAnchorPosY(retY, ChangeSpeed).SetEase(Ease.InOutCubic);
            });
        }
        else
        {
            cTime += Time.fixedDeltaTime;
        }
    }
}
