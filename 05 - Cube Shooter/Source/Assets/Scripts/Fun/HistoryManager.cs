using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HistoryManager : MonoBehaviour
{
	public List<HistorySlot> historySlots = null;

	private void Start()
	{
		// Read DataManager
		float[][] history = DataManager.instance.inventory.matchHistory;

		// Order by highest index first
		int k = 4;
		for (int i = 0; i < 5; ++i)
		{
			for (int j = k; j >= 0; --j)
			{
				if (history[j][0] < 0.0f)
				{
					--k;
					continue;
				}
				historySlots[i].parseData(history[j]);
				--k;
				break;
			}
		}
	}
}
