/*
	This class will hold a specific slots' items and its name: eg Hands slot with the parent item ( main item ) and its attachments + cargo

*/
class LoadOutSlot
{
	private string SLOT_NAME;
	private string ITEM_CLASS_NAME;
	private ref array<string> ITEM_ATTACHMENTS;
	private ref array<string> ITEM_CARGO;
	
	void LoadOutSlot(string slotName, string itemClassName, ref array<string> atts, ref array<string> cargo){
		SLOT_NAME 		 = slotName;
		ITEM_CLASS_NAME  = itemClassName;
		ITEM_ATTACHMENTS = atts;
		ITEM_CARGO		 = cargo;
	}
	
	string GetSlotName(){
		return SLOT_NAME;
	}
	
	string GetItemClassName(){
		return ITEM_CLASS_NAME;
	}
	
	ref array<string> GetAttachments(){
		return ITEM_ATTACHMENTS;
	}
	
	ref array<string> GetCargo(){
		return ITEM_CARGO;
	}
};

/*
	This class holds a FULL loadout, all human player slots with each of their items and cargo + attachments.
*/
class StaticLoadout
{	
	private string LO_NAME;
	private ref array<ref LoadOutSlot> LO_ITEMS;

	void StaticLoadout(string name, ref array<ref LoadOutSlot> items)
	{
		LO_NAME     = name;
		LO_ITEMS 	= items;
	}

	string GetLoadoutName(){
		return LO_NAME;
	}

	ref array<ref LoadOutSlot> GetLoadoutDetails(){
		return LO_ITEMS;
	}
};

/*
	This class is to store array of items with slot name for customizable loadouts *Use for config only*
*/
class CustomizableLoadout
{
	private string SLOT_NAME;
	private ref array<string> ITEM_CLASS_NAMES;
	
	void CustomizableLoadout(string slot, ref array<string> classNames){
		SLOT_NAME 		 = slot;
		ITEM_CLASS_NAMES = classNames;
	}
	
	string GetSlotName(){
		return SLOT_NAME;
	}
	
	ref array<string> GetItemClassNames(){
		return ITEM_CLASS_NAMES;
	}
};