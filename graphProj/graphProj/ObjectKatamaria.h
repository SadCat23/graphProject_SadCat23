#pragma once
#include "GameObject.h"
class ObjectKatamaria : public GameObject
{
public:
	float radius;
	bool justChild=false;
	float size;
	ObjectKatamaria() 
	{
		justChild = false;
	};
	typeObject GetType() override
	{
		return actor;
	}
};
