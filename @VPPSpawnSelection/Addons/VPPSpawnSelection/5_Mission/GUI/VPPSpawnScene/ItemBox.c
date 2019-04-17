class ItemBox
{
	private ref PlayerCustomizationTab m_ParentClass;
	private string 				  	   m_DropDownWidgetName;
	private string 					   m_SlotType;
	private ref Widget 				   m_RootLayout;
	protected Widget 				   m_DropDownWidget;
	protected ref DropdownSpawnMenu    m_DropDown;
	private string m_SelectedItem;
	private ItemPreviewWidget  		   m_ItemPreviewWidget;
	private ItemBase            	   m_PreviewItem;

	private vector m_ItemOrientation;
	private int    m_RotationX;
	private int    m_RotationY;

	void ItemBox(ref PlayerCustomizationTab parentClass, ref Widget rootLayout, string DropdownWidgetName, string slotType, ref array<string> content, string PreviewWidgetName)
	{
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(this.OnUpdate);

		m_RootLayout		  = rootLayout;
		m_ParentClass 		  = parentClass;
		m_DropDownWidgetName  = DropdownWidgetName;
		m_SlotType			  = slotType;
		m_DropDownWidget	  = Widget.Cast( m_RootLayout.FindAnyWidget( m_DropDownWidgetName ) );
		m_ItemPreviewWidget   = Widget.Cast( m_RootLayout.FindAnyWidget( PreviewWidgetName ) );
		m_DropDown = new DropdownSpawnMenu( m_DropDownWidget, content, slotType );
		m_DropDown.m_OnSelectItem.Insert( OnSelect );

		WidgetEventHandler.GetInstance().RegisterOnMouseButtonDown( m_ItemPreviewWidget, this, "MouseButtonDown" );
		WidgetEventHandler.GetInstance().RegisterOnDoubleClick( m_ItemPreviewWidget, this, "RemoveItem" );
	}

	void ~ItemBox() 
	{
		if (m_PreviewItem){
			GetGame().ObjectDelete(m_PreviewItem);
		}
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(this.OnUpdate);
	}

	protected void OnUpdate(float timeslice)
	{
		if (m_DropDown.IsExpanded()){
			m_ItemPreviewWidget.Show(false);
		}else{
			m_ItemPreviewWidget.Show(true);
		}
	}

	void SelectRandom()
	{
		m_DropDown.Randomize();
	}

	void OnSelect(int index)
	{
		SetItem(m_DropDown.GetContentArray().Get(index));

		m_DropDown.SetSelectedItem( m_DropDown.GetContentArray().Get(index) );	//For use of configs
		m_DropDown.SetDisplayItemName( g_Game.GetDisplayName(m_DropDown.GetContentArray().Get(index)) ); //For show
		m_DropDown.Close();

		//update preview box & item on player
		m_ParentClass.UpdateDemoItems(m_SlotType,m_SelectedItem);
		UpdateItemPreview();
	}

	void RemoveItem()
	{
		//update preview box & item on player
		m_SelectedItem = "";
		m_ParentClass.UpdateDemoItems(m_SlotType,m_SelectedItem);
		UpdateItemPreview();
	}

	void UpdateItemPreview()
	{	
		m_ItemPreviewWidget.SetItem( NULL );
		if (m_PreviewItem){
			GetGame().ObjectDelete(m_PreviewItem);
		}

		EntityAI entity;
		entity = GetGame().CreateObject( m_SelectedItem, "0 0 0", true, false );
		if ( Class.CastTo(m_PreviewItem,entity) ){
			m_ItemPreviewWidget.SetItem( m_PreviewItem );
			m_ItemPreviewWidget.SetView( m_PreviewItem.GetViewIndex() );
			m_ItemPreviewWidget.SetModelPosition( Vector(-5,0,0) );
			m_ItemPreviewWidget.SetModelOrientation( m_ItemOrientation );
		}
	}

	void UpdateItemRotation(int mouse_x, int mouse_y, bool is_dragging)
	{
		vector orientation;
		if (m_ItemOrientation[0] == 0 && m_ItemOrientation[1] == 0 && m_ItemOrientation[2] == 0)
		{
			orientation        = m_ItemPreviewWidget.GetModelOrientation();
			m_ItemOrientation  = m_ItemPreviewWidget.GetModelOrientation();
		}else{
			orientation = m_ItemOrientation;
		}
		
		orientation[0] = orientation[0] + (m_RotationY - mouse_y);
		orientation[1] = orientation[1] - (m_RotationX - mouse_x);

		m_ItemPreviewWidget.SetModelOrientation( orientation );

		if ( !is_dragging )
		{	
			m_ItemOrientation = orientation;
		}
	}

	bool MouseButtonDown()
	{
		g_Game.GetMousePos( m_RotationX, m_RotationY );
		GetGame().GetDragQueue().Call( this, "UpdateItemRotation" );
		return true;
	}

	void SetItem(string str){
		m_SelectedItem = str;
	}

	string GetItem(){
		return m_SelectedItem;
	}
};