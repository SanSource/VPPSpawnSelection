class SSConfig extends SpawnSelectorConfigs
{
	static const string SETTINGS_FILE = "$profile:\\SpawnSelectorConfig.json";
	
	private int  LOADOUT_SELECTION_TYPE; // 1 == CustomizableLoadouts  2 == StaticLoadouts
	private int  MAX_LOBBY_TIME;
	private int  LOGIN_TIME;
	private bool FORCE_RANDOM_SPAWNS;
	private ref array<string>     				   ESSENTIAL_ITEMS;
	private ref array<ref SpawnPointDetails> 	   SPAWN_POINTS;
	private ref array<ref StaticLoadout>     	   STATIC_LOADOUTS;
	private ref array<ref CustomizableLoadout>     CUSTOMIZABLE_LOADOUTS_ITEMS;

	void SSConfig(){
	
	}
	
	override void save(){
		JsonFileLoader<SSConfig>.JsonSaveFile(SETTINGS_FILE, this);
		load(); //Call load after save to assure matching class data
	}

	override void load()
	{
		if (FileExist(SETTINGS_FILE)) {
			JsonFileLoader<SSConfig>.JsonLoadFile(SETTINGS_FILE, this);
		}else{
			LOADOUT_SELECTION_TYPE = 1;
			MAX_LOBBY_TIME		   = 300; //Seconds
			LOGIN_TIME			   = 15;
			FORCE_RANDOM_SPAWNS	   = false;
			ESSENTIAL_ITEMS 	   = new array<string>;
			ESSENTIAL_ITEMS.Insert("apple");
			ESSENTIAL_ITEMS.Insert("rag");
			ESSENTIAL_ITEMS.Insert("rag");
			//-----
			SPAWN_POINTS    = new array<ref SpawnPointDetails>;
			SPAWN_POINTS.Insert(new SpawnPointDetails("SpawnPoint0", Vector(1598.47 ,0,2319.08), 1200.0));
			SPAWN_POINTS.Insert(new SpawnPointDetails("SpawnPoint1", Vector(1183.56 ,0,4387.4), 1200.0));
			SPAWN_POINTS.Insert(new SpawnPointDetails("SpawnPoint2", Vector(1614.98 ,0,5237.16), 1200.0));
			SPAWN_POINTS.Insert(new SpawnPointDetails("SpawnPoint3", Vector(1965.51 ,0,5226.71), 1200.0));
			SPAWN_POINTS.Insert(new SpawnPointDetails("SpawnPoint4", Vector(3101.78 ,0,4450.58), 1200.0));
			SPAWN_POINTS.Insert(new SpawnPointDetails("SpawnPoint5", Vector(3811.87 ,0,4967.03), 1200.0));
			SPAWN_POINTS.Insert(new SpawnPointDetails("SpawnPoint6", Vector(4793.88 ,0,4765.16), 1200.0));
			SPAWN_POINTS.Insert(new SpawnPointDetails("SpawnPoint7", Vector(2424.25 ,0,5810.53), 1200.0));
			SPAWN_POINTS.Insert(new SpawnPointDetails("SpawnPoint8", Vector(2424.25 ,0,5810.53), 1200.0));
			SPAWN_POINTS.Insert(new SpawnPointDetails("SpawnPoint9", Vector(2239.14 ,0,6537.76), 1200.0));
			SPAWN_POINTS.Insert(new SpawnPointDetails("SpawnPoint10", Vector(2709.21 ,0,6805.49), 1200.0));
			SPAWN_POINTS.Insert(new SpawnPointDetails("SpawnPoint11", Vector(3240.48 ,0,6675.31), 1200.0));
			SPAWN_POINTS.Insert(new SpawnPointDetails("SpawnPoint12", Vector(3559.28 ,0,6400.25), 1200.0));
			SPAWN_POINTS.Insert(new SpawnPointDetails("SpawnPoint13", Vector(4084.57 ,0,6218.31), 1200.0));
			SPAWN_POINTS.Insert(new SpawnPointDetails("SpawnPoint14", Vector(4411.33 ,0,6208.7), 1200.0));
			SPAWN_POINTS.Insert(new SpawnPointDetails("SpawnPoint15", Vector(4728.26 ,0,6283.69), 1200.0));
			SPAWN_POINTS.Insert(new SpawnPointDetails("SpawnPoint16", Vector(4712.08 ,0,5806.04), 1200.0));
			SPAWN_POINTS.Insert(new SpawnPointDetails("SpawnPoint17", Vector(4856.5 ,0,6278.57), 1200.0));
			SPAWN_POINTS.Insert(new SpawnPointDetails("SpawnPoint18", Vector(5947.13 ,0,6679.44), 1200.0));
			SPAWN_POINTS.Insert(new SpawnPointDetails("SpawnPoint19", Vector(3973.08 ,0,7975.11), 1200.0));
			SPAWN_POINTS.Insert(new SpawnPointDetails("SpawnPoint20", Vector(4137.5 ,0,8612.45), 1200.0));
			SPAWN_POINTS.Insert(new SpawnPointDetails("SpawnPoint21", Vector(3196.51 ,0,8906.59), 1200.0));
			SPAWN_POINTS.Insert(new SpawnPointDetails("SpawnPoint22", Vector(1969.12 ,0,9354.73), 1200.0));
			SPAWN_POINTS.Insert(new SpawnPointDetails("SpawnPoint23", Vector(651.716 ,0,9155.5), 1200.0));
			SPAWN_POINTS.Insert(new SpawnPointDetails("SpawnPoint24", Vector(2819.51 ,0,9707.45), 1200.0));
			SPAWN_POINTS.Insert(new SpawnPointDetails("SpawnPoint25", Vector(3483.69 ,0,9972.09), 1200.0));
			SPAWN_POINTS.Insert(new SpawnPointDetails("SpawnPoint26", Vector(2681.46 ,0,10454.5), 1200.0));
			SPAWN_POINTS.Insert(new SpawnPointDetails("SpawnPoint27", Vector(2227.24 ,0,10781.8), 1200.0));
			SPAWN_POINTS.Insert(new SpawnPointDetails("SpawnPoint28", Vector(3416.78 ,0,10811.1), 1200.0));
			SPAWN_POINTS.Insert(new SpawnPointDetails("SpawnPoint29", Vector(3416.78 ,0,10811.1), 1200.0));
			SPAWN_POINTS.Insert(new SpawnPointDetails("SpawnPoint30", Vector(4150.98 ,0,10895.3), 1200.0));
			SPAWN_POINTS.Insert(new SpawnPointDetails("SpawnPoint31", Vector(4491.77 ,0,10640.6), 1200.0));
			SPAWN_POINTS.Insert(new SpawnPointDetails("SpawnPoint32", Vector(4547.1 ,0,10293.8), 1200.0));
			SPAWN_POINTS.Insert(new SpawnPointDetails("SpawnPoint33", Vector(4846.42 ,0,9857.94), 1200.0));
			SPAWN_POINTS.Insert(new SpawnPointDetails("SpawnPoint34", Vector(5095.83 ,0,9538.65), 1200.0));
			SPAWN_POINTS.Insert(new SpawnPointDetails("SpawnPoint35", Vector(5303.58 ,0,9313.91), 1200.0));
			SPAWN_POINTS.Insert(new SpawnPointDetails("SpawnPoint36", Vector(5000.43 ,0,9170.28), 1200.0));
			SPAWN_POINTS.Insert(new SpawnPointDetails("SpawnPoint37", Vector(7147.12 ,0,9735.82), 1200.0));
			SPAWN_POINTS.Insert(new SpawnPointDetails("SpawnPoint38", Vector(7448.82 ,0,11635.9), 1200.0));
			//-----
			

			//-----
			STATIC_LOADOUTS = new array<ref StaticLoadout>;
			STATIC_LOADOUTS.Insert(
				new StaticLoadout(
					"Example 1",
					{
						new LoadOutSlot("Hands","M4A1", { "m4_suppressor", "m4_cqbbttstck", "acogoptic", "m4_plastichndgrd" }, {""}),
						new LoadOutSlot("Back","huntingbag", {""}, {"cannabis","cannabis","mag_stanagcoupled_30rnd","mag_stanagcoupled_30rnd"} ),
						new LoadOutSlot("Vest","ukassvest_black", {""}, {"rag"} )
					},
				)
			);

			STATIC_LOADOUTS.Insert(
				new StaticLoadout(
					"Example 2",
					{
						new LoadOutSlot("Hands","M4A1", { "m4_suppressor", "m4_cqbbttstck", "acogoptic", "mag_stanagcoupled_30rnd", "m4_plastichndgrd" }, {"mag_stanagcoupled_30rnd","mag_stanagcoupled_30rnd"}),
						new LoadOutSlot("Back","huntingbag", {""}, {"cannabis","cannabis"} )
					},
				)
			);
			//-----


			//-----
			CUSTOMIZABLE_LOADOUTS_ITEMS = new array<ref CustomizableLoadout>;
			CUSTOMIZABLE_LOADOUTS_ITEMS.Insert(new CustomizableLoadout("Feet",     {"CombatBoots_Black","Sneakers_Green","WorkingBoots_Beige","HikingBootsLow_Black"}));
			CUSTOMIZABLE_LOADOUTS_ITEMS.Insert(new CustomizableLoadout("Legs",     {"Jeans_Green","ParamedicPants_Green","CargoPants_Beige","Jeans_Black"}));
			CUSTOMIZABLE_LOADOUTS_ITEMS.Insert(new CustomizableLoadout("Body",     {"QuiltedJacket_Green","ParamedicJacket_Green","TShirt_Beige","RidersJacket_Black"}));
			CUSTOMIZABLE_LOADOUTS_ITEMS.Insert(new CustomizableLoadout("Vest",     {"pressvest_lightblue","platecarriercomplete","ukassvest_camo","ukassvest_olive"}));
			CUSTOMIZABLE_LOADOUTS_ITEMS.Insert(new CustomizableLoadout("Headgear", {"ballistichelmet_green","ballistichelmet_black","ballistichelmet_un","baseballcap_cmmg_black"}));
			CUSTOMIZABLE_LOADOUTS_ITEMS.Insert(new CustomizableLoadout("Gloves",   {"tacticalgloves_black","tacticalgloves_beige","tacticalgloves_green","workinggloves_black"}));
			CUSTOMIZABLE_LOADOUTS_ITEMS.Insert(new CustomizableLoadout("Back",     {"tortillabag","taloonbag_violet","huntingbag","leathersack_natural"}));
			CUSTOMIZABLE_LOADOUTS_ITEMS.Insert(new CustomizableLoadout("Eyewear",  {"sportglasses","aviatorglasses","designerglasses","thickframesglasses"}));
			//-----
			save();
		}
	}

	ref array<string> GetEssentialItems(){
		return ESSENTIAL_ITEMS;
	}

	vector GetRandomSpawnPoint(){
		return SPAWN_POINTS.Get(Math.RandomIntInclusive(0, SPAWN_POINTS.Count() - 1)).GetPosition();
	}

	ref SpawnPointDetails GetSpawnPointByPos(vector pos){
		foreach(ref SpawnPointDetails sp : SPAWN_POINTS){
			if (sp.GetPosition() == pos){
				return sp;
			}
		}
		return NULL;
	}

	ref SpawnPointDetails GetSpawnPointByName(string name){
		foreach(ref SpawnPointDetails sp : SPAWN_POINTS){
			if (sp.GetName() == name){
				return sp;
			}
		}
		return NULL;
	}

	ref array<string> GetAllSpawnPointsNames(){
		ref array<string> result = new array<string>;
		foreach(ref SpawnPointDetails sp : SPAWN_POINTS){
			result.Insert(sp.GetName());
		}
		return result;
	}

	ref array<ref SpawnPointDetails> GetSpawnPoints(){
		return SPAWN_POINTS;
	}

	ref array<ref StaticLoadout> GetStaticLoadouts(){
		return STATIC_LOADOUTS;
	}

	ref array<string> GetAllStaticLoadoutsNames(){
		ref array<string> result = new array<string>;
		foreach(ref StaticLoadout ld : STATIC_LOADOUTS){
			result.Insert(ld.GetLoadoutName());
		}
		return result;
	}

	ref StaticLoadout GetStaticLoadoutByName(string ldName){
		foreach(ref StaticLoadout loadout : STATIC_LOADOUTS){
			if (loadout.GetLoadoutName() == ldName){
				return loadout;
			}
		}
		return NULL;
	}

	ref array<ref CustomizableLoadout> GetCustomizableLoadouts(){
		return CUSTOMIZABLE_LOADOUTS_ITEMS;
	}

	ref array<string> GetCustomizableItemsNamesBySlot(string slotname){
		foreach(ref CustomizableLoadout ld : CUSTOMIZABLE_LOADOUTS_ITEMS){
			if (ld.GetSlotName() == slotname){
				return ld.GetItemClassNames();
			}
		}
		return NULL;
	}

	ref array<string> GetCustomizableItemsDisplayNamesBySlot(string slotname){
		foreach(ref CustomizableLoadout ld : CUSTOMIZABLE_LOADOUTS_ITEMS){
			if (ld.GetSlotName() == slotname){
				ref array<string> names = ld.GetItemClassNames();
				ref array<string> displayNames = new ref array<string>;
				foreach (string name : names){
					displayNames.Insert(g_Game.GetDisplayName(name));
				}
				return displayNames;
			}
		}
		return NULL;
	}

	int LoadoutSelectionType(){
		return LOADOUT_SELECTION_TYPE;
	}

	bool ForcedRandomSpawns(){
		return FORCE_RANDOM_SPAWNS;
	}

	int GetMaxLobbyTime(){
		return MAX_LOBBY_TIME;
	}

	int GetLoginTime(){
		return LOGIN_TIME;
	}
};

/*
All Human Slots
	Head
	Shoulder
	Melee
	Bow
	Headgear
	Mask
	Eyewear
	Hands
	LeftHand
	Gloves
	Armband
	Vest
	Body
	Back
	Hips
	Legs
	Feet
*/