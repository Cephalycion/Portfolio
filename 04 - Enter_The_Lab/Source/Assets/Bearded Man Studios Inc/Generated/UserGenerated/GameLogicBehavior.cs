using BeardedManStudios.Forge.Networking;
using BeardedManStudios.Forge.Networking.Unity;
using UnityEngine;

namespace BeardedManStudios.Forge.Networking.Generated
{
	[GeneratedRPC("{\"types\":[[\"byte[]\", \"byte[]\", \"string\", \"string\"][\"byte[]\", \"byte[]\", \"string\"][\"string\", \"int\", \"int\"][\"string\", \"string\", \"int\"][][\"string\"][\"string\"][\"byte[]\", \"string\"]]")]
	[GeneratedRPCVariableNames("{\"types\":[[\"StudentContentA\", \"StudentContentB\", \"serverUsername\", \"clientUsername\"][\"StudentA\", \"StudentB\", \"UUID\"][\"uuid\", \"TX\", \"TZ\"][\"UUID\", \"StudentUUID\", \"damage\"][][\"UUID\"][\"UUID\"][\"client\", \"clientUsername\"]]")]
	public abstract partial class GameLogicBehavior : NetworkBehavior
	{
		public const byte RPC_SEND_STUDENT = 0 + 5;
		public const byte RPC_STUDENT_PROJ_SPAWN = 1 + 5;
		public const byte RPC_STUDENT_MOVE = 2 + 5;
		public const byte RPC_STUDENT_HIT = 3 + 5;
		public const byte RPC_STUDENT_END_TURN = 4 + 5;
		public const byte RPC_STUDENT_OVER_WATCH = 5 + 5;
		public const byte RPC_STUDENT_HUNKER = 6 + 5;
		public const byte RPC_CLIENT_STUDENT_PRE = 7 + 5;
		
		public GameLogicNetworkObject networkObject = null;

		public override void Initialize(NetworkObject obj)
		{
			// We have already initialized this object
			if (networkObject != null && networkObject.AttachedBehavior != null)
				return;
			
			networkObject = (GameLogicNetworkObject)obj;
			networkObject.AttachedBehavior = this;

			base.SetupHelperRpcs(networkObject);
			networkObject.RegisterRpc("SendStudent", SendStudent, typeof(byte[]), typeof(byte[]), typeof(string), typeof(string));
			networkObject.RegisterRpc("StudentProjSpawn", StudentProjSpawn, typeof(byte[]), typeof(byte[]), typeof(string));
			networkObject.RegisterRpc("StudentMove", StudentMove, typeof(string), typeof(int), typeof(int));
			networkObject.RegisterRpc("StudentHit", StudentHit, typeof(string), typeof(string), typeof(int));
			networkObject.RegisterRpc("StudentEndTurn", StudentEndTurn);
			networkObject.RegisterRpc("StudentOverWatch", StudentOverWatch, typeof(string));
			networkObject.RegisterRpc("StudentHunker", StudentHunker, typeof(string));
			networkObject.RegisterRpc("ClientStudentPre", ClientStudentPre, typeof(byte[]), typeof(string));

			networkObject.onDestroy += DestroyGameObject;

			if (!obj.IsOwner)
			{
				if (!skipAttachIds.ContainsKey(obj.NetworkId)){
					uint newId = obj.NetworkId + 1;
					ProcessOthers(gameObject.transform, ref newId);
				}
				else
					skipAttachIds.Remove(obj.NetworkId);
			}

			if (obj.Metadata != null)
			{
				byte transformFlags = obj.Metadata[0];

				if (transformFlags != 0)
				{
					BMSByte metadataTransform = new BMSByte();
					metadataTransform.Clone(obj.Metadata);
					metadataTransform.MoveStartIndex(1);

					if ((transformFlags & 0x01) != 0 && (transformFlags & 0x02) != 0)
					{
						MainThreadManager.Run(() =>
						{
							transform.position = ObjectMapper.Instance.Map<Vector3>(metadataTransform);
							transform.rotation = ObjectMapper.Instance.Map<Quaternion>(metadataTransform);
						});
					}
					else if ((transformFlags & 0x01) != 0)
					{
						MainThreadManager.Run(() => { transform.position = ObjectMapper.Instance.Map<Vector3>(metadataTransform); });
					}
					else if ((transformFlags & 0x02) != 0)
					{
						MainThreadManager.Run(() => { transform.rotation = ObjectMapper.Instance.Map<Quaternion>(metadataTransform); });
					}
				}
			}

			MainThreadManager.Run(() =>
			{
				NetworkStart();
				networkObject.Networker.FlushCreateActions(networkObject);
			});
		}

		protected override void CompleteRegistration()
		{
			base.CompleteRegistration();
			networkObject.ReleaseCreateBuffer();
		}

		public override void Initialize(NetWorker networker, byte[] metadata = null)
		{
			Initialize(new GameLogicNetworkObject(networker, createCode: TempAttachCode, metadata: metadata));
		}

		private void DestroyGameObject(NetWorker sender)
		{
			MainThreadManager.Run(() => { try { Destroy(gameObject); } catch { } });
			networkObject.onDestroy -= DestroyGameObject;
		}

		public override NetworkObject CreateNetworkObject(NetWorker networker, int createCode, byte[] metadata = null)
		{
			return new GameLogicNetworkObject(networker, this, createCode, metadata);
		}

		protected override void InitializedTransform()
		{
			networkObject.SnapInterpolations();
		}

		/// <summary>
		/// Arguments:
		/// byte[] StudentContentA
		/// byte[] StudentContentB
		/// string serverUsername
		/// string clientUsername
		/// </summary>
		public abstract void SendStudent(RpcArgs args);
		/// <summary>
		/// Arguments:
		/// byte[] StudentA
		/// byte[] StudentB
		/// string UUID
		/// </summary>
		public abstract void StudentProjSpawn(RpcArgs args);
		/// <summary>
		/// Arguments:
		/// string uuid
		/// int TX
		/// int TZ
		/// </summary>
		public abstract void StudentMove(RpcArgs args);
		/// <summary>
		/// Arguments:
		/// string UUID
		/// string StudentUUID
		/// int damage
		/// </summary>
		public abstract void StudentHit(RpcArgs args);
		/// <summary>
		/// Arguments:
		/// </summary>
		public abstract void StudentEndTurn(RpcArgs args);
		/// <summary>
		/// Arguments:
		/// </summary>
		public abstract void StudentOverWatch(RpcArgs args);
		/// <summary>
		/// Arguments:
		/// </summary>
		public abstract void StudentHunker(RpcArgs args);
		/// <summary>
		/// Arguments:
		/// </summary>
		public abstract void ClientStudentPre(RpcArgs args);

		// DO NOT TOUCH, THIS GETS GENERATED PLEASE EXTEND THIS CLASS IF YOU WISH TO HAVE CUSTOM CODE ADDITIONS
	}
}