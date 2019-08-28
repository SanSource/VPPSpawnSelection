modded class MissionGameplay
{
	void MissionGameplay()
	{
		GetRPCManager().AddRPC( "RPC_HandleSettings", "HandleSettings", this, SingeplayerExecutionType.Server );
        GetRPCManager().SendRPC( "RPC_MissionServer", "test", NULL, true);
	}
	
	void HandleSettings( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target )
	{
		Param1<ref SSConfig> data;
        if ( !ctx.Read( data ) ) return;

        if (type == CallType.Client)
        {
        	g_LocalConfigInstance = data.param1;
        }
    }

    void ManageHud(bool state)
	{
        g_Game.GetUIManager().ShowCursor(state);
 		GetGame().GetMission().GetHud().Show( state );
    }

	override void OnUpdate(float timeslice) {
	    super.OnUpdate(timeslice);
	    Input input = GetGame().GetInput();
	
	    if ( input.LocalPress("UAOpenVPPSpawnSelection", false) ) {
			g_Game.ShowSpawnScene();
	    }
	}
}

static ref SSConfig g_LocalConfigInstance;

static ref SSConfig GetLocalConfigInstance(){
    return g_LocalConfigInstance;
}    