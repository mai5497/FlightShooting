// ユーザ定義メッシュ [Mesh.h]
#pragma once
#include "main.h"

class CMeshMaterial {
public:
	XMFLOAT4	m_Diffuse;		// Diffuse color RGBA
	XMFLOAT4	m_Ambient;		// Ambient color RGB
	XMFLOAT4	m_Specular;		// Specular 'shininess'
	XMFLOAT4	m_Emissive;		// Emissive color RGB
	float		m_Power;		// Sharpness if specular highlight
};

class CMesh
{
private:
	ID3D11Buffer* m_pVertexBuffer;	// 頂点バッファインターフェースへのポインタ
	ID3D11Buffer* m_pIndexBuffer;	// インデックスバッファインターフェースへのポインタ

	int m_nNumVertex;				// 総頂点数	
	int m_nNumIndex;				// 総インデックス数

	XMFLOAT4X4 m_mWorld;

	CMeshMaterial m_material;		// マテリアル

	static ID3D11Buffer* m_pConstantBuffer[2];	// 定数バッファ
	static ID3D11SamplerState* m_pSamplerState;	// テクスチャ サンプラ
	static ID3D11VertexShader* m_pVertexShader;	// 頂点シェーダ
	static ID3D11InputLayout* m_pInputLayout;	// 頂点フォーマット
	static ID3D11PixelShader* m_pPixelShader;	// ピクセルシェーダ

public:
	CMesh();
	virtual ~CMesh();

	static HRESULT InitShader();
	static void FinShader();

	HRESULT Init(const VERTEX_3D vertexWk[], int nVertex, const int indexWk[], int nIndex);
	virtual void Fin();
	virtual void Draw();

	void SetWorld(XMFLOAT4X4* pWorld) { m_mWorld = *pWorld; }
	void SetMaterial(const CMeshMaterial* pMaterial);
	void GetMaterial(CMeshMaterial* pMaterial) { *pMaterial = m_material; }
	void SetDiffuse(XMFLOAT4 vDiffuse) { m_material.m_Diffuse = vDiffuse; }
};
