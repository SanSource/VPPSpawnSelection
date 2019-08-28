modded class MissionServer
{    
	private ref SSConfig m_Config;
	
	void MissionServer() {
		m_Config = GetPluginManager().SLGetConfigByType(SSConfig);
        GetRPCManager().AddRPC( "RPC_MissionServer", "test", this, SingleplayerExecutionType.Server );
    }

    // This method is only relevant on the server. It will respond to each client with a bool saying whether or not 3d markers are allowed to be used.
    void test( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target ) {        
		if (type == CallType.Server) {	
			if (sender != NULL) {
				//reload config before sending....to update any changes
				m_Config.load();
        		GetRPCManager().SendRPC( "RPC_HandleSettings", "HandleSettings", new Param1<ref SSConfig>(m_Config), true, sender);				
        	}
		}
    }
	
	PlayerBase CreatePlayerEnt(PlayerIdentity identity, ParamsReadContext ctx){		
		Entity playerEnt;
		string skin;
		vector pos;

		ref Param1<string> skinParam; 		  ctx.Read(skinParam); 			   skin 	     = skinParam.param1;
		ref Param1<vector> posParam;  		  ctx.Read(posParam);  			   pos  		 = posParam.param1;
	
		if (skin == "") {
			skin = GetGame().CreateRandomPlayer();
		}
		if (pos == "0 0 0"){
			pos = SSConfig.Cast(GetPluginManager().SLGetConfigByType(SSConfig)).GetRandomSpawnPoint();
		}else{
			//Add cooldown only if point is NOT random
			ref SpawnPointDetails SpawnPoint = SSConfig.Cast(GetPluginManager().SLGetConfigByType(SSConfig)).GetSpawnPointByPos(pos);
			CoolDownManager.Cast(GetPluginManager().GetPluginByType(CoolDownManager)).AddCooldown(identity.GetId(),SpawnPoint.GetName());
		}

		//Select player vehicle
		playerEnt = GetGame().CreatePlayer(identity, skin, pos, 0, "NONE");
		Class.CastTo(m_player, playerEnt);
		GetGame().SelectPlayer(identity, m_player);

		return m_player;
	}
};