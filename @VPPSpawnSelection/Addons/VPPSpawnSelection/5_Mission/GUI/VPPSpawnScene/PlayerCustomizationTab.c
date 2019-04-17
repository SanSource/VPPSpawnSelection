class PlayerCustomizationTab extends TabsBase
{
	private	string       M_TAB_NAME;
	private Widget 		 m_Parent;
	private Widget  	 m_Root;
	private bool    	 m_Showing;
	private ButtonWidget m_ButtonHide;
	private ButtonWidget m_ButtonRandomize;
	private ButtonWidget m_BtnSelectRandSL;
	private TextWidget   m_Title;
	private vector m_CharacterOrientation;
	private int m_CharacterRotationX,m_CharacterRotationY;
	private PlayerPreviewWidget m_PlayerPreview;
	private vector  			m_DemoPos;
	private PlayerBase 		    m_DemoPlayer;
	protected ref DropdownSpawnMenu m_CharactersDropDown;
	private string m_PreviewPlayerClassName;
	ref array<string> m_FemaleChars;
	ref array<string> m_MaleChars;
	private ref array<int> m_InventoryEnums = new array<int>;
	private XComboBoxWidget      m_GenderCombo;
	private Widget      	     m_HeadList;
	private ImageWidget          m_ImgSelection;
	private Widget 			     m_PanelStaticLoadouts;
	private Widget 			     m_StaticLoadoutsList;
	protected ref DropdownSpawnMenu m_SLODropDown;

	private ref array<ref ItemBox> 	m_SelectionBoxes;
	
	void PlayerCustomizationTab(Widget father, string tabName, string tabTitle, vector demoPos)
	{
		M_TAB_NAME   = tabName;
		m_Parent     = father;
		m_Root       = GetGame().GetWorkspace().CreateWidgets( "VPPSpawnSelection/GUI/Layouts/SpawnSelection_PlayerPanel.layout", father);
		m_DemoPos    = demoPos;
		ShowTab(false);

		g_Game.PreloadObject( "SurvivorM_Mirek", 1000.0);
		m_DemoPlayer    = PlayerBase.Cast( GetGame().CreateObject( "SurvivorM_Mirek", m_DemoPos, true, false ) );
		m_PlayerPreview = PlayerPreviewWidget.Cast( m_Root.FindAnyWidget( "PlayerPreview" ) );
		m_PlayerPreview.SetPlayer( m_DemoPlayer );
		m_PlayerPreview.SetModelPosition( "0 0 0.45" );
		m_PlayerPreview.SetSize( 0.80, 0.80 );

        if (GetLocalConfigInstance().LoadoutSelectionType() == 1){
        	//CustomizableLoadouts
        	m_ImgSelection = ImageWidget.Cast( m_Root.FindAnyWidget( "ImgSelection" ) );
			m_ImgSelection.LoadImageFile(0,"VPPSpawnSelection\\GUI\\Textures\\arrows_white.paa");
			m_ImgSelection.Show(true);

			m_ButtonRandomize = ButtonWidget.Cast( m_Root.FindAnyWidget( "ButtonRandomize" ) );
			m_ButtonRandomize.Show(true);
			WidgetEventHandler.GetInstance().RegisterOnMouseButtonDown( m_ButtonRandomize,  this, "OnClick" );

			//Show boxes
			TStringArray strs = {"FeetPanel","PantsPanel","TopsPanel","VestPanel","HeadGearPanel","GlovesPanel","BagsPanel","FacePanel"};
			foreach(string wName : strs){
				Widget.Cast(m_Root.FindAnyWidget(wName)).Show(true);
			}

			m_SelectionBoxes = new array<ref ItemBox>;
			m_SelectionBoxes.Insert( new ItemBox(this, m_Root, "FeetList", "Feet", GetLocalConfigInstance().GetCustomizableItemsNamesBySlot("Feet"), "ItemFeet") );
			m_SelectionBoxes.Insert( new ItemBox(this, m_Root, "PantsList", "Legs", GetLocalConfigInstance().GetCustomizableItemsNamesBySlot("Legs"), "ItemPants") );
			m_SelectionBoxes.Insert( new ItemBox(this, m_Root, "TopsList", "Body", GetLocalConfigInstance().GetCustomizableItemsNamesBySlot("Body"), "ItemTop") );
			m_SelectionBoxes.Insert( new ItemBox(this, m_Root, "VestsList", "Vest", GetLocalConfigInstance().GetCustomizableItemsNamesBySlot("Vest"), "ItemVest") );
			m_SelectionBoxes.Insert( new ItemBox(this, m_Root, "HeadGearList", "HeadGear", GetLocalConfigInstance().GetCustomizableItemsNamesBySlot("Headgear"), "ItemHeadGear") );
			m_SelectionBoxes.Insert( new ItemBox(this, m_Root, "GlovesList", "Gloves", GetLocalConfigInstance().GetCustomizableItemsNamesBySlot("Gloves"), "ItemGloves") );
			m_SelectionBoxes.Insert( new ItemBox(this, m_Root, "BagsList", "Back", GetLocalConfigInstance().GetCustomizableItemsNamesBySlot("Back"), "ItemBag") );
			m_SelectionBoxes.Insert( new ItemBox(this, m_Root, "FaceList", "Eyewear", GetLocalConfigInstance().GetCustomizableItemsNamesBySlot("Eyewear"), "ItemFace") );

			RandomizeCLO();
        }else{
        //StaticLoadouts
        	m_PanelStaticLoadouts = Widget.Cast( m_Root.FindAnyWidget( "PanelStaticLoadouts" ) );
        	m_PanelStaticLoadouts.Show(true);

        	m_BtnSelectRandSL = ButtonWidget.Cast( m_Root.FindAnyWidget( "BtnSelectRandSL" ) );
        	m_StaticLoadoutsList = Widget.Cast( m_Root.FindAnyWidget( "StaticLOList" ) );
        	WidgetEventHandler.GetInstance().RegisterOnMouseButtonDown( m_BtnSelectRandSL,  this, "OnClick" );

        	m_SLODropDown = new DropdownSpawnMenu( m_StaticLoadoutsList, GetLocalConfigInstance().GetAllStaticLoadoutsNames(), "Select a loadout:" );
			m_SLODropDown.m_OnSelectItem.Insert( OnSelectSLO );
        }

		m_GenderCombo = XComboBoxWidget.Cast( m_Root.FindAnyWidget( "GenderCombo" ) );
        m_HeadList    = Widget.Cast( m_Root.FindAnyWidget( "HeadList" ) );
		m_ButtonHide   = ButtonWidget.Cast( m_Root.FindAnyWidget( "ButtonHide" ) );
		m_Title        = TextWidget.Cast( m_Root.FindAnyWidget( "TitleText" ) );
		m_Title.SetText(tabTitle);
		m_Title.SetItalic(true);

		WidgetEventHandler.GetInstance().RegisterOnMouseButtonDown( m_PlayerPreview,  this, "RotateDemoModel" );
		WidgetEventHandler.GetInstance().RegisterOnMouseButtonDown( m_ButtonHide,  this, "OnClick" );
		WidgetEventHandler.GetInstance().RegisterOnClick( m_GenderCombo,  this, "OnClick" );

		m_CharactersDropDown = new DropdownSpawnMenu( m_HeadList, {"null"}, "SurvivorM_Mirek" );
		m_CharactersDropDown.m_OnSelectItem.Insert( OnCharacterSelect );
		SortCharacters(); 	    //Fill arrays with all avaiable chars
		UpdateCharactersList(); //update drop down list

		ShowTab(true);

		m_InventoryEnums.Insert(InventorySlots.SHOULDER);
		m_InventoryEnums.Insert(InventorySlots.MELEE);
		m_InventoryEnums.Insert(InventorySlots.HEADGEAR);
		m_InventoryEnums.Insert(InventorySlots.MASK);
		m_InventoryEnums.Insert(InventorySlots.EYEWEAR);
		m_InventoryEnums.Insert(InventorySlots.GLOVES);
		m_InventoryEnums.Insert(InventorySlots.ARMBAND);
		m_InventoryEnums.Insert(InventorySlots.BODY);
		m_InventoryEnums.Insert(InventorySlots.VEST);
		m_InventoryEnums.Insert(InventorySlots.BACK);
		m_InventoryEnums.Insert(InventorySlots.LEGS);
		m_InventoryEnums.Insert(InventorySlots.FEET);
	}

	void ~PlayerCustomizationTab()
	{
		if (m_DemoPlayer) GetGame().ObjectDelete(m_DemoPlayer);
	}

	//Deletes item from specifc slot and updates array for config
	void UpdateDemoItems(string slotType, string UpdateItem)
	{
		ItemBase ItemInSlot;
		if ( Class.CastTo(ItemInSlot,m_DemoPlayer.FindAttachmentBySlotName(slotType)) ){
			GetGame().ObjectDelete(ItemInSlot);
		}

		if (UpdateItem != "") m_DemoPlayer.GetHumanInventory().CreateInInventory(UpdateItem);

		//update config items
		ref array<string> SelectedItems = new array<string>;
		foreach(ref ItemBox ib : m_SelectionBoxes){
			SelectedItems.Insert(ib.GetItem());
		}
		GetSpawnParams().SetCLOItems(SelectedItems);
	}

	void ClearDemoUnitInventory()
    {
		if ( m_DemoPlayer.GetInventory() )
		{
			ItemBase item;
			foreach(int InvEnum : m_InventoryEnums){
				if ( Class.CastTo(item,m_DemoPlayer.GetHumanInventory().FindAttachment(InvEnum)) ){
					GetGame().ObjectDelete(item);
				}
			}
			ItemBase item_in_hands = ItemBase.Cast( m_DemoPlayer.GetHumanInventory().GetEntityInHands() );
			if ( item_in_hands ){
				m_DemoPlayer.LocalDestroyEntityInHands();
			}
		}
    }

	void RandomizeCLO()
	{
		//Clear Invetory first
		ClearDemoUnitInventory();
		ref array<string> updatedItems = new array<string>;
		foreach(ref ItemBox ib : m_SelectionBoxes){
			ib.SelectRandom();
			updatedItems.Insert(ib.GetItem());
		}
		GetSpawnParams().SetCLOItems(updatedItems);
	}

	void OnSelectSLO(int index)
	{
		if (index != -1){
			string selectedLoadout = m_SLODropDown.GetContentArray().Get(index);
			m_SLODropDown.SetSelectedItem( selectedLoadout );
			m_SLODropDown.Close();
			GetSpawnParams().SetSLO(selectedLoadout);
		}
		
		//Clear Invetory first
		ClearDemoUnitInventory();
		ref StaticLoadout m_Loadout = GetLocalConfigInstance().GetStaticLoadoutByName(m_SLODropDown.GetSelectedItem());
		//Spawn the items for the demo
		ref array<ref LoadOutSlot> LoadoutDetails = m_Loadout.GetLoadoutDetails();
		foreach(ref LoadOutSlot slot : LoadoutDetails){
			ItemBase itemIB;
			if (Class.CastTo(itemIB,m_DemoPlayer.GetHumanInventory().CreateInInventory(slot.GetItemClassName())))
			{
				ref array<string> inventoryItems = slot.GetCargo();
				foreach(string inv : inventoryItems){
					itemIB.GetInventory().CreateInInventory(inv);
				}

				ref array<string> attachments = slot.GetAttachments();
				foreach(string att : attachments){
					itemIB.GetInventory().CreateAttachment(att);
				}
			}
		}
	}

	void OnCharacterSelect(int index)
	{
		m_PreviewPlayerClassName = m_CharactersDropDown.GetContentArray().Get(index);
		m_CharactersDropDown.SetSelectedItem( m_PreviewPlayerClassName );
		m_CharactersDropDown.Close();

		GetSpawnParams().SetSelectedCharacterSkin(m_PreviewPlayerClassName); //Update config

		//Update Preview dude guy man
		if (m_DemoPlayer){
			GetGame().ObjectDelete(m_DemoPlayer);
			g_Game.PreloadObject( m_PreviewPlayerClassName, 1000.0);
			m_DemoPlayer    = PlayerBase.Cast( GetGame().CreateObject( m_PreviewPlayerClassName, m_DemoPos, true, false ) );
			m_PlayerPreview.SetPlayer( m_DemoPlayer );
			m_PlayerPreview.SetModelPosition( "0 0 0.45" );
			m_PlayerPreview.SetSize( 0.80, 0.80 );

			//Add back the gear to the demo unit
			if (GetLocalConfigInstance().LoadoutSelectionType() == 1)
			{
				ref array<string> selectedItems = GetSpawnParams().GetCLOItems();
				foreach(string item : selectedItems){
					m_DemoPlayer.GetHumanInventory().CreateInInventory( item );
				}
			}else{
				OnSelectSLO(-1);
			}
		}
	}

	void UpdateCharactersList()
	{
		int selectedGender = m_GenderCombo.GetCurrentItem();
		if (selectedGender == 0){ //Male
			m_CharactersDropDown.UpdateListedItems(m_MaleChars);
		}else if (selectedGender == 1){
			m_CharactersDropDown.UpdateListedItems(m_FemaleChars);
		}
	}

	void SortCharacters()
	{
		string character_CfgName;
		ref array<string> m_AllCharacters = new array<string>;
		m_AllCharacters = GetGame().ListAvailableCharacters();
		m_FemaleChars 	= new array<string>;
		m_MaleChars 	= new array<string>;
		for (int i = 0; i < m_AllCharacters.Count(); i++){
			character_CfgName = m_AllCharacters.Get(i);
			if (GetGame().IsKindOf(character_CfgName, "SurvivorMale_Base")){
				m_MaleChars.Insert( character_CfgName );
			}else{
				m_FemaleChars.Insert( character_CfgName );
			}
		}
	}

	bool RotateDemoModel(){
		g_Game.GetMousePos( m_CharacterRotationX, m_CharacterRotationY );
		GetGame().GetDragQueue().Call( this, "UpdateDemoUnitRotation" );
		return true;
	}

	void UpdateDemoUnitRotation( int mouse_x, int mouse_y, bool is_dragging ){
		vector orientation = m_CharacterOrientation;
		orientation[1] = orientation[1] - ( m_CharacterRotationX - mouse_x );
		m_PlayerPreview.SetModelOrientation( orientation );

		if ( !is_dragging ){
			m_CharacterOrientation = orientation;
		}
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
	}

	bool OnClick( Widget w, int x, int y, int button ){
		if (w == m_ButtonHide){
			ShowTab(false); return true;
		}
		if (w == m_GenderCombo){
			if (m_GenderCombo.GetCurrentItem() == 0){
				m_GenderCombo.SetCurrentItem(1);
			}else{
				m_GenderCombo.SetCurrentItem(0);
			}
			UpdateCharactersList();
			return true;
		}
		if (w == m_ButtonRandomize){
			RandomizeCLO();
			return true;
		}
		if (w == m_BtnSelectRandSL){
			m_SLODropDown.Randomize();
			return true;
		}
		return false;
	}
};