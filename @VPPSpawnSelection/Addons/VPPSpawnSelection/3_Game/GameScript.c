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
			GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(this.ClientOnSpawnScreen);
			Print("--->> SPAWN SCREEN SAYS PLAYER IS READY...");
		}
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