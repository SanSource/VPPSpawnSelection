class DropDownInstance
{
	private Widget RootWidget;
	private string InstanceName;
	protected ref DropdownSpawnMenu m_DropdownSpawnMenu;

	void DropDownInstance(string name, Widget rootWidget, ref DropdownSpawnMenu dropdownClass)
	{
		RootWidget   		= rootWidget;
		InstanceName		= name;
		m_DropdownSpawnMenu = dropdownClass;

		m_DropdownSpawnMenu.m_OnSelectItem.Insert( OnSelectDropDownItem );
	}

	void OnSelectDropDownItem(int index)
	{
		m_DropdownSpawnMenu.SetSelectedItem( m_DropdownSpawnMenu.GetContentArray().Get(index) ); //use m_DropdownSpawnMenu.GetSelectedItem() to get selected item
		m_DropdownSpawnMenu.Close();
	}

	ref DropdownSpawnMenu GetDropDownInstance(){
		return m_DropdownSpawnMenu;
	}

	ref Widget GetWidget(){
		return RootWidget;
	}
};