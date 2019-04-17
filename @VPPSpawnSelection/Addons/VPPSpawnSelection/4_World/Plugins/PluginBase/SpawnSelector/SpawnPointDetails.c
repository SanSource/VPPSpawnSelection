class SpawnPointDetails
{	
	private string SP_NAME;
	private vector SP_POSITION;
	private float  SP_COOLDOWN;

	void SpawnPointDetails(string name, vector pos, float cl)
	{
		SP_NAME     = name;
		SP_POSITION = pos;
		SP_COOLDOWN = cl;
	}

	void SetName(string name){
		SP_NAME = name;
	}

	void SetPosition(vector pos){
		SP_POSITION = pos;
	}

	void SetCooldown(float cl){
		SP_COOLDOWN = cl;
	}

	string GetName(){
		return SP_NAME;
	}

	vector GetPosition(){
		return SP_POSITION;
	}

	float GetCooldown(){
		return SP_COOLDOWN;
	}
};