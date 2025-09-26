using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ProjectileBlocker : MonoBehaviour
{
    public enum BlockerType
	{
        BLOCK_NONE,
		BLOCK_PARTIAL,
		BLOCK_FULL
	};

	public BlockerType Type;
}
