class HelpBoxWidget extends UIScriptedMenu
{
	private Widget          m_Root;    
	private Widget 			m_InfoPanelRoot;
	private TextWidget		m_InfoLabel;
	private RichTextWidget	m_InfoText;
	private Widget     		m_TargetWidget;
	private ImageWidget     m_TargetImageWidget;

	private string m_InnerText;
	private string m_TitleText;

	void HelpBoxWidget(Widget RootWidget, string TargetWidgetName, string TitleText, string InnerText)
	{
		m_Root			= GetGame().GetWorkspace().CreateWidgets( "gui/layouts/DZTest/VPPHelpBox.layout" );

		m_InfoPanelRoot = Widget.Cast( m_Root.FindAnyWidget( "Info_Panel" ) );
		m_InfoLabel	    = TextWidget.Cast( m_InfoPanelRoot.FindAnyWidget( "LabelText"  ) );
		m_InfoText	    = RichTextWidget.Cast( m_InfoPanelRoot.FindAnyWidget( "Info_Content" ) );

		m_InnerText     = InnerText;
		m_TitleText     = TitleText;

		if (TargetWidgetName == "InfoImage"){
			m_TargetImageWidget = ImageWidget.Cast( RootWidget.FindAnyWidget( TargetWidgetName ) );

			WidgetEventHandler.GetInstance().RegisterOnMouseEnter( m_TargetImageWidget,  this, "MouseEnter" );
    		WidgetEventHandler.GetInstance().RegisterOnMouseLeave( m_TargetImageWidget,  this, "MouseLeave" );
		}else{
			m_TargetWidget  = Widget.Cast( RootWidget.FindAnyWidget( TargetWidgetName ) );

			WidgetEventHandler.GetInstance().RegisterOnMouseEnter( m_TargetWidget,  this, "MouseEnter" );
    		WidgetEventHandler.GetInstance().RegisterOnMouseLeave( m_TargetWidget,  this, "MouseLeave" );
		}
	}

	void ~HelpBoxWidget()
	{
		Print("HelpBoxWidget Destroyed.");
	}

	void Destroy(){
		delete this;
	}

	void SetTitle(string NewTitle)
	{
		m_TitleText = NewTitle;
	}

	string GetTitle(){
		return m_TitleText;
	}

	void SetInnerText(string NewInnerText){
		m_InnerText = NewInnerText;
	}

	string GetInnerText(){
		return m_InnerText;
	}

	bool MouseEnter( Widget w, int x, int y ){
		int m_x, m_y;
		g_Game.GetMousePos(m_x, m_y);
    	m_Root.SetPos(m_x,m_y);

		m_InfoPanelRoot.Show(true);
		m_InfoLabel.SetText( m_TitleText );
		m_InfoText.SetText( m_InnerText );

		m_Root.Update();
		m_InfoPanelRoot.Update();
		m_InfoLabel.Update();
		m_InfoText.Update();
		return true;
	}
	
	bool MouseLeave( Widget w, Widget s, int x, int y){
		m_InfoPanelRoot.Show(false); return true;
		return false;
	}
}