class VPPSSTextItemPicker extends ScriptedWidgetEventHandler {
	private Widget m_Root;
	private VPPSpawnMenuNew m_Menu;
	private ref array<string> m_TextItems = new ref array<string>;
	private ref array<string> m_TextIds = new ref array<string>;
	private Widget m_ArrowLeft;
	private Widget m_ArrowRight;
	private TextWidget m_TextItem;
	private int m_CurrentIndex = 0;
	private string m_SlotType = "";
	private int m_ColorGreyedOut = ARGB(255,100,100,100);
	private int m_ColorWhite = ARGB(255,255,255,255);

	void VPPSSTextItemPicker(VPPSpawnMenuNew menu, Widget root, ref array<string> textItems, ref array<string> textIds = NULL, string slotType = "") {
        m_Root = GetGame().GetWorkspace().CreateWidgets( "VPPSpawnSelection/GUI/Layouts/TextItemPicker.layout", root );
		m_Menu = menu;
		m_TextItems = textItems;
		if (textIds == NULL){
			textIds = textItems;
		}
		m_TextIds = textIds;
		Print("m_TextItems count: " + m_TextItems.Count());

		m_ArrowLeft = m_Root.FindAnyWidget("image_left");
		m_ArrowRight = m_Root.FindAnyWidget("image_right");
		m_TextItem = TextWidget.Cast(m_Root.FindAnyWidget("text_item"));
		
		m_SlotType = slotType;
		
		UpdateView();

        m_Root.SetHandler(this);
	}

	override bool OnMouseButtonDown( Widget w, int x, int y, int button ) {
        if (button == MouseState.LEFT) {
            if (w == m_ArrowLeft && m_CurrentIndex > 0) {
				m_CurrentIndex--;
				UpdateView();
                return true;
            } else if (w == m_ArrowRight && m_CurrentIndex < m_TextItems.Count() - 1) {
				m_CurrentIndex++;
				UpdateView();
                return true;
            }
        }
        return false;
    }
	
	string GetSelectedTextItem() {
		return m_TextItems.Get(m_CurrentIndex);
	}
	
	void UpdateView() {
		if (m_CurrentIndex == 0){
			m_ArrowLeft.SetColor(m_ColorGreyedOut);
		} else {
			m_ArrowLeft.SetColor(m_ColorWhite);			
		}
		if (m_CurrentIndex == m_TextItems.Count() - 1){
			m_ArrowRight.SetColor(m_ColorGreyedOut);		
		} else {
			m_ArrowRight.SetColor(m_ColorWhite);					
		}
		Print("m_CurrentIndex: " + m_CurrentIndex);
		Print("setting text: " + m_TextItems.Get(m_CurrentIndex));
		m_TextItem.SetText(m_TextItems.Get(m_CurrentIndex));
		if (m_SlotType != "") {
			m_Menu.UpdateDemoItems(m_SlotType, m_TextIds.Get(m_CurrentIndex));
		} else {
			m_Menu.OnStaticLoadoutSelect(m_TextIds.Get(m_CurrentIndex));
		}		
	}
}