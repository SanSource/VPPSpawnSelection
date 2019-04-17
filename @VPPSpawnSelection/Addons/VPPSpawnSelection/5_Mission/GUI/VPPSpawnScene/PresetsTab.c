class PresetsTab extends TabsBase
{
	private	string       M_TAB_NAME;
	private Widget 		 m_Parent;
	private Widget  	 m_Root;
	private bool    	 m_Showing;
	private ButtonWidget m_ButtonHide;
	private TextWidget   m_Title;

	void PresetsTab(Widget father, string tabName, string tabTitle)
	{
		M_TAB_NAME   = tabName;
		m_Parent     = father;
		m_Root       = GetGame().GetWorkspace().CreateWidgets( "VPPSpawnSelection/GUI/Layouts/SpawnSelection_PresetPanel.layout", father);

		m_ButtonHide = ButtonWidget.Cast( m_Root.FindAnyWidget( "ButtonHide" ) );
		m_Title      = TextWidget.Cast( m_Root.FindAnyWidget( "TitleText" ) );
		m_Title.SetText(tabTitle);
		m_Title.SetItalic(true);
		ShowTab(false);

		WidgetEventHandler.GetInstance().RegisterOnMouseButtonDown( m_ButtonHide,  this, "OnClick" );
	}

	void ~PresetsTab()
	{
		
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

	bool OnClick( Widget w, int x, int y, int button ){
		if (w == m_ButtonHide){
			ShowTab(false); return true;
		}
		return false;
	}
};