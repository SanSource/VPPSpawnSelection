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
	
	void SpawnLoadout(ref SpawnParams data, ref PlayerIdentity sender)
	{
		PlayerBase TargetPlayer = GetPlayerObjectByIdentity(sender);
		if (SSConfig.Cast(GetPluginManager().SLGetConfigByType(SSConfig)).LoadoutSelectionType() == 1) //CustomizableLoadouts
		{
			ref array<string> items = data.GetCLOItems();
			EntityAI ItemEntity;
			ItemBase IBItem;
			foreach(string str : items){
				if (TargetPlayer){
					ItemEntity = EntityAI.Cast( TargetPlayer.GetHumanInventory().CreateInInventory( str ) );

        			if ( ItemEntity == NULL ) continue;

        			if ( ItemEntity.IsInherited( ItemBase ) )
		            {
		                IBItem = ItemBase.Cast( ItemEntity );
		                IBItem.SetupSpawnedItem( IBItem, ItemEntity.GetMaxHealth(), IBItem.GetQuantityMax() );
		            }
				}
			}
		}else{

			ref StaticLoadout m_StaticLoadout = SSConfig.Cast(GetPluginManager().SLGetConfigByType(SSConfig)).GetStaticLoadoutByName(data.GetSLO());
			if (m_StaticLoadout == NULL) return;

			ref array<ref LoadOutSlot> LoadoutDetails = m_StaticLoadout.GetLoadoutDetails();
			foreach(ref LoadOutSlot slot : LoadoutDetails){
				ItemBase itemIB;
				if (slot.GetSlotName() == "Hands"){
					Class.CastTo(itemIB,TargetPlayer.GetHumanInventory().CreateInHands(slot.GetItemClassName()))
				}else{
					Class.CastTo(itemIB,TargetPlayer.GetHumanInventory().CreateInInventory(slot.GetItemClassName()));
				}

				if (itemIB == NULL) continue;
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

		//Spawn Essential Items
		ref array<string> m_EssientalITems = SSConfig.Cast(GetPluginManager().SLGetConfigByType(SSConfig)).GetEssentialItems();
		foreach(string item : m_EssientalITems){
			EntityAI ItemEntityEE;
			ItemBase IBItemEE;

			ItemEntityEE = EntityAI.Cast( TargetPlayer.GetInventory().CreateInInventory( item ) );
			if ( ItemEntityEE == NULL ) return;

			if ( ItemEntityEE.IsInherited( ItemBase ) ){
                IBItemEE = ItemBase.Cast( ItemEntityEE );
                IBItemEE.SetupSpawnedItem( IBItemEE, IBItemEE.GetMaxHealth(), IBItemEE.GetQuantityMax() );
            }
		}

		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(this.SpawnLoadout);
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
        	ref SpawnParams _PARAMS_ = data.param1;
        	if (sender != NULL){
				GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(this.SpawnLoadout, 5000, false, _PARAMS_, sender);
        	}
        }
    }
};