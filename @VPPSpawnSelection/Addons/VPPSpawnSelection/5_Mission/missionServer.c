modded class MissionServer
{
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

	override PlayerBase OnClientNewEvent(PlayerIdentity identity, vector pos, ParamsReadContext ctx){
		if (CreatePlayerEnt(identity, ctx)){
			GetGame().RPCSingleParam(m_player, ERPCs.RPC_CHARACTER_EQUIPPED, NULL, true, m_player.GetIdentity());
		}
		return m_player;
	}

	override void OnEvent(EventType eventTypeId, Param params) 
	{
		if (eventTypeId == PreloadEventTypeID)
		{
			PreloadEventParams preloadParams;
			Class.CastTo(preloadParams, params);
			
			OnPreloadEvent(preloadParams.param1, preloadParams.param2, preloadParams.param3, preloadParams.param4, preloadParams.param5);
		}else{
			super.OnEvent(eventTypeId, params);
		}
	}
	
	override void OnPreloadEvent(PlayerIdentity identity, out bool useDB, out vector pos, out float yaw, out int queueTime){
		if (GetHive()){
			useDB = true;
		}else{
			useDB = false;
			pos = "7500 0.0 7500";
			yaw = 0;
		}
		queueTime = SSConfig.Cast(GetPluginManager().SLGetConfigByType(SSConfig)).GetMaxLobbyTime();
	}
};