#pragma once
#include "GameObject.h"
#include "ObjectKatamaria.h"
#include <vector>
class SphereKatamaria: public GameObject
{
public:
	
	float size;
	typeObject GetType() override
	{
		return actor;
	}

};