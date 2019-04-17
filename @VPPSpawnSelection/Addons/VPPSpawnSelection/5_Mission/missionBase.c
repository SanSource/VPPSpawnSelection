modded class MissionBase
{
	override UIScriptedMenu CreateScriptedMenu(int id)
	{
		UIScriptedMenu menu = NULL;
        menu = super.CreateScriptedMenu(id);
        if (!menu)
        {
            switch (id)
            {
                case VPP_MENU_SPAWN_SCENE_NEW:
                menu = new VPPSpawnMenuNew;
                break;
            }
            if (menu)
            {
                menu.SetID(id);
            }
        }
        return menu;
	}
}