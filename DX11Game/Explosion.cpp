/*
	爆発エフェクトクラス
	2022/04/26
	伊地田真衣
*/
#include "Explosion.h"
#include "Shader.h"
#include "Scene.h"



struct SHADER_GLOBAL {	// シェーダーに渡す値
	XMMATRIX mWVP;	// ワールド*ビュー*射影行列（転置行列）
	XMMATRIX mW;	//ワールド行列
};

struct SHADER_GLOBAL2 {
	XMVECTOR vEye;	// 視点座標

	XMVECTOR vLightDir;	// 光源方向
	XMVECTOR vLa;	// 光源色（アンビエント）
	XMVECTOR vLd;	// 光源色（ディフューズ）
	XMVECTOR vLs;	// 光源色（スぺキュラ）

	XMVECTOR vAmbient;		// アンビエント色
	XMVECTOR vDiffuse;		// ディフューズ色
	XMVECTOR vSpecular;		// スペキュラ色（＋スペキュラ強度）
	XMVECTOR vEmissive;	// エミッシブ色
};

// 静的メンバ変数
ID3D11Buffer* CExplosion::m_pConstantBuffer[2] = { nullptr, nullptr };
ID3D11VertexShader* CExplosion::m_pVertexShader = nullptr;
ID3D11InputLayout* CExplosion::m_pInputLayout = nullptr;
ID3D11PixelShader* CExplosion::m_pPixelShader = nullptr;

//========================================================
//
// 破片
//
//========================================================

HRESULT CDebris::Init(XMFLOAT3& v0, XMFLOAT3& v1, XMFLOAT3& v2,
	float fRadius, XMFLOAT4X4* pWorld, XMFLOAT3* pVelocity)
{

	XMMATRIX world = XMLoadFloat4x4(pWorld);
	XMFLOAT3 v[3];
	XMStoreFloat3(&v[0], XMVector3TransformCoord(XMLoadFloat3(&v0), world));
	XMStoreFloat3(&v[1], XMVector3TransformCoord(XMLoadFloat3(&v1), world));
	XMStoreFloat3(&v[2], XMVector3TransformCoord(XMLoadFloat3(&v2), world));


	XMFLOAT3 c = {
		(v[0].x + v[1].x + v[2].x) / 3.0f,
		(v[0].y + v[1].y + v[2].y) / 3.0f,
		(v[0].z + v[1].z + v[2].z) / 3.0f
	};


	for (int i = 0; i < 3; ++i) {
		v[i].x -= c.x;
		v[i].y -= c.y;
		v[i].z -= c.z;
	}

	for (int i = 0; i < 3; ++i) {
		m_vertex[i] = v[i];
	}


	XMVECTOR edge =
		XMVectorSet(v[1].x - v[0].x, v[1].y - v[0].y, v[1].z - v[0].z, 0.0f);
	XMFLOAT3 e;
	XMStoreFloat3(&e, edge);
	if (e.x == 0.0f && e.y == 0.0f && e.z == 0.0f) {
		return E_FAIL;
	}
	XMVECTOR n = XMVector3Normalize(XMVector3Cross(edge,
		XMVectorSet(v[2].x - v[1].x, v[2].y - v[1].y, v[2].z - v[1].z, 0.0f)));
	XMStoreFloat3(&m_normal, n);
	if (m_normal.x == 0.0f && m_normal.y == 0.0f && m_normal.z == 0.0f) {
		return E_FAIL;
	}


	m_alpha = 1.0f;


	XMMATRIX rotate =
		XMMatrixRotationAxis(n, XMConvertToRadians(float(rand() % 360)));
	XMStoreFloat3(&m_axis,
		XMVector3TransformNormal(XMVector3Normalize(edge), rotate));


	m_rotate = (120 + rand() % 240) / 60.0f;


	m_velocity.x = m_normal.x * fRadius * 0.02f;
	m_velocity.y = m_normal.y * fRadius * 0.02f;
	m_velocity.z = m_normal.z * fRadius * 0.02f;
	if (pVelocity) {
		m_velocity.x += pVelocity->x;
		m_velocity.y += pVelocity->y;
		m_velocity.z += pVelocity->z;
	}


	m_accel = XMFLOAT3(0.0f, fRadius * -0.0005f, 0.0f);


	XMStoreFloat4x4(&m_world, XMMatrixTranslation(c.x, c.y, c.z));

	return S_OK;
}


void CDebris::Update()
{

	m_alpha -= (1.0f / 180.0f);
	if (m_alpha < 1.0f / 255.0f) {
		m_alpha = 0.0f;
		return;
	}


	m_velocity.x += m_accel.x;
	m_velocity.y += m_accel.y;
	m_velocity.z += m_accel.z;


	XMFLOAT3 pos = { m_world._41, m_world._42, m_world._43 };
	m_world._41 = m_world._42 = m_world._43 = 0.0f;


	pos.x += m_velocity.x;
	pos.y += m_velocity.y;
	pos.z += m_velocity.z;


	XMStoreFloat4x4(&m_world, XMMatrixMultiply(XMLoadFloat4x4(&m_world),
		XMMatrixRotationAxis(XMLoadFloat3(&m_axis),
			XMConvertToRadians(m_rotate))));


	m_world._41 = pos.x;
	m_world._42 = pos.y;
	m_world._43 = pos.z;
}


void CDebris::UpdateBuffer(VERTEX* pVertex)
{
	XMMATRIX world = XMLoadFloat4x4(&m_world);
	XMFLOAT3 normal;
	XMStoreFloat3(&normal,
		XMVector3TransformNormal(
			XMLoadFloat3(&m_normal), world));
	XMFLOAT4 diffuse = XMFLOAT4(1.0f, 0.0f, 0.0f, m_alpha);
	for (int i = 0; i < 3; ++i, ++pVertex) {
		XMStoreFloat3(&pVertex->position,
			XMVector3TransformCoord(XMLoadFloat3(&m_vertex[i]), world));
		pVertex->normal = normal;
		pVertex->diffuse = diffuse;
	}
}





CExplosion::CExplosion(CScene* pScene) : CGameObj(pScene)
{
	m_id = GOT_EXPLOSION;

	m_pDebris = nullptr;
	m_uDebris = 0;
	m_pVertex = nullptr;
	m_uVertex = 0;
	m_pIndex = nullptr;
	m_uIndex = 0;
	m_pVertexBuffer = nullptr;
}


CExplosion::~CExplosion()
{
}


HRESULT CExplosion::InitShader()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	static const D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
		D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
		D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	hr = LoadShader("ExpVertex", "ExpPixel",
		&m_pVertexShader, &m_pInputLayout, &m_pPixelShader,
		layout, _countof(layout));
	if (FAILED(hr)) {
		return hr;
	}


	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SHADER_GLOBAL);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = pDevice->CreateBuffer(&bd, nullptr, &m_pConstantBuffer[0]);
	if (FAILED(hr)) return hr;
	bd.ByteWidth = sizeof(SHADER_GLOBAL2);
	hr = pDevice->CreateBuffer(&bd, nullptr, &m_pConstantBuffer[1]);

	return hr;
}


void CExplosion::FinShader()
{

	for (int i = 0; i < _countof(m_pConstantBuffer); ++i) {
		SAFE_RELEASE(m_pConstantBuffer[i]);
	}


	SAFE_RELEASE(m_pPixelShader);


	SAFE_RELEASE(m_pInputLayout);


	SAFE_RELEASE(m_pVertexShader);
}


HRESULT CExplosion::Init()
{

	return S_OK;
}


void CExplosion::Fin()
{

	SAFE_RELEASE(m_pVertexBuffer);

	SAFE_DELETE_ARRAY(m_pDebris);

	SAFE_DELETE_ARRAY(m_pIndex);
	SAFE_DELETE_ARRAY(m_pVertex);
}


void CExplosion::Update()
{

	if (!m_pDebris) return;
	CDebris* pDeb = m_pDebris;
	for (UINT i = 0; i < m_uDebris; ++i, ++pDeb) {
		pDeb->Update();
	}

	if (!m_pVertexBuffer) return;
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	D3D11_MAPPED_SUBRESOURCE msr;
	HRESULT hr = pDeviceContext->Map(m_pVertexBuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &msr);
	if (SUCCEEDED(hr)) {

		pDeb = m_pDebris;
		VERTEX* pVertex = (VERTEX*)msr.pData;
		for (UINT i = 0; i < m_uDebris; ++i, ++pDeb) {
			pDeb->UpdateBuffer(pVertex);
			pVertex += 3;
		}

		pDeviceContext->Unmap(m_pVertexBuffer, 0);
	}
}


void CExplosion::Draw()
{

}
void CExplosion::DrawAlpha()
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();


	pDeviceContext->VSSetShader(m_pVertexShader, nullptr, 0);
	pDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);
	pDeviceContext->IASetInputLayout(m_pInputLayout);


	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);


	SHADER_GLOBAL cb;
	XMMATRIX mtxWorld = XMMatrixIdentity();
	CCamera* pCamera = CCamera::Get();
	XMFLOAT4X4& mtxView = pCamera->GetViewMatrix();
	XMFLOAT4X4& mtxProj = pCamera->GetProjMatrix();
	cb.mWVP = XMMatrixTranspose(mtxWorld *
		XMLoadFloat4x4(&mtxView) * XMLoadFloat4x4(&mtxProj));
	cb.mW = XMMatrixTranspose(mtxWorld);
	pDeviceContext->UpdateSubresource(m_pConstantBuffer[0], 0, nullptr, &cb, 0, 0);
	pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer[0]);


	SHADER_GLOBAL2 cb2;
	cb2.vEye = XMLoadFloat3(&pCamera->GetPos());
	CLight* pLight = CLight::Get();
	cb2.vLightDir = XMLoadFloat3(&pLight->GetDir());
	cb2.vLa = XMLoadFloat4(&pLight->GetAmbient());
	cb2.vLd = XMLoadFloat4(&pLight->GetDiffuse());
	cb2.vLs = XMLoadFloat4(&pLight->GetSpecular());
	cb2.vDiffuse = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	cb2.vAmbient = XMVectorSet(0.3f, 0.3f, 0.3f, 1.0f);
	cb2.vSpecular = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	cb2.vEmissive = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	pDeviceContext->UpdateSubresource(m_pConstantBuffer[1],
		0, nullptr, &cb2, 0, 0);
	pDeviceContext->PSSetConstantBuffers(1, 1, &m_pConstantBuffer[1]);


	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	pDeviceContext->Draw(m_uDebris * 3, 0);
}


HRESULT CExplosion::SetModel(CAssimpModel* pModel)
{
	HRESULT hr = S_OK;
	if (!pModel) return E_FAIL;


	UINT nVertex = 0, nIndex = 0;
	pModel->GetVertexCount(&nVertex, &nIndex);
	if (!nVertex || !nIndex) return E_FAIL;
	SAFE_DELETE_ARRAY(m_pIndex);
	SAFE_DELETE_ARRAY(m_pVertex);
	m_pVertex = new TAssimpVertex[nVertex];
	m_uVertex = nVertex;
	m_pIndex = new UINT[nIndex];
	m_uIndex = nIndex;
	pModel->GetVertex(m_pVertex, m_pIndex);


	SetCenter(pModel->GetCenter());
	SetBBox(pModel->GetBBox());
	SetRadius(pModel->GetRadius());

	return hr;
}


HRESULT CExplosion::Start(XMFLOAT4X4* pWorld, XMFLOAT3* pVelocity)
{
	HRESULT hr = S_OK;


	m_uDebris = m_uIndex / 3;
	if (!m_uDebris) return E_FAIL;
	SAFE_DELETE_ARRAY(m_pDebris);
	m_pDebris = new CDebris[m_uDebris];
	UINT n = 0;
	for (UINT i = 0; i < m_uIndex; i += 3) {
		UINT i0 = m_pIndex[i + 0];
		UINT i1 = m_pIndex[i + 1];
		UINT i2 = m_pIndex[i + 2];
		if (i0 >= m_uVertex || i1 >= m_uVertex || i2 >= m_uVertex) {
			continue;
		}
		XMFLOAT3& v0 = m_pVertex[i0].vPos;
		XMFLOAT3& v1 = m_pVertex[i1].vPos;
		XMFLOAT3& v2 = m_pVertex[i2].vPos;
		hr = m_pDebris[n++].Init(v0, v1, v2, GetRadius(), pWorld, pVelocity);
		if (FAILED(hr)) {
			--n;
			continue;
		}
	}
	if (!n) {
		SAFE_DELETE_ARRAY(m_pDebris);
		return hr;
	}
	m_uDebris = n;


	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = sizeof(VERTEX) * m_uDebris * 3;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	VERTEX* vertex = new VERTEX[m_uDebris * 3];
	VERTEX* pVtx = vertex;
	for (UINT i = 0; i < m_uDebris; ++i, pVtx += 3) {
		m_pDebris[i].UpdateBuffer(pVtx);
	}
	initData.pSysMem = vertex;
	ID3D11Device* pDevice = GetDevice();
	hr = pDevice->CreateBuffer(&vbd, &initData, &m_pVertexBuffer);
	delete[] vertex;
	if (FAILED(hr)) {
		SAFE_DELETE_ARRAY(m_pDebris);
		return hr;
	}

	return hr;
}
