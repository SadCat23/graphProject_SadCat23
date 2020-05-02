#pragma once
#include "GameObject.h"
class KatamariaScene : public GameObject
{
	typeObject GetType() override
	{
		return actor;
	}


};

