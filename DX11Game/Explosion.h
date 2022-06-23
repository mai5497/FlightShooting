/*
	爆発エフェクトクラス
	2022/04/26
	伊地田真衣
*/
#pragma once
#include "GameObj.h"
#include "AssimpModel.h"


//========================= 構造体定義 ===========================
struct VERTEX {	// 頂点座標
	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT4 diffuse;
};


//========================= クラス定義 ===========================
class CDebris	// 破片クラス
{
private:
	XMFLOAT3 m_vertex[3];
	XMFLOAT3 m_normal;
	XMFLOAT4X4 m_world;
	XMFLOAT3 m_velocity;
	XMFLOAT3 m_accel;
	XMFLOAT3 m_axis;
	float m_rotate;
	float m_alpha;

public:
	HRESULT Init(XMFLOAT3& v0, XMFLOAT3& v1, XMFLOAT3& v2,
		float fRadius, XMFLOAT4X4* pWorld, XMFLOAT3* pVelocity);
	void Update();
	void UpdateBuffer(VERTEX* pVertex);
};


class CExplosion : public CGameObj
{
private:
	CDebris* m_pDebris;
	UINT m_uDebris;
	ID3D11Buffer* m_pVertexBuffer;
	static ID3D11Buffer* m_pConstantBuffer[2];
	static ID3D11VertexShader* m_pVertexShader;
	static ID3D11InputLayout* m_pInputLayout;
	static ID3D11PixelShader* m_pPixelShader;
	TAssimpVertex* m_pVertex;
	UINT m_uVertex;
	UINT* m_pIndex;
	UINT m_uIndex;

public:
	CExplosion(CScene* pScene);
	~CExplosion();
	static HRESULT InitShader();
	static void FinShader();
	HRESULT Init();
	void Fin();
	void Update();
	void Draw();
	void DrawAlpha();

	HRESULT SetModel(CAssimpModel* pModel);

	HRESULT Start(XMFLOAT4X4* pWorld, XMFLOAT3* pVelocity = nullptr);
};
