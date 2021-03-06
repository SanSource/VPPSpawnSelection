class CoolDownManager extends PluginBase
{
	private float m_CooldownDefault = 1200;
	ref map<string, ref map<string,float>> m_CLMap = new map<string, ref map<string,float>>; // Map 1: GUID, CL Info || Map 2: Position Name, CL Length
	void CoolDownManager()
	{
		Print("CoolDownManager()");
		GetRPCManager().AddRPC( "RPC_GetCooldownStatus", "GetCooldownStatus", this, SingeplayerExecutionType.Server );
	}

	override void OnInit()
	{
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(this.UpdateCountDown, 1000, true);
	}

	void GetCooldownStatus( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target )
	{
		Param1<string> data;
        if ( !ctx.Read( data ) ) return;

        if (type == CallType.Server)
        {
        	ref map<string,float> CoolDownInfo;
        	string GUID = sender.GetId();
        	bool hasCooldown = false;
        	float  TimeDuration = 0;
        	
		    for (int x = 0; x < m_CLMap.Count(); ++x)
		    {
		    	string StoredGUID  = m_CLMap.GetKey(x);
		    	CoolDownInfo = m_CLMap.Get(StoredGUID);

			    for (int i = 0; i < CoolDownInfo.Count(); ++i)
			    {
			    	string CLName  = CoolDownInfo.GetKey(i);
			    	float    clTime  = CoolDownInfo.Get(CLName);

			    	if (clTime != 0 && CLName == data.param1 && GUID == StoredGUID)
			    	{
			    		hasCooldown = true;
			    		TimeDuration = clTime;
			    	}
			    	else if (clTime <= 0 && CLName == data.param1 && GUID == StoredGUID)
			    	{
			    		hasCooldown = false;
			    		TimeDuration = 0;
			    	}
			    }
		    }
		    Param2<float,bool> oParams = new Param2<float,bool>(TimeDuration,hasCooldown);
    		GetRPCManager().SendRPC( "RPC_CooldownInfo", "CooldownInfo", oParams, true, sender );
        }
	}

	void UpdateCountDown()
	{
		ref map<string,float> CoolDownInfo;
	    for (int x = 0; x < m_CLMap.Count(); ++x)
	    {
	    	string GUID  = m_CLMap.GetKey(x);
	    	CoolDownInfo = m_CLMap.Get(GUID);

    	    for (int i = 0; i < CoolDownInfo.Count(); ++i)
    	    {
    	    	string CLName   = CoolDownInfo.GetKey(i);
    	    	float  clTime   = CoolDownInfo.Get(CLName);
    	    	float  NewTime;
    	    	if (clTime != 0)
    	    	{
    	    		NewTime = clTime - 1;
    	    		CoolDownInfo.Set(CLName,NewTime);
    	    	}
    	    	else if (clTime <= 0)
    	    	{
    	    		CoolDownInfo.Remove(CLName);
	    		   	m_CLMap.Set(GUID,CoolDownInfo);
    	    		Print("Timer Ended..Removed from Map");

	    		    for (int xx = 0; xx < m_CLMap.Count(); ++xx)
	    		    {
	    		    	string id    = m_CLMap.GetKey(xx);
	    		    	CoolDownInfo = m_CLMap.Get(id);

    		    	    for (int ff = 0; ff < CoolDownInfo.Count(); ++ff)
    		    	    {
    		    	    	string cllocation = CoolDownInfo.GetKey(ff);
    		    	    	float  cltimex    = CoolDownInfo.Get(cllocation);
    		    	    	Print("ID: "+id + " Location: "+cllocation+ " Time"+cltimex);
    		    	    }
	    		    }  
    	    	}
    	    }
	    }
	}

	float GetCoolDownTimer(string LocationName)
	{
		return SSConfig.Cast(GetPluginManager().SLGetConfigByType(SSConfig)).GetSpawnPointByName(LocationName).GetCooldown();
	}

	void AddCooldown(string GUID, string Location)
	{
		ref map<string,float> CoolDown_Info;
		if (m_CLMap.Contains(GUID))
		{
			CoolDown_Info = m_CLMap.Get(GUID);
			if (CoolDown_Info)
			{
			    for (int i = 0; i < CoolDown_Info.Count(); ++i)
			    {
			    	string LocationName = CoolDown_Info.GetKey(i);
	   			    int time 			= CoolDown_Info.Get(LocationName);
	   			    if (LocationName == Location)
	   			    {
						Print("Error..Location already added!");
						return;
	   			    }
			    }
			    CoolDown_Info.Insert(Location,GetCoolDownTimer(Location));
	   			m_CLMap.Set(GUID,CoolDown_Info);
	   			Print("Added Coolodown for "+GUID+ " Info: "+Location+ " " +GetCoolDownTimer(Location));
			}
		}
		else
		{
			CoolDown_Info = new map<string,float>;
			CoolDown_Info.Insert(Location,GetCoolDownTimer(Location));

			m_CLMap.Insert(GUID,CoolDown_Info);
			Print("[Init]Added Coolodown for "+GUID+ " Info: "+Location+ " " +GetCoolDownTimer(Location));
		}
	}

	void RemoveCooldown(string GUID, string Location)
	{
		if (m_CLMap.Contains(GUID))
		{
			ref map<string,float> CoolDownInfo;
		    for (int i = 0; i < m_CLMap.Count(); ++i)
		    {
		    	string PID   = m_CLMap.GetKey(i);
		    	CoolDownInfo = m_CLMap.Get(PID);
		    	if (CoolDownInfo.Count() >= 1)
		    	{
		    		for (int x = 0; x < CoolDownInfo.Count(); ++x)
		    	    {
		    	    	string CLName   = CoolDownInfo.GetKey(x);
		    	    	float    clTime   = CoolDownInfo.Get(CLName);
		    	    	if (Location == CLName)
		    	    	{
		    	    		CoolDownInfo.Remove(CLName);
		    	    		m_CLMap.Set(GUID,CoolDownInfo); //updated map
		    	    	}
		    	    }
		    	}
		    	else
		    	{
		    		Print("CoolDownManager:: Error ( User Slot Contains No cooldown info ) Removing CL Params for: "+GUID);
		    	}
		    }
		}
		else
		{
			Print("CoolDownManager:: Error ( Unregistered user ) Removing CL Params for: "+GUID);
		}
	}
}
