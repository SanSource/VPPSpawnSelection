class VPPSpawnMenuNew extends UIScriptedMenu
{
	//----------------Scene-Cam-----------------
	private Camera 			m_Camera;
	private vector 			m_CameraTrans[4];
	private vector 			m_Target;
	private vector 			m_LookAtPos;
	//-----------------------------------------
	private vector 			m_DemoUnitPos;
	private PlayerBase 		m_DemoPlayer;
	private PlayerPreviewWidget m_PlayerPreview;
	private vector m_CharacterOrientation;
	private int m_CharacterRotationX, m_CharacterRotationY;
	private ref array<string> m_FemaleCharIds = new ref array<string>;
	private ref array<string> m_MaleCharIds = new ref array<string>;
	private ref ModelListAdapter m_ModelAdapterFemale;
	private ref ModelListAdapter m_ModelAdapterMale;
	private ButtonWidget m_ButtonFemale;
	private ButtonWidget m_ButtonMale;
	private Widget m_PanelFemale;
	private Widget m_PanelMale;
	private Widget m_GridPickerStatic;
	private ref VPPSSTextItemPicker m_StaticLoadoutPicker;
	private Widget m_GridPickerCustom;
	private ref VPPSSTextItemPicker m_CustomHeadgearPicker, m_CustomEyewearPicker, m_CustomBodyPicker, m_CustomVestPicker, m_CustomBackPicker, m_CustomGlovesPicker, m_CustomLegsPicker, m_CustomFeetPicker;
	private ref array<int> m_InventoryEnums = new array<int>;
	private MapWidget m_MapWidget;
	private TextWidget m_SelectedSpawn;
	private TextWidget m_Cooldown;

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
		
		if (m_DemoPlayer)
		    GetGame().ObjectDelete(m_DemoPlayer);
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
        layoutRoot = GetGame().GetWorkspace().CreateWidgets( "VPPSpawnSelection/GUI/Layouts/Main.layout" );

		LoadCharacterList();
        m_ButtonFemale = ButtonWidget.Cast(layoutRoot.FindAnyWidget( "button_female" ));
        m_ButtonMale = ButtonWidget.Cast(layoutRoot.FindAnyWidget( "button_male" ));
        m_PanelMale = layoutRoot.FindAnyWidget( "panel_models_male" );
		m_PanelMale.Show(true);
        m_ModelAdapterMale = new ModelListAdapter(this, m_PanelMale, m_MaleCharIds);
		m_ModelAdapterMale.SetSelectedCharId("SurvivorM_Mirek");
        m_PanelFemale = layoutRoot.FindAnyWidget( "panel_models_female" );
		m_PanelFemale.Show(false);
        m_ModelAdapterFemale = new ModelListAdapter(this, m_PanelFemale, m_FemaleCharIds);
		m_ModelAdapterFemale.SetSelectedCharId("SurvivorF_Eva");

		g_Game.PreloadObject( "SurvivorM_Mirek", 1000.0);
		m_DemoPlayer = PlayerBase.Cast( GetGame().CreateObject( "SurvivorM_Mirek", m_DemoUnitPos, true, false ) );
		m_PlayerPreview = PlayerPreviewWidget.Cast( layoutRoot.FindAnyWidget( "playerpreview" ) );
		m_PlayerPreview.SetPlayer( m_DemoPlayer );
		m_PlayerPreview.SetModelPosition( "0 0 0" );
		m_PlayerPreview.SetSize( 0.40, 0.40 );
		
		if (GetLocalConfigInstance().LoadoutSelectionType() == 1){
        	//CustomizableLoadouts
			m_GridPickerCustom = layoutRoot.FindAnyWidget("grid_pickers_custom");
			m_GridPickerCustom.Show(true);
			
			m_CustomHeadgearPicker = new VPPSSTextItemPicker(this, m_GridPickerCustom.FindAnyWidget("panel_picker_headgear"),
									 GetLocalConfigInstance().GetCustomizableItemsDisplayNamesBySlot("Headgear"), GetLocalConfigInstance().GetCustomizableItemsNamesBySlot("Headgear"), "Headgear");
			m_CustomEyewearPicker = new VPPSSTextItemPicker(this, m_GridPickerCustom.FindAnyWidget("panel_picker_eyewear"),
									GetLocalConfigInstance().GetCustomizableItemsDisplayNamesBySlot("Eyewear"), GetLocalConfigInstance().GetCustomizableItemsNamesBySlot("Eyewear"), "Eyewear");
			m_CustomBodyPicker = new VPPSSTextItemPicker(this, m_GridPickerCustom.FindAnyWidget("panel_picker_body"),
								 GetLocalConfigInstance().GetCustomizableItemsDisplayNamesBySlot("Body"), GetLocalConfigInstance().GetCustomizableItemsNamesBySlot("Body"), "Body");
			m_CustomVestPicker = new VPPSSTextItemPicker(this, m_GridPickerCustom.FindAnyWidget("panel_picker_vest"),
								 GetLocalConfigInstance().GetCustomizableItemsDisplayNamesBySlot("Vest"), GetLocalConfigInstance().GetCustomizableItemsNamesBySlot("Vest"), "Vest");
			m_CustomBackPicker = new VPPSSTextItemPicker(this, m_GridPickerCustom.FindAnyWidget("panel_picker_back"),
								 GetLocalConfigInstance().GetCustomizableItemsDisplayNamesBySlot("Back"), GetLocalConfigInstance().GetCustomizableItemsNamesBySlot("Back"), "Back");
			m_CustomGlovesPicker = new VPPSSTextItemPicker(this, m_GridPickerCustom.FindAnyWidget("panel_picker_gloves"),
								   GetLocalConfigInstance().GetCustomizableItemsDisplayNamesBySlot("Gloves"), GetLocalConfigInstance().GetCustomizableItemsNamesBySlot("Gloves"), "Gloves");
			m_CustomLegsPicker = new VPPSSTextItemPicker(this, m_GridPickerCustom.FindAnyWidget("panel_picker_legs"),
								 GetLocalConfigInstance().GetCustomizableItemsDisplayNamesBySlot("Legs"), GetLocalConfigInstance().GetCustomizableItemsNamesBySlot("Legs"), "Legs");
			m_CustomFeetPicker = new VPPSSTextItemPicker(this, m_GridPickerCustom.FindAnyWidget("panel_picker_feet"),
								 GetLocalConfigInstance().GetCustomizableItemsDisplayNamesBySlot("Feet"), GetLocalConfigInstance().GetCustomizableItemsNamesBySlot("Feet"), "Feet");
        }else{
        	//StaticLoadouts
			m_GridPickerStatic = layoutRoot.FindAnyWidget("grid_pickers_static");
			m_GridPickerStatic.Show(true);
        	Widget panelPickerLoadouts = layoutRoot.FindAnyWidget("panel_picker_loadouts");
			m_StaticLoadoutPicker = new VPPSSTextItemPicker(this, panelPickerLoadouts, GetLocalConfigInstance().GetAllStaticLoadoutsNames());
        }
		
		WidgetEventHandler.GetInstance().RegisterOnMouseButtonDown( m_PlayerPreview,  this, "RotateDemoModel" );
		
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
		
		m_SelectedSpawn = layoutRoot.FindAnyWidget("text_selectedspawn");
		m_Cooldown = layoutRoot.FindAnyWidget("text_cooldown");
		
		m_MapWidget = layoutRoot.FindAnyWidget("map_widget");
        WidgetEventHandler.GetInstance().RegisterOnDoubleClick( m_MapWidget, this, "OnMapDoubleClick" );
		DisplaySpawnPoints();
		
		return layoutRoot;
	}

	void LoadCharacterList()
	{
		ref array<string> availableChars = GetGame().ListAvailableCharacters();
		foreach (string char : availableChars) {
			if (GetGame().IsKindOf(char, "SurvivorMale_Base")){
				m_MaleCharIds.Insert(char);
			} else {
				m_FemaleCharIds.Insert(char);
			}
		}
	}
	
	void DisplaySpawnPoints(){
		m_MapWidget.ClearUserMarks();
		ref array<ref SpawnPointDetails> SpawnPoints = GetLocalConfigInstance().GetSpawnPoints();
		string spawnReadyIcon = "VPPSpawnSelection\\GUI\\Textures\\spawnpoint_ready.paa";
		string spawnNotReadyIcon = "VPPSpawnSelection\\GUI\\Textures\\spawnpoint_notready.paa";
		string spawnSelectedIcon = "VPPSpawnSelection\\GUI\\Textures\\spawnpoint_selected.paa";
		vector selectedSpawnPos = GetSpawnParams().GetSelectedSpawnPoint();
		foreach(ref SpawnPointDetails sp: SpawnPoints){
			string icon;
			if (sp.GetCooldown() > 0){
				if (sp.GetPosition() == selectedSpawnPos){
					icon = spawnSelectedIcon;
				} else {
					icon = spawnReadyIcon;
				}
			} else {
				icon = spawnNotReadyIcon;				
			}
			m_MapWidget.AddUserMark(sp.GetPosition(), "", ARGB(255,255,255,255), icon);
		}
	}

    //Returns world coords of where player clicks on map
    vector GetMapClickPos() {
        vector world_pos,ScreenToMap,dir,from,to;

        dir = GetGame().GetPointerDirection();
        from = GetGame().GetCurrentCameraPosition();
        to = from + ( dir * 1000 );

        world_pos = GetGame().GetScreenPos( to );
        ScreenToMap = m_MapWidget.ScreenToMap( world_pos );
        return ScreenToMap;
    }
	
    void OnMapDoubleClick(Widget w, int x, int y, int button) {
		vector clickPos = GetMapClickPos();
        float minDistance = 150, distance;
        ref SpawnPointDetails nearestSpawnPoint;
        bool isCustomServerMarker = false;
		ref array<ref SpawnPointDetails> SpawnPoints = GetLocalConfigInstance().GetSpawnPoints();
		foreach(ref SpawnPointDetails sp: SpawnPoints){
            distance = vector.Distance(clickPos, sp.GetPosition());
            if (distance < minDistance) {
                minDistance = distance;
                nearestSpawnPoint = sp;
            }
        }
        if (nearestSpawnPoint) {
			string spawnPointName = nearestSpawnPoint.GetName();
			//Get Cooldown info
			GetRPCManager().SendRPC( "RPC_GetCooldownStatus", "GetCooldownStatus", new Param1<string>(spawnPointName), true, NULL);
			m_SelectedSpawn.SetText(spawnPointName);
			m_Cooldown.SetText(nearestSpawnPoint.GetCooldown().ToString() + "s");
			
			//update config
			GetSpawnParams().SetSelectedSpawnPoint(nearestSpawnPoint.GetPosition());
			DisplaySpawnPoints();
        }
	}

	void CooldownInfo( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target )
	{
		// TODO
		/*Param2<float,bool> data;
        if ( !ctx.Read( data ) ) return;

        if (type == CallType.Client){
        	m_PointTimeLeft  = data.param1;
			m_PointStatus    = data.param2;
			SetLockIcon(m_PointStatus);
			m_InputCLCount.Show(m_PointStatus);
			m_InputCLCount.Update();
        }*/
	}

	override void Update(float timeslice)
	{
		super.Update(timeslice);
	}
	
	override bool OnClick(Widget w, int x, int y, int button) {
		if (w == m_ButtonFemale){
			m_PanelMale.Show(false);			
			m_PanelFemale.Show(true);
			OnCharacterSelect(m_ModelAdapterFemale.GetSelectedCharId());
			return true;
		} else if (w == m_ButtonMale){
			m_PanelMale.Show(true);			
			m_PanelFemale.Show(false);
			OnCharacterSelect(m_ModelAdapterMale.GetSelectedCharId());
			return true;
		}
		return false;
	}
	
	void OnStaticLoadoutSelect(string loadout) {
		//Clear Inventory first
		ClearDemoUnitInventory();
		ref StaticLoadout m_Loadout = GetLocalConfigInstance().GetStaticLoadoutByName(loadout);
		//Spawn the items for the demo
		ref array<ref LoadOutSlot> LoadoutDetails = m_Loadout.GetLoadoutDetails();
		foreach(ref LoadOutSlot slot : LoadoutDetails){
			ItemBase itemIB;
			if (Class.CastTo(itemIB, m_DemoPlayer.GetHumanInventory().CreateInInventory(slot.GetItemClassName())))
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
	
	void UpdateDemoItems(string slotType, string itemId) {
		ItemBase ItemInSlot;
		if ( Class.CastTo(ItemInSlot,m_DemoPlayer.FindAttachmentBySlotName(slotType)) ){
			GetGame().ObjectDelete(ItemInSlot);
		}

		if (itemId != "") m_DemoPlayer.GetHumanInventory().CreateInInventory(itemId);

		//update config items
		ref array<string> SelectedItems = new array<string>;
		SelectedItems.Insert(m_CustomHeadgearPicker.GetSelectedTextItem());
		SelectedItems.Insert(m_CustomEyewearPicker.GetSelectedTextItem());
		SelectedItems.Insert(m_CustomBodyPicker.GetSelectedTextItem());
		SelectedItems.Insert(m_CustomVestPicker.GetSelectedTextItem());
		SelectedItems.Insert(m_CustomBackPicker.GetSelectedTextItem());
		SelectedItems.Insert(m_CustomGlovesPicker.GetSelectedTextItem());
		SelectedItems.Insert(m_CustomLegsPicker.GetSelectedTextItem());
		SelectedItems.Insert(m_CustomFeetPicker.GetSelectedTextItem());
		GetSpawnParams().SetCLOItems(SelectedItems);
	}

	void ClearDemoUnitInventory() {
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

	void OnCharacterSelect(string charId) {
		GetSpawnParams().SetSelectedCharacterSkin(charId); //Update config

		if (m_DemoPlayer){
			GetGame().ObjectDelete(m_DemoPlayer);
			g_Game.PreloadObject( charId, 1000.0);
			m_DemoPlayer    = PlayerBase.Cast( GetGame().CreateObject( charId, m_DemoUnitPos, true, false ) );
			m_PlayerPreview.SetPlayer( m_DemoPlayer );
			m_PlayerPreview.SetModelPosition( "0 0 0" );
			m_PlayerPreview.SetSize( 0.40, 0.40 );

			//Add back the gear to the demo unit
			Print("GetLocalConfigInstance: " + GetLocalConfigInstance());
			if (GetLocalConfigInstance().LoadoutSelectionType() == 1) {
				ref array<string> selectedItems = GetSpawnParams().GetCLOItems();
				foreach(string item : selectedItems){
					m_DemoPlayer.GetHumanInventory().CreateInInventory( item );
				}
			} else {
				OnStaticLoadoutSelect(m_StaticLoadoutPicker.GetSelectedTextItem());
			}
		}
	}

    static int VectorToARGB(vector color) {
        float R,G,B;
        R = color[0];
        G = color[1];
        B = color[2];
        return ARGB(255,R,G,B);
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
};