using UnityEngine;
using UnityEngine.UI;
using UnityEngine.Events;
using UnityEngine.EventSystems;
using System;

// Button that raises onDown event when OnPointerDown is called.
public class ButtonV2 : Button
{
	// Event delegate triggered on mouse or touch down.
	[SerializeField]
	ButtonDownEvent _onDown = new ButtonDownEvent();

	[SerializeField]
	ButtonEnterEvent _onEnter = new ButtonEnterEvent();

	[SerializeField]
	ButtonExitEvent _onExit = new ButtonExitEvent();

	protected ButtonV2() { }

	public override void OnPointerDown(PointerEventData eventData)
	{
		base.OnPointerDown(eventData);
		_onDown.Invoke();
	}

	public override void OnPointerEnter(PointerEventData eventData)
	{
		base.OnPointerEnter(eventData);
		_onEnter.Invoke();
	}

	public override void OnPointerExit(PointerEventData eventData)
	{
		base.OnPointerExit(eventData);
		_onExit.Invoke();
	}

	public ButtonDownEvent onDown
	{
		get { return _onDown; }
		set { _onDown = value; }
	}

	public ButtonEnterEvent onEnter
	{
		get { return _onEnter; }
		set { _onEnter = value; }
	}

	public ButtonExitEvent onExit
	{
		get { return _onExit; }
		set { _onExit = value; }
	}

	[Serializable]
	public class ButtonDownEvent : UnityEvent { }

	[Serializable]
	public class ButtonEnterEvent : UnityEvent { }

	[Serializable]
	public class ButtonExitEvent : UnityEvent { }
}