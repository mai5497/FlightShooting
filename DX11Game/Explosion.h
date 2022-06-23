/*
	�����G�t�F�N�g�N���X
	2022/04/26
	�ɒn�c�^��
*/
#pragma once
#include "GameObj.h"
#include "AssimpModel.h"


//========================= �\���̒�` ===========================
struct VERTEX {	// ���_���W
	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT4 diffuse;
};


//========================= �N���X��` ===========================
class CDebris	// �j�ЃN���X
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
