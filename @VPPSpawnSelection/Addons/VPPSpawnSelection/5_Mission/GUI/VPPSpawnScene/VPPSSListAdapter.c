class VPPSSListAdapter extends ScriptedWidgetEventHandler {

    Widget m_ListContent;
    ScrollWidget m_ScrollWidget;
    Widget m_PanelListFrame;

    ref array<ref VPPSSMapItem> m_Items = new array<ref VPPSSMapItem>>; // array of all data item objects

    ref map<ref Widget, ref VPPSSMapItem> m_WidgetItemMap = new map<ref Widget, ref VPPSSMapItem>; // map that stores the item's root widget together with its data item object

    void init(string layout, Widget root) {
        m_PanelListFrame = root;
        m_ScrollWidget = ScrollWidget.Cast(GetGame().GetWorkspace().CreateWidgets( layout, root ));
        m_ListContent = m_ScrollWidget.FindAnyWidget( "list_content" );

        m_ScrollWidget.SetHandler(this);

        UpdateView();
    }

    void UpdateContent(array<ref VPPSSMapItem> items) {
        m_Items = items;
        UpdateView();
    }

    void UpdateView() {
        // Remove all existing widgets
        Widget child = m_ListContent.GetChildren();
        while( child ) {
            Widget lastChild = child;
            child = child.GetSibling();
            m_ListContent.RemoveChild(lastChild);
        }

        // Clear the widget-item map
        m_WidgetItemMap.Clear();

        // Add all new widgets
        AddAllWidgets(m_Items);

        m_ListContent.Update();
        m_ScrollWidget.Update();

        // Show/hide scroll bar
        if(ShouldShowScrollBar()) {
            m_ScrollWidget.SetAlpha( 1 );
        } else {
            m_ScrollWidget.SetAlpha( 0 );
        }
    }

    void AddAllWidgets(array<ref VPPSSMapItem> items);

    VPPSSItemViewHolder CreateViewHolder(Widget root, VPPSSMapItem item);

    void FillViewHolder(VPPSSItemViewHolder viewHolder, VPPSSMapItem item);

    bool ShouldShowScrollBar();

    void OnItemClicked(string widgetName, VPPSSMapItem item);

    override bool OnMouseButtonDown( Widget w, int x, int y, int button ) {
        if (button == MouseState.LEFT) {
            VPPSSMapItem item = m_WidgetItemMap.Get(w.GetParent());
            if (item) {
                OnItemClicked(w.GetName(), item);
                UpdateView();
                return true;
            }
        }
        return false;
    }
}

class VPPSSItemViewHolder extends Managed {
    ref Widget m_RootWidget;

    void init(Widget root) {
        m_RootWidget = root;
    }

    void fillView(VPPSSMapItem item);
}

class VPPSSMapItem extends Managed {
}