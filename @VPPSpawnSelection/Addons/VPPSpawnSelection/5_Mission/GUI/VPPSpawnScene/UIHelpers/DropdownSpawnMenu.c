class DropdownSpawnMenu extends ScriptedWidgetEventHandler
{
	protected Widget				m_Root;
	protected ScrollWidget			m_Scroller;
	protected Widget				m_ContentContainer;
	protected ref array<Widget>		m_Content		= new array<Widget>>;
	
	protected Widget				m_Button;
	protected string                m_TextCache;
	protected string 				m_DisplayNameCache;
	protected int 					m_IndexCache;
	protected TextWidget			m_Text;
	protected ImageWidget			m_ImageExpand;
	protected ImageWidget			m_ImageCollapse;
	protected ref array<string>		m_ContentArrStr;
	protected bool					m_IsExpanded;
	ref ScriptInvoker				m_OnSelectItem		= new ScriptInvoker();

	void DropdownSpawnMenu( Widget root, ref array<string> contentStrs, string text = "" )
	{
		m_Root				= GetGame().GetWorkspace().CreateWidgets( "VPPSpawnSelection/GUI/Layouts/dropdown_prefab_modded/dropdown_prefab.layout", root );

		m_Scroller			= ScrollWidget.Cast( m_Root.FindAnyWidget( "dropdown_container" ) );
		m_ContentContainer	= m_Root.FindAnyWidget( "dropdown_content" );
		m_Text				= TextWidget.Cast( m_Root.FindAnyWidget( "dropdown_text" ) );
		SetSelectedItem( text );
		
		m_Button			= m_Root.FindAnyWidget( "dropdown_selector_button" );
		m_ImageExpand		= ImageWidget.Cast( m_Root.FindAnyWidget( "expand_image" ) );
		m_ImageCollapse		= ImageWidget.Cast( m_Root.FindAnyWidget( "collapse_image" ) );
		
		m_Root.SetHandler( this );
		m_ContentArrStr = contentStrs;
		
		RefreshContent();
		
		//Add all elements on init
		foreach(string str : m_ContentArrStr){
			AddElement(g_Game.GetDisplayName(str));
		}
	}
	
	ref array<string> GetContentArray(){
		return m_ContentArrStr;
	}

	void Randomize()
	{
		int arrpos = Math.RandomIntInclusive(0, m_ContentArrStr.Count() - 1);
		SetSelectedItem(m_ContentArrStr.Get(arrpos));
		string displayname = g_Game.GetDisplayName(m_ContentArrStr.Get(arrpos));
		if (displayname != ""){
			SetDisplayItemName(displayname);
		}else{
			SetDisplayItemName(m_ContentArrStr.Get(arrpos));
		}
		m_OnSelectItem.Invoke( arrpos );
	}
	
	void UpdateListedItems(ref array<string> items)
	{
		RemoveAllElements();
		m_ContentArrStr = items;
		foreach(string str : items){
			AddElement(str);
		}
	}

	void RefreshContent()
	{
		Widget child = m_ContentContainer.GetChildren();
		while( child )
		{
			if( m_Content.Find( child ) > -1 )
			{
				m_Content.Insert( child );
			}
		}
		
		m_ContentContainer.Update();
		m_Root.Update();
		
		float x, y;
		m_ContentContainer.GetScreenSize( x, y );
		if( y > m_Scroller.GetContentHeight() )
		{
			m_Scroller.SetAlpha( 1 );
		}
		else
		{
			m_Scroller.SetAlpha( 0 );
		}
	}
	
	int AddElement( string text, Widget content = null )
	{
		ButtonWidget element = ButtonWidget.Cast( GetGame().GetWorkspace().CreateWidgets( "VPPSpawnSelection/GUI/Layouts/dropdown_prefab_modded/dropdown_element.layout", m_ContentContainer ) );
		element.SetText( text );
		
		if( content )
		{
			element.AddChild( content, false );
		}
		m_ContentContainer.Update();
		m_Root.Update();
		
		m_Content.Insert( element );
		return m_Content.Count() - 1;
	}
	
	void RemoveElement( int index )
	{
		if( 0 < index && index < m_Content.Count() )
		{
			delete m_Content.Get( index );
			m_ContentContainer.Update();
			if( 0 < index && index < m_ContentArrStr.Count() )
			{
				m_ContentArrStr.Remove( index );
			}
			m_Root.Update();
		}
	}

	void RemoveAllElements()
	{
	    for (int i = 0; i < m_Content.Count(); ++i)
	    {
	    	delete m_Content.Get( i );
			m_ContentContainer.Update();
			m_Root.Update();
	    }
	    m_Content	= new array<Widget>>;
		m_ContentArrStr = new array<string>;
	}
	
	void Close()
	{
		if( m_IsExpanded )
		{
			m_Scroller.Show( false );
			m_ImageExpand.Show( false );
			m_ImageCollapse.Show( true );
		}
	}

	/*
		use with `SetSelectedItem` for inventory items...this way we can store display name and cfg name call SetSelectedItem FIRST!
	*/
	void SetDisplayItemName(string text){
		m_Text.SetText( text );
		m_DisplayNameCache = text;
	}
	
	void SetSelectedItem( string text ){
		m_Text.SetText( text );
		m_TextCache = text;
	}

	string GetSelectedItem(){
		return m_TextCache;
	}

	string GetDisplayItemName(){
		return m_DisplayNameCache;
	}

	void SetIndex(int index){
		m_IndexCache = index;
	}

	int GetIndex(){
		return m_IndexCache;
	}

	bool IsExpanded(){
		return m_IsExpanded;
	}
	
	override bool OnClick( Widget w, int x, int y, int button )
	{
		int index = m_Content.Find( w );
		if( index > -1 )
		{
			m_OnSelectItem.Invoke( index );
			m_IsExpanded = false;
			m_Scroller.Show( m_IsExpanded );
			m_ImageExpand.Show( !m_IsExpanded );
			m_ImageCollapse.Show( m_IsExpanded );
			return true;
		}
		return false;
	}
	
	override bool OnMouseButtonUp( Widget w, int x, int y, int button )
	{
		if( w == m_Root && button == MouseState.LEFT )
		{
			m_IsExpanded = !m_IsExpanded;
			m_Scroller.Show( m_IsExpanded );
			m_ImageExpand.Show( !m_IsExpanded );
			m_ImageCollapse.Show( m_IsExpanded );
			
			m_Root.Update();
			return true;
		}
		return false;
	}
}