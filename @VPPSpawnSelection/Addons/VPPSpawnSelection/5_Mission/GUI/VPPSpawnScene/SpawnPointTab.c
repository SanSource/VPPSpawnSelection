class SpawnPointTab extends TabsBase
{
	private	string       M_TAB_NAME;
	private Widget 		 m_Parent;
	private Widget  	 m_Root;
	private bool    	 m_Showing;
	private ButtonWidget m_ButtonHide;
	private TextWidget   m_Title;
	private ImageWidget  m_ImageLock;
	private EditBoxWidget  m_InputSelectedPoint;
	private EditBoxWidget  m_InputCooldown;
	private CheckBoxWidget m_ChkRandPoint;
	private EditBoxWidget  m_InputCLCount;
	private MapWidget      m_MapWidget;
	private string m_IconWayPoint = "VPPSpawnSelection\\GUI\\Textures\\CustomMapIcons\\waypoint_CA.paa";

	private bool  m_PointStatus    = false;
	private float m_PointTimeLeft  = 0.0;
	private float m_LocalTick	   = 0.0;
	private float m_TickerInterval = 1000;

	private ref array<string> m_SpawnPointsName;
	private Widget 					m_Widget_PositionsDropDown;
	protected ref DropdownSpawnMenu m_PositionsDropDown;

	void SpawnPointTab(Widget father, string tabName, string tabTitle)
	{
		M_TAB_NAME   = tabName;
		m_Parent     = father;
		m_Root       = GetGame().GetWorkspace().CreateWidgets( "VPPSpawnSelection/GUI/Layouts/SpawnSelection_SpawnPointsPanel.layout", father);

		//---RPCs---
		GetRPCManager().AddRPC( "RPC_CooldownInfo", "CooldownInfo", this, SingeplayerExecutionType.Server );
		//---
		m_ButtonHide = ButtonWidget.Cast( m_Root.FindAnyWidget( "ButtonHide" ) );
		m_Title      = TextWidget.Cast( m_Root.FindAnyWidget( "TitleText" ) );
		m_Title.SetText(tabTitle);
		m_Title.SetItalic(true);
		m_InputSelectedPoint = EditBoxWidget.Cast( m_Root.FindAnyWidget( "InputSelectedPoint" ) );
		m_InputCooldown		 = EditBoxWidget.Cast( m_Root.FindAnyWidget( "InputCooldown" ) );
		m_InputCLCount		 = EditBoxWidget.Cast( m_Root.FindAnyWidget( "InputCLCount" ) );
		m_MapWidget			 = MapWidget.Cast( m_Root.FindAnyWidget( "MapWidget" ) );
		ShowTab(false);
		m_ImageLock = ImageWidget.Cast( m_Root.FindAnyWidget( "ImageLock" ) );
		WidgetEventHandler.GetInstance().RegisterOnMouseButtonDown( m_ButtonHide,  this, "OnClick" );

		m_SpawnPointsName = GetLocalConfigInstance().GetAllSpawnPointsNames();

		m_ChkRandPoint = CheckBoxWidget.Cast( m_Root.FindAnyWidget( "ChkRandPoint" ) );
		m_Widget_PositionsDropDown = Widget.Cast( m_Root.FindAnyWidget( "PositionsList" ) );
		m_PositionsDropDown = new DropdownSpawnMenu( m_Widget_PositionsDropDown, m_SpawnPointsName, "Spawn Location:" );
		m_PositionsDropDown.m_OnSelectItem.Insert( OnSelectPosition );
		SetLockIcon(false);

		RefreshMarkers();
	}

	void ~SpawnPointTab()
	{
		
	}

	void AddMarker(string markerName, vector pos, vector color)
	{
		float R,G,B;
		R = color[0];
		G = color[1];
		B = color[2];
		m_MapWidget.AddUserMark(pos, markerName, ARGB(255,R,G,B), m_IconWayPoint);
	}

	void RefreshMarkers(){
		m_MapWidget.ClearUserMarks();
		ref array<ref SpawnPointDetails> SpawnPoints = GetLocalConfigInstance().GetSpawnPoints();
		foreach(ref SpawnPointDetails sp: SpawnPoints){
			AddMarker(sp.GetName(),sp.GetPosition(),Vector(0,255,0));
		}
	}

	void OnSelectPosition(int index)
	{
		string spawnPointName = GetLocalConfigInstance().GetSpawnPointByName(m_PositionsDropDown.GetContentArray().Get(index)).GetName();
		//Get Cooldown info
		GetRPCManager().SendRPC( "RPC_GetCooldownStatus", "GetCooldownStatus", new Param1<string>(spawnPointName), true, NULL);

		m_InputSelectedPoint.SetText(spawnPointName);
		m_InputCooldown.SetText(ConvertToMinutes(GetLocalConfigInstance().GetSpawnPointByName(spawnPointName).GetCooldown()).ToString() + " Minutes");
		
		//update config
		GetSpawnParams().SetSelectedSpawnPoint(GetLocalConfigInstance().GetSpawnPointByName(spawnPointName).GetPosition());

		m_PositionsDropDown.SetSelectedItem( m_PositionsDropDown.GetContentArray().Get(index) );
		m_PositionsDropDown.Close();
	}

	void CooldownInfo( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target )
	{
		Param2<float,bool> data;
        if ( !ctx.Read( data ) ) return;

        if (type == CallType.Client){
        	m_PointTimeLeft  = data.param1;
			m_PointStatus    = data.param2;
			SetLockIcon(m_PointStatus);
			m_InputCLCount.Show(m_PointStatus);
			m_InputCLCount.Update();
        }
	}

	static float ConvertToMinutes(float secs){
		return Math.Round(secs / 60);
	}

	void SetLockIcon(bool state){
		if (state){
			m_ImageLock.LoadImageFile(0,"set:dayz_gui image:icon_locked_sb");
			m_ImageLock.SetColor( ARGBF(1, 1.0, 0.0, 0.0) );
		}else{
			m_ImageLock.LoadImageFile(0,"VPPSpawnSelection\\GUI\\Textures\\icon_lock_open.paa");
			m_ImageLock.SetColor( ARGBF(1, 0.0, 1.0, 0.0) );
		}
		m_ImageLock.Update();
	}

	override string GetTabName(){
		return M_TAB_NAME;
	}

	override bool IsOpen(){
		return m_Showing;
	}

	override void ShowTab(bool b){
		m_Showing = b;
		m_Root.Show(m_Showing);
		if (m_Showing){
			SetFocusedWidget(m_Root);
		}
	}

	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);
		float currentSecond = GetGame().GetTime();

		if (m_ChkRandPoint.IsChecked()){
			GetSpawnParams().SetSelectedSpawnPoint("0 0 0"); //Triggers random pos
			m_Widget_PositionsDropDown.Show(false);
			TextWidget.Cast(m_Root.FindAnyWidget("PositionListTxt" )).Show(false);
			SetLockIcon(false);
			m_InputCLCount.Show(false);
			m_InputCLCount.Update();
		}else{
			m_Widget_PositionsDropDown.Show(true);
			TextWidget.Cast(m_Root.FindAnyWidget("PositionListTxt" )).Show(true);
		}

		if (m_PointStatus){
			GetSpawnParams().SetSelectedSpawnPoint("0 0 0"); //Triggers random pos if point is locked/has cooldown
		}

		if (m_PointStatus && (currentSecond - m_LocalTick) >= m_TickerInterval){
			//Tick tock local cooldown for display
			m_PointTimeLeft--;
			m_InputCLCount.SetText(ConvertToMinutes(m_PointTimeLeft).ToString() + " mins left");
			m_LocalTick = currentSecond;
		}
	}

	bool OnClick( Widget w, int x, int y, int button ){
		if (w == m_ButtonHide){
			ShowTab(false); return true;
		}
		return false;
	}
};