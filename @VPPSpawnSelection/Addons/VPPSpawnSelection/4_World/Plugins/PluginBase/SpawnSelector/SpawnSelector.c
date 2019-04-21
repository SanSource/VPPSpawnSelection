class SpawnSelector extends PluginBase
{
	private ref SSConfig m_Config;
	void SpawnSelector()
	{
		m_Config = GetPluginManager().SLGetConfigByType(SSConfig);

		//---RPC's----
		GetRPCManager().AddRPC( "RPC_GetSettings", "GetSettings", this, SingeplayerExecutionType.Server );
		GetRPCManager().AddRPC( "RPC_HandleSpawnEvent", "HandleSpawnEvent", this, SingeplayerExecutionType.Server );
	}

	void GetSettings( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target )
	{
        if (type == CallType.Server)
        {
        	if (sender != NULL)
        	{
				//reload config before sending....to update any changes
				m_Config.load();
        		GetRPCManager().SendRPC( "RPC_HandleSettings", "HandleSettings", new Param1<ref SSConfig>(m_Config), true, sender);
        	}
        }
    }

    static PlayerBase GetPlayerObjectByIdentity( PlayerIdentity identity )
	{
	    if ( identity == NULL ) return NULL;

	    int networkIdLowBits;
	    int networkIdHighBits;
	    GetGame().GetPlayerNetworkIDByIdentityID( identity.GetPlayerId(), networkIdLowBits, networkIdHighBits );

	    return PlayerBase.Cast( GetGame().GetObjectByNetworkId( networkIdLowBits, networkIdHighBits ) );
	}
	
	void SpawnLoadout(ref SpawnParams data, ref PlayerBase TargetPlayer)
	{
		//PlayerBase TargetPlayer = GetPlayerObjectByIdentity(sender);
		if (SSConfig.Cast(GetPluginManager().SLGetConfigByType(SSConfig)).LoadoutSelectionType() == 1) //CustomizableLoadouts
		{
			ref array<string> items = data.GetCLOItems();
			EntityAI ItemEntity;
			ItemBase IBItem;
			foreach(string str : items){
				if (TargetPlayer != NULL && str != ""){
					ItemEntity = EntityAI.Cast( TargetPlayer.GetHumanInventory().CreateInInventory( str ) );

        			if ( ItemEntity != NULL ){
        				if ( ItemEntity.IsInherited( ItemBase ) )
			            {
			            	if (Class.CastTo( IBItem,ItemEntity )) IBItem.SetupSpawnedItem( IBItem, ItemEntity.GetMaxHealth(), IBItem.GetQuantityMax() );
			            }
        			}
				}
			}
		}else{
			ref SSConfig m_ssConfig;
			ref StaticLoadout m_StaticLoadout;
			if (Class.CastTo(m_ssConfig,GetPluginManager().SLGetConfigByType(SSConfig))){
				m_StaticLoadout = m_ssConfig.GetStaticLoadoutByName(data.GetSLO());
				if (m_StaticLoadout == NULL || TargetPlayer == NULL) return;
				ref array<ref LoadOutSlot> LoadoutDetails = m_StaticLoadout.GetLoadoutDetails();
				foreach(ref LoadOutSlot slot : LoadoutDetails){
					if (slot.GetSlotName() != ""){
						ItemBase itemIB;
						if (slot.GetSlotName() == "Hands"){
							Class.CastTo(itemIB,TargetPlayer.GetHumanInventory().CreateInHands(slot.GetItemClassName()))
						}else{
							Class.CastTo(itemIB,TargetPlayer.GetHumanInventory().CreateInInventory(slot.GetItemClassName()));
						}

						if (itemIB != NULL){
							ref array<string> attachments = slot.GetAttachments();
							foreach(string att : attachments){
								itemIB.GetInventory().CreateAttachment(att);
							}

							ref array<string> inventoryItems = slot.GetCargo();
							foreach(string inv : inventoryItems){
								itemIB.GetInventory().CreateInInventory(inv);
							}
						}
					}
				}
			}
		}

		//Spawn Essential Items
		ref array<string> m_EssientalITems = SSConfig.Cast(GetPluginManager().SLGetConfigByType(SSConfig)).GetEssentialItems();
		foreach(string item : m_EssientalITems){
			EntityAI ItemEntityEE;
			ItemBase IBItemEE;

			if (TargetPlayer == NULL) return;
			if ( Class.CastTo( ItemEntityEE,TargetPlayer.GetInventory().CreateInInventory( item ) ) ){
				if ( ItemEntityEE.IsInherited( ItemBase ) ){
					if (Class.CastTo( IBItemEE,ItemEntityEE )) IBItemEE.SetupSpawnedItem( IBItemEE, IBItemEE.GetMaxHealth(), IBItemEE.GetQuantityMax() );
	            }
			}
		}
	}

	//Calls to spawn loadout once player ready
	void CheckReady(ref SpawnParams data, ref PlayerIdentity sender)
	{
		PlayerBase TargetPlayer;
		if (Class.CastTo(TargetPlayer,GetPlayerObjectByIdentity(sender)) && TargetPlayer.IsAlive()){
			SpawnLoadout(data, TargetPlayer);
			Print("ready");
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(this.CheckReady);
		}else{
			TargetPlayer = PlayerBase.Cast(GetPlayerObjectByIdentity(sender));
			Print("not ready");
		}
	}

	/*
		For player gear/loadout ONLY.. position and character type is done by CTX sent by `StoreLoginData()`
	*/
	void HandleSpawnEvent( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target )
	{
		Param1<ref SpawnParams> data;
        if ( !ctx.Read( data ) ) return;
		
        if (type == CallType.Server)
        {
        	bool m_PlayerReady = false;
        	PlayerBase TargetPlayer;
        	ref SpawnParams _PARAMS_ = data.param1;
        	if (sender != NULL){
        		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(this.CheckReady, 1000, true, _PARAMS_, sender);
        	}
        }
    }
};