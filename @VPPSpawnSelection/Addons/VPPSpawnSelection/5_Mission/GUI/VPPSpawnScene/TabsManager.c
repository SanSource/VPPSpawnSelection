class TabsManager extends TabsBase
{
	private Widget 		 m_Root;
	private Widget 		 m_Parent;
	private Widget 		 m_TabsPanel;
	private ButtonWidget m_PlayerTabBtn;
	private ButtonWidget m_PositionTabBtn;
	private ButtonWidget m_PresetsTabBtn;
	private ImageWidget  m_InfoImage;
	private ButtonWidget m_SpawnButton;

	private ref array<ref HelpBoxWidget> m_HelpBoxes;

	void TabsManager(Widget parentroot, string tabName, string tabTitle)
	{
		m_Parent      = parentroot;
		m_Root        = GetGame().GetWorkspace().CreateWidgets( "VPPSpawnSelection/GUI/Layouts/SpawnSelection_Tabs.layout", parentroot);

		m_TabsPanel   = Widget.Cast( m_Root.FindAnyWidget( "TabsPanel" ) );
		m_PlayerTabBtn   = ButtonWidget.Cast( m_Root.FindAnyWidget( "PlayerTabBtn" ) );
		m_PositionTabBtn = ButtonWidget.Cast( m_Root.FindAnyWidget( "PositionTabBtn" ) );
		m_PresetsTabBtn  = ButtonWidget.Cast( m_Root.FindAnyWidget( "PresetsTabBtn" ) );
		m_SpawnButton    = ButtonWidget.Cast( m_Root.FindAnyWidget( "ButtonSpawnIn" ) );
		
		m_PositionTabBtn.Enable(!GetLocalConfigInstance().ForcedRandomSpawns());

		m_InfoImage = ImageWidget.Cast( m_Root.FindAnyWidget( "InfoImage" ) );
        m_InfoImage.LoadImageFile(0,"set:ccgui_enforce image:FilterCurrentObjectivesFocus");

        SetFocusedWidget(m_Root);
		m_HelpBoxes = new array<ref HelpBoxWidget>;
		m_HelpBoxes.Insert( new HelpBoxWidget(GetFocusedWidget(),"PlayerTabBtn","Info:","Allows you to change your character skin. Choose gear/loadouts and save presets!") );
		m_HelpBoxes.Insert( new HelpBoxWidget(GetFocusedWidget(),"PositionTabBtn","Info:","Choose your spawn location! (The Administrator of the server has full control over the spawn points and cooldown!). If the option is colored black that means its disabled by administrator!\n") );
		m_HelpBoxes.Insert( new HelpBoxWidget(GetFocusedWidget(),"PresetsTabBtn","Info:","Here you can edit your saved player presets!") );
		m_HelpBoxes.Insert( new HelpBoxWidget(GetFocusedWidget(),"InfoImage","Info:","Staying idle in the menu for more than 5 minutes will cause a kick! If a spawn point is locked/has cooldown and you click respawn you will get a random position. If you choose to select a random spawn point, cooldown will not apply to the point that will be picked.\n") );

		WidgetEventHandler.GetInstance().RegisterOnMouseButtonDown( m_PlayerTabBtn,  this, "OnClick" );
		WidgetEventHandler.GetInstance().RegisterOnMouseButtonDown( m_PositionTabBtn,  this, "OnClick" );
		WidgetEventHandler.GetInstance().RegisterOnMouseButtonDown( m_PresetsTabBtn,  this, "OnClick" );
		WidgetEventHandler.GetInstance().RegisterOnMouseButtonDown( m_SpawnButton,  this, "OnClick" );

	}

	void ~TabsManager()
	{
		foreach(ref HelpBoxWidget helpbox: m_HelpBoxes){
			if (helpbox != NULL) helpbox.Destroy();
		}
	}

	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);
	}

	bool OnClick(Widget w, int x, int y, int button){
		GetRootTabClass().HideAllTabs();
		switch(w)
		{
			case m_SpawnButton:
			GetGame().GetUIManager().Back(); //Close Root menu
			g_Game.SetSpawnStatus(true);
			return true;
			break;

			case m_PlayerTabBtn:
			if (GetRootTabClass().IsTabOpen("PlayerTab")){
				GetRootTabClass().ShowTabByName("PlayerTab",false);
			}else{
				GetRootTabClass().ShowTabByName("PlayerTab",true);
			}
			return true;
			break;

			case m_PositionTabBtn:
			if (GetLocalConfigInstance().ForcedRandomSpawns()) return false;

			if (GetRootTabClass().IsTabOpen("SpawnPointTab")){
				GetRootTabClass().ShowTabByName("SpawnPointTab",false);
			}else{
				GetRootTabClass().ShowTabByName("SpawnPointTab",true);
			}
			return true;
			break;

			case m_PresetsTabBtn:
			if (GetRootTabClass().IsTabOpen("PresetsTab")){
				GetRootTabClass().ShowTabByName("PresetsTab",false);
			}else{
				GetRootTabClass().ShowTabByName("PresetsTab",true);
			}
			return true;
			break;
		}
		return false;
	}
}