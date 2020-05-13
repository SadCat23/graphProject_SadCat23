#include "KatamariaGame.h"
#include "Engine.h"
#include <iostream>
void KatamariaGame::Init()
{
	this->sphere = (SphereKatamaria*)MyUrealEngine->AddGameObject(XMFLOAT3(0.0f, 0.0f, 0.0f), nullptr, "Data\\Objects\\ball\\Ball.obj", this->MyUrealEngine->GetGraphicsEngine()->StarDeath);
	this->sphere->SetScale(1);
	this->sphere->radiusBound = 1;
	this->sphere->size = 1;
	




}

void KatamariaGame::SpawnObject()
{
	ObjectKatamaria* newObjects;
	newObjects = (ObjectKatamaria*)MyUrealEngine->AddGameObject(XMFLOAT3(5.0f, 0.0f, 0.0f), nullptr, "Data\\Objects\\ball\\Ball.obj",this->MyUrealEngine->GetGraphicsEngine()->Ogon);
	newObjects->justChild = false;
	newObjects->SetScale(0.5);
	newObjects->radiusBound = 1*newObjects->scale;
	this->objects.push_back(newObjects);
	

	newObjects = (ObjectKatamaria*)MyUrealEngine->AddGameObject(XMFLOAT3(0.0f, 0.0f, 5.0f), nullptr, "Data\\Objects\\ball\\Ball.obj", this->MyUrealEngine->GetGraphicsEngine()->Ogon);
	newObjects->justChild = false;
	newObjects->SetScale(0.5);
	newObjects->radiusBound = 1 * newObjects->scale;
	this->objects.push_back(newObjects);
	
	
	Light* light1;
	light1 = (Light*)MyUrealEngine->AddLight(XMFLOAT3(2.0f, 2.0f, 0.0f), nullptr);
	light1->SetScale(0.1);
	MyUrealEngine->GetGraphicsEngine()->light.push_back(light1);


	Light* light2;
	light2 = (Light*)MyUrealEngine->AddLight(XMFLOAT3(5.0f, 2.0f, 0.0f), nullptr);
	light2->SetScale(0.1);
	light2->lightColor = XMFLOAT3(0.0f, 1.0f, 1.0f);
	MyUrealEngine->GetGraphicsEngine()->light.push_back(light2);

	/*newObjects = (ObjectKatamaria*)MyUrealEngine->AddGameObject(XMFLOAT3(5.0f, 0.0f, 13.0f), nullptr, "Data\\Objects\\ball\\fighter.obj", this->MyUrealEngine->GetGraphicsEngine()->StarDeath);
	newObjects->justChild = false;
	newObjects->SetScale(0.01);
	this->objects.push_back(newObjects);*/
	
	
	/*newObjects = (ObjectKatamaria*)MyUrealEngine->AddGameObject(XMFLOAT3(0.0f, -1.0f, 0.0f), nullptr, "Data\\Objects\\plane\\plane.obj", this->MyUrealEngine->GetGraphicsEngine()->Grass);
	newObjects->justChild = false;
	newObjects->SetScale(1);
	this->objects.push_back(newObjects);*/
}

void KatamariaGame::GameLoop()
{
	for (int i = 0; i < objects.size(); i++)
	{
		if (!this->objects[i]->justChild)
		{
			if (CheckCollison(objects[i]))
			{
				AddToChild(objects[i]);
			}
		}
	}
	
}

void KatamariaGame::AddToChild(ObjectKatamaria* obj)
{
	obj->SetParent(this->sphere);
	XMFLOAT3 vector;
	vector.x = obj->GetPositionFloat3().x - this->sphere->GetPositionFloat3().x;
	vector.y = obj->GetPositionFloat3().y-this->sphere->GetPositionFloat3().y;
	vector.z = obj->GetPositionFloat3().z-this->sphere->GetPositionFloat3().z;
	
	
	
	XMVECTOR a = XMLoadFloat3(&vector);
	XMVECTOR Length = XMVector3Length(a);
	XMFLOAT3 vectorLength;
	XMStoreFloat3(&vectorLength, Length);
	float InverseLength = 1/ vectorLength.x;
	
	vector.x *= InverseLength * (this->sphere->radiusBound + obj->radiusBound);
	vector.y *= InverseLength * (this->sphere->radiusBound + obj->radiusBound);
	vector.z *= InverseLength * (this->sphere->radiusBound + obj->radiusBound);
	
	obj->oldX = obj->parent->GetRotationFloat3().x;
	obj->oldZ = obj->parent->GetRotationFloat3().z;

	
	obj->SetPosition(vector.x, vector.y, vector.z);

	this->sphere->childs.push_back(obj);
	obj->justChild = true;
}

bool KatamariaGame::CheckCollison(ObjectKatamaria* obj)
{
	XMFLOAT3 vector;

	

	
	vector.x = this->sphere->GetPositionFloat3().x- obj->GetPositionFloat3().x;
	vector.y = this->sphere->GetPositionFloat3().y - obj->GetPositionFloat3().y;
	vector.z = this->sphere->GetPositionFloat3().z - obj->GetPositionFloat3().z;

	float length = sqrtf(powf(vector.x, 2) + powf(vector.y, 2) + powf(vector.z, 2));

	float RadiusSum = this->sphere->radiusBound + obj->radiusBound;

	std::cout << length << "\n";
	std::cout << "________________________\n";
	std::cout << RadiusSum << "\n";


	
	return length<RadiusSum;



}

void KatamariaGame::Input(char Key)
{
	if (Key == 'W')
	{
		this->sphere->AdjustPosition(0.0f, 0.0f, 0.001f*MyUrealEngine->deltaTime);
		this->sphere->AdjustRotation(0.001f*MyUrealEngine->deltaTime, 0.0f, 0.0f);
		
		this->MyUrealEngine->GetGraphicsEngine()->myCamera.SetPosition(sphere->GetPositionFloat3().x + 0, sphere->GetPositionFloat3().y + 5.0f, sphere->GetPositionFloat3().z - 4.0f);
		this->sphere->UpdateWorldMatrix();
		if (!this->sphere->childs.empty())
		{
			for (int i = 0; i < this->sphere->childs.size(); i++)
			{
			this->sphere->childs[i]->UpdateWorldMatrix();
			}
		}
		
	}
	if (Key == 'S')
	{
		this->sphere->AdjustPosition(0.0f, 0.0f, -0.001f*MyUrealEngine->deltaTime);
		this->sphere->AdjustRotation(-0.001f*MyUrealEngine->deltaTime, 0.0f, 0.0f);
		this->MyUrealEngine->GetGraphicsEngine()->myCamera.SetPosition(sphere->GetPositionFloat3().x + 0, sphere->GetPositionFloat3().y + 5.0f, sphere->GetPositionFloat3().z - 4.0f);
		this->sphere->UpdateWorldMatrix();
		for (int i = 0; i < this->sphere->childs.size(); i++)
		{
			this->sphere->childs[i]->UpdateWorldMatrix();
		}
	}
	if (Key == 'A')
	{
		
		this->sphere->AdjustPosition(-0.001f*MyUrealEngine->deltaTime, 0.0f,0.0f );
		this->sphere->AdjustRotation(0.0f, 0.0f, -0.001f*MyUrealEngine->deltaTime);
		
		this->MyUrealEngine->GetGraphicsEngine()->myCamera.SetPosition(sphere->GetPositionFloat3().x + 0, sphere->GetPositionFloat3().y + 5.0f, sphere->GetPositionFloat3().z - 4.0f);
		this->sphere->UpdateWorldMatrix();
		for (int i = 0; i < this->sphere->childs.size(); i++)
		{
			this->sphere->childs[i]->UpdateWorldMatrix();
		}
	}
	if (Key == 'D')
	{
		this->sphere->AdjustPosition(0.001f*MyUrealEngine->deltaTime, 0.0f, 0.0f);
		this->sphere->AdjustRotation(0.0f, 0.0f, 0.001f*MyUrealEngine->deltaTime);
		this->MyUrealEngine->GetGraphicsEngine()->myCamera.SetPosition(sphere->GetPositionFloat3().x + 0, sphere->GetPositionFloat3().y + 5.0f, sphere->GetPositionFloat3().z - 4.0f);
		this->sphere->UpdateWorldMatrix();
		for (int i = 0; i < this->sphere->childs.size(); i++)
		{
			this->sphere->childs[i]->UpdateWorldMatrix();
		}
	}


}