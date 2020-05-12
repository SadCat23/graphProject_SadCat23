#pragma once
#include <DirectXMath.h>

struct Vertex_Texture
{
	Vertex_Texture() {}
	Vertex_Texture(float x, float y, float z, float tu, float tv,float nx, float ny, float nz)
		: pos(x, y, z), tex(tu, tv), normals(nx,ny,nz)  {}

	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 tex;
	DirectX::XMFLOAT3 normals;
};