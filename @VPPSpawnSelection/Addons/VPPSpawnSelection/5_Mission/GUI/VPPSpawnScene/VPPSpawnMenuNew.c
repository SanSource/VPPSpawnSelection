class VPPSpawnMenuNew extends UIScriptedMenu
{
	//----------------Scene-Cam-----------------
	private Camera 			m_Camera;
	private vector 			m_CameraTrans[4];
	private vector 			m_Target;
	private vector 			m_LookAtPos;
	//-----------------------------------------
	private vector 			m_DemoUnitPos;

	void VPPSpawnMenuNew()
	{
		g_Game.GetUIManager().ShowUICursor(true);
		GetGame().GetMission().PlayerControlDisable(INPUT_EXCLUDE_ALL);
        GetGame().GetInput().ChangeGameFocus( 1 );
        SetupScene();

        MissionGameplay.Cast( GetGame().GetMission() ).ManageHud(false);
	}

	void ~VPPSpawnMenuNew()
	{
		GetGame().GetMission().PlayerControlEnable();
        g_Game.GetUIManager().ShowUICursor(false);
        GetGame().GetInput().ResetGameFocus();
		GetGame().GetUIManager().Back();
		DestroyScene();
		
		MissionGameplay.Cast( GetGame().GetMission() ).ManageHud(true);

		g_RootTabClass = NULL;
	}

	void DestroyScene()
	{
		SetFocus( null );
		m_Camera.SetActive(false);
		GetGame().ObjectDelete(m_Camera);
	}

	void SetupScene()
	{
		string root_path = "cfgVPPCharacterScenes " + g_Game.GetWorldName();
		string childName;
		g_Game.ConfigGetChildName(root_path, Math.RandomIntInclusive(0, g_Game.ConfigGetChildrenCount(root_path) - 1), childName);
	
		string scene_path = root_path + " " + childName;
		m_Target = g_Game.ConfigGetVector(scene_path + " target");
		vector position = g_Game.ConfigGetVector(scene_path + " position");
		m_DemoUnitPos = position;
		vector SnappedPos = g_Game.SnapToGround(position);
		float fov = g_Game.ConfigGetFloat(scene_path + " fov");
		vector FinalPos;
		FinalPos[0] = SnappedPos[0];
		FinalPos[1] = SnappedPos[1]; //FinalPos[1] = SnappedPos[1] + 175;
		FinalPos[2] = SnappedPos[2];

		Class.CastTo(m_Camera, g_Game.CreateObject("staticcamera", FinalPos, true));
		Math3D.MatrixIdentity4(m_CameraTrans);
		
		if (m_Camera)
		{
			m_Camera.LookAt(m_Target);
			m_Camera.SetFOV(fov);
			//m_Camera.SetFocus(2.0, 0.0);
	
			m_Camera.SetActive(true);
			
			Math3D.DirectionAndUpMatrix(m_Target - FinalPos, "0 1 0", m_CameraTrans);
			m_CameraTrans[3] = m_Camera.GetPosition();
			m_LookAtPos = Vector(0.685547, -0.988281, 3.68823).Multiply4(m_CameraTrans);

			float pos_x = m_LookAtPos[0];
			float pos_z = m_LookAtPos[2];
			float pos_y = GetGame().SurfaceY(pos_x, pos_z);
			vector ground_demo_pos = Vector(pos_x, pos_y, pos_z);
			m_LookAtPos = ground_demo_pos;
			//m_Camera.LookAt(m_LookAtPos);
		}
		//Vignette
		PPEffects.SetVignette(0.0, 0,0,0);
		PPEffects.Init();
		PPEffects.DisableBurlapSackBlindness();
	}

	override Widget Init()
    {
        layoutRoot = GetGame().GetWorkspace().CreateWidgets( "VPPSpawnSelection/GUI/Layouts/SpawnSceneMain.layout" );

		//Create sub tab instances (player, spawn point, preset tabs) tab is set to NOT show on init.
		GetRootTabClass().InsertTab( new ref TabsManager(layoutRoot, "TabsManager", "") ); //Tab manager
		GetRootTabClass().InsertTab( new ref PlayerCustomizationTab(layoutRoot,"PlayerTab","Player Customization",m_DemoUnitPos));
		GetRootTabClass().InsertTab( new ref PresetsTab(layoutRoot,"PresetsTab","Character Presets"));
		GetRootTabClass().InsertTab( new ref SpawnPointTab(layoutRoot,"SpawnPointTab","Spawn Point Selection"));
		return layoutRoot;
	}

	override void Update(float timeslice)
	{
		super.Update(timeslice);
		foreach(ref TabsBase tab: GetRootTabClass().m_AllTabs){
			tab.OnUpdate(timeslice);
		}
	}
};

static ref TabsBase g_RootTabClass;
static ref TabsBase GetRootTabClass()
{
    if ( !g_RootTabClass || g_RootTabClass == NULL){
        g_RootTabClass = new TabsBase;
    }
    return g_RootTabClass;
}

static void SetFocusedWidget(Widget w)
{
	g_RootTabClass.m_FocusedWidget = w;
}
static ref Widget GetFocusedWidget()
{
	return g_RootTabClass.m_FocusedWidget;
}