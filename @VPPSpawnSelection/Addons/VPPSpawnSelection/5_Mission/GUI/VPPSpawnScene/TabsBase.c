class TabsBase
{
	ref array<ref TabsBase> m_AllTabs = new array<ref TabsBase>;
	ref Widget m_FocusedWidget;
	void TabsBase()
	{
		Print("Tab Created: "+this.Type());
	}

	void ~TabsBase()
	{
		Print("Tab Destroyed: "+this.Type());
	}

	void OnUpdate(float timeslice)
	{
		
	}

	void InsertTab(ref TabsBase tab){
		m_AllTabs.Insert(tab);
	}

	bool IsOpen()
	{
		return false;
	}

	void ShowTab(bool b){

	}

	string GetTabName(){
		return "";
	}

	bool IsTabOpen(string tabName){
		foreach(ref TabsBase t : m_AllTabs){
			if (t.GetTabName() == tabName){
				return t.IsOpen();
			}
		}
		return false;
	}

	void ShowTabByName(string tabName, bool show){
		foreach(ref TabsBase t : m_AllTabs){
			if (t.GetTabName() == tabName){
				t.ShowTab(show);
			}
		}
	}

	void HideAllTabs(){
		foreach(ref TabsBase t : m_AllTabs){
			t.ShowTab(false);
		}
	}
};