class ModelListAdapter extends VPPSSListAdapter {

	private VPPSpawnMenuNew m_Menu;
    private string m_SelectedCharId;
	private ref array<string> m_CharIds;

    void ModelListAdapter(VPPSpawnMenuNew menu, Widget root, ref array<string> charIds) {
        super.init("VPPSpawnSelection/GUI/Layouts/List/HorizontalList.layout", root);
		
		m_Menu = menu;
		m_CharIds = charIds;
    }

    void SetSelectedCharId(string icon) {
        m_SelectedCharId = icon;
        UpdateContent();
    }

    string GetSelectedCharId() {
        return m_SelectedCharId;
    }

    void UpdateContent() {
        array<ref VPPSSMapItem> items = new array<ref VPPSSMapItem>>;
        foreach (string charId : m_CharIds) {
            VPPSSModelItem item = new VPPSSModelItem();
            item.m_CharId = charId;
            if (!m_SelectedCharId || m_SelectedCharId == item.m_CharId) {
                item.m_IsSelected = true;
                m_SelectedCharId = item.m_CharId;
            }
            items.Insert(item);
        }
        super.UpdateContent(items);
    }

    override void AddAllWidgets(array<ref VPPSSMapItem> items) {
        foreach (VPPSSMapItem item: items) {
            VPPSSItemViewHolder viewHolder = CreateViewHolder(m_ListContent, item);
            FillViewHolder(viewHolder, item);
            m_WidgetItemMap.Insert(viewHolder.m_RootWidget, item);
        }
    }

    override VPPSSItemViewHolder CreateViewHolder(Widget root, VPPSSMapItem item) {
        if (item.IsInherited(VPPSSModelItem)) {
            return VPPSSModelItemViewHolder(root);
        }
        return null;
    }

    override void FillViewHolder(VPPSSItemViewHolder viewHolder, VPPSSMapItem item) {
        if (item.IsInherited(VPPSSModelItem) && viewHolder.IsInherited(VPPSSModelItemViewHolder)) {
            VPPSSModelItemViewHolder vh = VPPSSModelItemViewHolder.Cast(viewHolder);
            VPPSSModelItem modelItem = VPPSSModelItem.Cast(item);
            vh.fillView(modelItem);
        }
    }

    override bool ShouldShowScrollBar() {
        float x, y;
        m_PanelListFrame.GetScreenSize( x, y );
        return m_ScrollWidget.GetContentWidth() > x;
    }

    override void OnItemClicked(string widgetName, VPPSSMapItem item) {

        if (widgetName == "model_image" || widgetName == "selected_image") {

            if (item.IsInherited(VPPSSModelItem)) {
                VPPSSModelItem modelItem = VPPSSModelItem.Cast(item);
                m_SelectedCharId = modelItem.m_CharId;
				m_Menu.OnCharacterSelect(m_SelectedCharId);
                UpdateContent();
            }
        }
    }
}

class VPPSSModelItemViewHolder extends VPPSSItemViewHolder {
    ImageWidget mModelImage;
    ImageWidget mSelectedImage;

    void VPPSSModelItemViewHolder(Widget root) {
        super.init(GetGame().GetWorkspace().CreateWidgets("VPPSpawnSelection/GUI/Layouts/List/ModelListItem.layout", root));

        mModelImage = ImageWidget.Cast( m_RootWidget.FindAnyWidget( "model_image" ) );
        mSelectedImage = ImageWidget.Cast( m_RootWidget.FindAnyWidget( "selected_image" ) );
    }

    void fillView(VPPSSModelItem item) {
        mModelImage.LoadImageFile(0, "set:vppss_models image:" + item.m_CharId);
        mSelectedImage.Show(item.m_IsSelected);
    }
}

class VPPSSModelItem extends VPPSSMapItem {
    bool m_IsSelected = false;
    string m_CharId;
}