modded class PluginManager
{
	private ref array<ref SpawnSelectorConfigs> m_SLModConfigs = new array<ref SpawnSelectorConfigs>;

	void PluginManager()
	{
		
	}

	void Init()
	{
		super.Init();
		//----------------------------------------------------------------------
		// 						   Register modules
		//----------------------------------------------------------------------
		//		        Module Class Name 				Client	Server
		//----------------------------------------------------------------------
		RegisterPlugin( "SpawnSelector",			    false, 	true );
		RegisterPlugin( "CoolDownManager",			    false, 	true );
	}

	override void RegisterPlugin(string plugin_class_name, bool reg_on_client, bool reg_on_server, bool reg_on_release = true)
	{
		super.RegisterPlugin(plugin_class_name,reg_on_client, reg_on_server, reg_on_release);
		if ( GetGame().IsMultiplayer() )
		{
			if ( GetGame().IsServer() )
			{
				switch(plugin_class_name)
				{
					case "SpawnSelector":
					m_SLModConfigs.Insert(new SSConfig);
					break;
				}
			}
		}
	}

	override void PluginsInit()
	{
		super.PluginsInit();
	    LoadSLConfigs();
	}

	ref SpawnSelectorConfigs SLGetConfigByType(typename configType)
	{
		if ( GetGame().IsMultiplayer() && GetGame().IsServer() )
		{
			for (int i = 0; i < m_SLModConfigs.Count(); ++i)
			{
				ref SpawnSelectorConfigs config = m_SLModConfigs.Get(i);
				if (config.getType() == configType){
					return m_SLModConfigs.Get(i);
				}
			}
		}
		return NULL;
	}

    void LoadSLConfigs()
    {
    	foreach(SpawnSelectorConfigs modconfigs : m_SLModConfigs){
    		modconfigs.load();
    	}
    }
};
