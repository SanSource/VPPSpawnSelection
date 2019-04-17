modded class DayZGame
{
	private int 	m_TickDownTime;
	//base def var bool == m_IsPlayerSpawning;
	private bool vpp_IsNewCharacter;
	private bool vpp_CanPlayerSpawn = false;
	private int  vpp_TickLoading = 4;
	private string LoadingMsg = "Please Wait..Retrieving Data From Server";

	void DayZGame()
	{
		Print("DZM_VPPSpawnSelection!");
	}

	/*
		This bool is to be given a state via the spawn selector menu!
	*/
	void SetSpawnStatus(bool state){
		vpp_CanPlayerSpawn = state;
	}

	override void OnEvent(EventType eventTypeId, Param params)
	{
		if (eventTypeId == ClientSpawningEventTypeID)
		{
			ClientSpawningEventParams spawningParams;
			if (Class.CastTo(spawningParams, params)){
				//If its a new char then set to use max timer for lobby kick....
				if (spawningParams.param2){
					m_TickDownTime = spawningParams.param1;
				}else{
					m_TickDownTime = 15; //Login Time for saved chars
				}
				OnClientSpawningEvent(m_TickDownTime, spawningParams.param2);
			}

		}else{
			super.OnEvent(eventTypeId,params);
		}
	}

	/*
		Called by OnEvent on any spawn event wheather its login or respawn
	*/
	override void OnClientSpawningEvent(int queueTime, bool newChar)
	{
		/* NOTE: StoreCTXData must be called for game to continue !!! */
		GetGame().GetUserManager().GetUserDatabaseIdAsync();

		if (newChar)
		{
			GetRPCManager().SendRPC( "RPC_GetSettings", "GetSettings", NULL, true);
			GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.ShowSpawnScene, 1000, true);
			GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.ClientOnSpawnScreen, 100, true, newChar);
		}else{
			// turn the lights off
			SetEVValue(-5);
			if (queueTime > 0)
			{
				// timer for spawning screen
				GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.ClientSpawning, 1000, true, newChar);
			}else{
				//Triggers only when timer is less than 1
				ClientSpawningFinished(newChar);
			}
		}
	}

	/*
		Count down to Show spawn screen ( 5 second wait to get RPC data from server )
	*/
	void ShowSpawnScene()
	{
		if (vpp_TickLoading >= 0)
		{
			LoadingMsg += ".";
		    GetUIManager().ScreenFadeIn(0, LoadingMsg, FadeColors.BLACK, FadeColors.WHITE);
			vpp_TickLoading--;
		}else{
			vpp_TickLoading = 4;
			LoadingMsg = "Please Wait...Retrieving Data From Server";
			GetUIManager().ScreenFadeOut(0);
		    GetUIManager().EnterScriptedMenu(VPP_MENU_SPAWN_SCENE_NEW, NULL);
			GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(this.ShowSpawnScene);
		}
	}

	/*
		Called within loop to check if player is ready to spawn / click on spawn within GUI
	*/
	void ClientOnSpawnScreen(bool newChar)
	{
		if (vpp_CanPlayerSpawn){
			vpp_CanPlayerSpawn = false;
			ClientSpawningFinished(newChar);
			GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(this.ClientOnSpawnScreen);
			Print("--->> SPAWN SCREEN SAYS PLAYER IS READY...");
		}
	}

	/*
		Called by `OnClientSpawningEvent` every second to count down then exit with `ClientSpawningFinished` & `CancelQueueTime`
	*/
	override void ClientSpawning(bool newChar)
	{
		// countdown on the spawning screen
		if (m_TickDownTime >= 0) // count all the way to zero
		{
		#ifndef NO_GUI	
			string text = "Spawning in: " + m_TickDownTime.ToString() + " " + "#dayz_game_seconds" + " (°>°)";
			GetUIManager().ScreenFadeIn(0, text, FadeColors.BLACK, FadeColors.WHITE);
		#endif
			m_TickDownTime--;
		}else{
			// hide spawning text
			CancelQueueTime();
			ClientSpawningFinished(newChar);
		}
	}
	
	/*
		Tells the game to let server know that we are ready to spawn in: `StoreCTXData` does exactly that
	*/
	override void ClientSpawningFinished(bool newChar)
	{
		// tell game to continue
		StoreCTXData(newChar);
		
		//Toggles logo (loading screen)
		Mission mission = GetMission();
		
		if ( mission && m_IsPlayerSpawning )
		{
			m_loading = new LoadingScreen(this);
			m_loading.Show();
			m_loading.SetTitle("Loading: Please wait...");
			m_IsPlayerSpawning = false;
		}
	}

	/*
		Kills the timer loop
	*/
	override void CancelQueueTime()
	{
		GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(this.ClientSpawning);
	}

	/*
		This functions MUST be called to continue the login/respawn
	*/
	void StoreCTXData(bool newChar)
	{
		ref array<ref Param> params = new array<ref Param>;
			
		ref Param skinParam = new Param1<string>(GetSpawnParams().GetSelectedCharacterSkin());
		params.Insert(skinParam);
			
		ref Param posParam = new Param1<vector>(GetSpawnParams().GetSelectedSpawnPoint());
		params.Insert(posParam);

		ref Param spawnParamsClass = new Param1<ref SpawnParams>(GetSpawnParams());
		params.Insert(spawnParamsClass);

		GetGame().StoreLoginData(params); //This param is handled at server, this MUST match with server!
		if (newChar){
			GetRPCManager().SendRPC( "RPC_HandleSpawnEvent", "HandleSpawnEvent", new Param1<ref SpawnParams>(GetSpawnParams()), true); //call to spawn gear
		}
		DestroySpawnParamsClass();   //Destroy and reset the carrier class for future use.
	}

	/*
		Called by engine when dealing with connect/respawn event.
	*/
	override void SetNewCharacter(bool state)
	{
		vpp_IsNewCharacter = state;
	}
	
	/*
		Used by engine to check if player new after respawn?
	*/
	override bool IsNewCharacter()
	{
		return vpp_IsNewCharacter;
	}
	//---------------------------------------------------------------
	
	/*
		Convert string to vector format
	*/
	static vector StringToVector(string str){
		vector oV = "0 0 0";
		TStringArray strs = new TStringArray;
		str.Replace("<","");
		str.Replace(">","");
		str.Replace(","," ");

		str.Split( " ", strs );

		oV = Vector(strs.Get(0).ToFloat(),strs.Get(1).ToFloat(),strs.Get(2).ToFloat());
		return oV;
	}

	/*
		Convert vector to string format
	*/
	static string VectorToString(vector vctr){
		string oStr;
		oStr = vctr[0].ToString();
		oStr += "," + vctr[1].ToString();
		oStr += "," + vctr[2].ToString();
		return oStr;
	}


	static vector SwapYZ(vector vec){
		vector tmp;
		tmp[0] = vec[0];
		tmp[1] = vec[2];
		tmp[2] = vec[1];
	
		return tmp;
	}
	
	static vector SnapToGround(vector pos){
		float pos_x = pos[0];
		float pos_z = pos[2];
		float pos_y = GetGame().SurfaceY(pos_x, pos_z);
		vector tmp_pos = Vector(pos_x, pos_y, pos_z);
		tmp_pos[1] = tmp_pos[1] + pos[1];
	
		return tmp_pos;
	}

	/*
		use configs to get name instead of entity....easier than searching for selected entity
	*/
	static string GetDisplayName(string cfg_Name){
		string result;
		GetGame().ConfigGetText("CfgVehicles " + cfg_Name + " displayName", result);
		if (result != ""){
			return result;
		}
		return cfg_Name;
	}
}

/*
	Carrier for spawn details...
*/
class SpawnParams
{
	private ref array<string> m_StaticLoadoutItems;
	private vector 			  m_SelectedSpawnPoint;
	private string            m_SelectedStaticLO;
	private string 			  m_SelectedCharacterSkin;

	void SpawnParams(){
		Print("-->>SpawnParams()");
	}

	void SetCLOItems(ref array<string> items){
		m_StaticLoadoutItems = items;
	}

	void SetSelectedSpawnPoint(vector pnt){
		m_SelectedSpawnPoint = pnt;
	}

	void SetSLO(string str){
		m_SelectedStaticLO = str;
	}

	void SetSelectedCharacterSkin(string str){
		m_SelectedCharacterSkin = str;
	}

	string GetSelectedCharacterSkin(){
		return m_SelectedCharacterSkin;
	}

	vector GetSelectedSpawnPoint(){
		return m_SelectedSpawnPoint;
	}

	string GetSLO(){
		return m_SelectedStaticLO;
	}

	ref array<string> GetCLOItems(){
		return m_StaticLoadoutItems;
	}
};

static ref SpawnParams g_SpawnParams;
static void DestroySpawnParamsClass(){
	delete g_SpawnParams;
	g_SpawnParams = NULL;
}
static ref SpawnParams GetSpawnParams()
{
	if ( !g_SpawnParams || g_SpawnParams == NULL){
        g_SpawnParams = new SpawnParams;
    }
    return g_SpawnParams;
}