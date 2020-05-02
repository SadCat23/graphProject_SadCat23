#include "GameObject.h"
#include <iostream>



GameObject::GameObject()
{
	this->oldRotate = XMMatrixIdentity();
}


bool GameObject::Initialize(const std::string & filePath, ID3D11Device * device, ID3D11DeviceContext * deviceContext, ID3D11ShaderResourceView * texture, ConstantBuffer<CB_VS_vertexshader>& cb_vs_vertexshader)
{
	if (!model.Initialize(filePath, device, deviceContext, texture, cb_vs_vertexshader))
		return false;

	this->centerBound = this->model.centerbound;
	this->radiusBound = this->model.radiusBound;
	this->SetPosition(0.0f, 0.0f, 0.0f);
	this->SetRotation(0.0f, 0.0f, 0.0f);
	this->UpdateWorldMatrix();
	return true;
}

void GameObject::Draw(const XMMATRIX & viewProjectionMatrix)
{
	model.Draw(this->worldMatrix, viewProjectionMatrix);
}




void GameObject::UpdateWorldMatrix()
{


	if (this->parent != nullptr)
	{

		static float oldX = parent->GetRotationFloat3().x;
		static float oldZ = parent->GetRotationFloat3().z;


		float deltaX = parent->GetRotationFloat3().x - oldX;
		float deltaZ = parent->GetRotationFloat3().z - oldZ;

		oldX = parent->GetRotationFloat3().x;
		oldZ = parent->GetRotationFloat3().z;


		oldRotate *= XMMatrixRotationX(deltaX);
		oldRotate *= XMMatrixRotationZ(-deltaZ);
		
		XMMATRIX scaleMatrix = XMMatrixScaling(this->scale, this->scale, this->scale);
		this->worldMatrix = scaleMatrix;
		this->worldMatrix *= XMMatrixRotationRollPitchYaw(this->rot.x, this->rot.y, this->rot.z);
		this->worldMatrix *= XMMatrixTranslation(this->pos.x, this->pos.y, this->pos.z);
		//this->worldMatrix *= XMMatrixRotationRollPitchYaw(this->GetRotationRelativeFloat3().x, this->GetRotationRelativeFloat3().y, this->GetRotationRelativeFloat3().z);
		//this->worldMatrix *= XMMatrixRotationRollPitchYaw(parent->GetRotationFloat3().x, parent->GetRotationFloat3().y, parent->GetRotationFloat3().z);
		this->worldMatrix *= oldRotate;
		this->worldMatrix *= XMMatrixTranslation(parent->GetPositionFloat3().x, parent->GetPositionFloat3().y, parent->GetPositionFloat3().z);
		
		if (parent->parent != nullptr)
		{
			this->worldMatrix *= XMMatrixTranslation(parent->parent->GetPositionFloat3().x, parent->parent->GetPositionFloat3().y, parent->parent->GetPositionFloat3().z);
			this->worldMatrix *= XMMatrixRotationRollPitchYaw(parent->parent->GetRotationRelativeFloat3().x, parent->parent->GetRotationRelativeFloat3().y, parent->parent->GetRotationRelativeFloat3().z);
		}

	}
	else
	{
		
	
		this->worldMatrix = XMMatrixScaling(this->scale, this->scale, this->scale);
		
		//this->worldMatrix *= XMMatrixRotationRollPitchYaw(0, 0, 0);
		
		static float oldX = this->rot.x;
		static float oldZ = this->rot.z;


		float deltaX = this->rot.x - oldX;
		float deltaZ = this->rot.z - oldZ;

		oldX = this->rot.x;
		oldZ = this->rot.z;


		oldRotate *= XMMatrixRotationX(deltaX);
		oldRotate *= XMMatrixRotationZ(-deltaZ);


		this->worldMatrix *= oldRotate;
	
		this->worldMatrix *= XMMatrixTranslation(this->pos.x, this->pos.y, this->pos.z);
		//this->worldMatrix *= XMMatrixRotationRollPitchYaw(this->GetRotationRelativeFloat3().x, this->GetRotationRelativeFloat3().y, this->GetRotationRelativeFloat3().z);

	}
	CalcCenterBound();
	UpdateDirectionVectors();

}

const XMVECTOR & GameObject::GetPositionVector() const
{
	return this->posVector;
}

const XMFLOAT3 & GameObject::GetPositionFloat3() const
{
	return this->pos;
}

const XMVECTOR & GameObject::GetRotationVector() const
{
	return this->rotVector;
}

const XMFLOAT3 & GameObject::GetRotationRelativeFloat3() const
{
	return this->rotRelative;
	// TODO: вставьте здесь оператор return
}

const XMFLOAT3 & GameObject::GetRotationFloat3() const
{
	return this->rot;
}

void GameObject::SetPosition(const XMVECTOR & pos)
{
	
	
	XMStoreFloat3(&this->pos, pos);
	this->posVector = pos;

	this->UpdateWorldMatrix();
}

void GameObject::SetPosition(const XMFLOAT3 & pos)
{
	this->pos = pos;
	this->posVector = XMLoadFloat3(&this->pos);

	this->UpdateWorldMatrix();
}

void GameObject::SetPosition(float x, float y, float z)
{
	this->pos = XMFLOAT3(x, y, z);
	this->posVector = XMLoadFloat3(&this->pos);

	this->UpdateWorldMatrix();
}

void GameObject::AdjustPosition(const XMVECTOR & pos)
{
	this->posVector += pos;
	XMStoreFloat3(&this->pos, this->posVector);

	this->UpdateWorldMatrix();
}






void GameObject::AdjustPosition(const XMFLOAT3 & pos)
{
	this->pos.x += pos.y;
	this->pos.y += pos.y;
	this->pos.z += pos.z;
	this->posVector = XMLoadFloat3(&this->pos);

	this->UpdateWorldMatrix();
}

void GameObject::AdjustPosition(float x, float y, float z)
{
	this->pos.x += x;
	this->pos.y += y;
	this->pos.z += z;
	this->posVector = XMLoadFloat3(&this->pos);

	this->UpdateWorldMatrix();
}

void GameObject::SetRotation(const XMVECTOR & rot)
{
	this->rotVector = rot;
	XMStoreFloat3(&this->rot, rot);
	this->UpdateWorldMatrix();
}

void GameObject::SetRotation(const XMFLOAT3 & rot)
{
	this->rot = rot;
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateWorldMatrix();
}

void GameObject::SetRotation(float x, float y, float z)
{
	this->rot = XMFLOAT3(x, y, z);
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateWorldMatrix();
}

void GameObject::AdjustRotation(const XMVECTOR & rot)
{
	this->rotVector += rot;
	XMStoreFloat3(&this->rot, this->rotVector);
	this->UpdateWorldMatrix();
}


void GameObject::AdjustRotation(const XMFLOAT3 & rot)
{
	this->rot.x += rot.x;
	this->rot.y += rot.y;
	this->rot.z += rot.z;
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateWorldMatrix();
}

void GameObject::AdjustRotation(float x, float y, float z)
{
	this->rot.x += x;
	this->rot.y += y;
	this->rot.z += z;
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateWorldMatrix();
}




void GameObject::AdjustRelativeRotation(const XMVECTOR & rotRelative)
{
	this->rotRelativeVector += rotRelative;
	XMStoreFloat3(&this->rotRelative, this->rotRelativeVector);
	this->UpdateWorldMatrix();
}

void GameObject::AdjustRelativeRotation(float x, float y, float z)
{
	this->rotRelative.x += x;
	this->rotRelative.y += y;
	this->rotRelative.z += z;
	this->rotRelativeVector = XMLoadFloat3(&this->rotRelative);
	this->UpdateWorldMatrix();
}

void GameObject::SetColor(const XMVECTOR & color)
{

	XMStoreFloat3(&this->color, color);
}

void GameObject::SetColor(const XMFLOAT3 & color)
{
	this->color = color;
}

void GameObject::SetColor(float r, float g, float b)
{
	this->color.x = r;
	this->color.y = g;
	this->color.z = b;
}

void GameObject::SetScale(float scaleNew)
{
	this->scale = scaleNew;
	this->UpdateWorldMatrix();
}

void GameObject::SetParent(GameObject* parent)
{
	this->parent = parent;
}


void GameObject::SetLookAtPos(XMFLOAT3 lookAtPos)
{

	//Verify that look at pos is not the same as cam pos. They cannot be the same as that wouldn't make sense and would result in undefined behavior.
	if (lookAtPos.x == this->pos.x && lookAtPos.y == this->pos.y && lookAtPos.z == this->pos.z)
		return;

	lookAtPos.x = this->pos.x - lookAtPos.x;
	lookAtPos.y = this->pos.y - lookAtPos.y;
	lookAtPos.z = this->pos.z - lookAtPos.z;

	float pitch = 0.0f;
	if (lookAtPos.y != 0.0f)
	{
		const float distance = sqrt(lookAtPos.x * lookAtPos.x + lookAtPos.z * lookAtPos.z);
		pitch = atan(lookAtPos.y / distance);
	}

	float yaw = 0.0f;
	if (lookAtPos.x != 0.0f)
	{
		yaw = atan(lookAtPos.x / lookAtPos.z);
	}
	if (lookAtPos.z > 0)
		yaw += XM_PI;

	this->SetRotation(pitch, yaw, 0.0f);
}

const XMVECTOR & GameObject::GetForwardVector(bool omitY )
{
	if (omitY)
		return this->vec_forward_noY;
	else
	return this->vec_forward;
}

const XMVECTOR & GameObject::GetRightVector(bool omitY)
{
	if (omitY)
		return this->vec_right_noY;
	else
		return this->vec_right;
}

const XMVECTOR & GameObject::GetBackwardVector(bool omitY)
{
	if (omitY)
		return this->vec_backward_noY;
	else
		return this->vec_backward;
}

const XMVECTOR & GameObject::GetLeftVector(bool omitY)
{
	if (omitY)
		return this->vec_left_noY;
	else
		return this->vec_left;
}

void GameObject::CalcCenterBound()
{
	FXMVECTOR vector = XMLoadFloat3(&this->centerBound);
	XMVECTOR NewCenter=XMVector3Transform(vector, worldMatrix);
	XMStoreFloat3(&this->centerBound, NewCenter);
}

void GameObject::UpdateDirectionVectors() 
{
	XMMATRIX vecRotationMatrix = XMMatrixRotationRollPitchYaw(this->rot.x, this->rot.y, 0.0f);
	this->vec_forward = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
	this->vec_backward = XMVector3TransformCoord(this->DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
	this->vec_left = XMVector3TransformCoord(this->DEFAULT_LEFT_VECTOR, vecRotationMatrix);
	this->vec_right = XMVector3TransformCoord(this->DEFAULT_RIGHT_VECTOR, vecRotationMatrix);

	XMMATRIX vecRotationMatrixNoY = XMMatrixRotationRollPitchYaw(0.0f, this->rot.y, 0.0f);
	this->vec_forward_noY = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, vecRotationMatrixNoY);
	this->vec_backward_noY = XMVector3TransformCoord(this->DEFAULT_BACKWARD_VECTOR, vecRotationMatrixNoY);
	this->vec_left_noY = XMVector3TransformCoord(this->DEFAULT_LEFT_VECTOR, vecRotationMatrixNoY);
	this->vec_right_noY = XMVector3TransformCoord(this->DEFAULT_RIGHT_VECTOR, vecRotationMatrixNoY);
}