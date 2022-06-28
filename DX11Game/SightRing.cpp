// 照準環 [SightRing.cpp]
#include <stdio.h>
#include "SightRing.h"
#include "Texture.h"
#include "Polygon.h"
#include "Camera.h"

// グローバル変数
namespace {
	LPCTSTR PATH_TEXTURE[MAX_SRT] = {
		_T("data/texture/sight_ring.png"),
		_T("data/texture/PressStart2Ph.png"),
	};
	float FONT_WIDTH = 16.0f;
	float FONT_HEIGHT = 16.0f;
}

// 静的メンバ変数
ID3D11ShaderResourceView* CSightRing::m_pTexture[MAX_SRT];

// 更新
void CSightRing::Update()
{
	if (!m_bVisible) return;

	CCamera* pCamera = CCamera::Get();
	XMFLOAT4X4& mV = pCamera->GetViewMatrix();
	XMVECTOR vZ = XMVectorSet(mV._13, mV._23, mV._33, 0.0f);
	XMFLOAT3& vCam = pCamera->GetPos();
	XMVECTOR vT = XMVectorSet(m_vWorldPos.x - vCam.x,
		m_vWorldPos.y - vCam.y, m_vWorldPos.z - vCam.z, 0.0f);
	float fDot;
	XMStoreFloat(&fDot, XMVector3Dot(vZ, vT));
	if (fDot <= 0.0f) {
		m_bHide = true;
		return;
	}
	float fDist;
	XMStoreFloat(&fDist, XMVector3Length(vT));
	sprintf_s(m_szStat, sizeof(m_szStat), "%.1f", fDist);
	m_bHide = false;
	XMFLOAT4X4& mP = pCamera->GetProjMatrix();
	// ビルボード用ワールド変換
	XMFLOAT4X4 mW(mV._11, mV._21, mV._31, 0.0f,
		mV._12, mV._22, mV._32, 0.0f,
		mV._13, mV._23, mV._33, 0.0f,
		m_vWorldPos.x, m_vWorldPos.y, m_vWorldPos.z, 1.0f);
	// ビューポート変換
	XMFLOAT4X4 mVP(SCREEN_WIDTH / 2.0f, 0.0f, 0.0f, 0.0f,
		0.0f, SCREEN_HEIGHT / 2.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	// マトリックスを合成
	XMMATRIX matrix = XMLoadFloat4x4(&mW) * XMLoadFloat4x4(&mV)
		* XMLoadFloat4x4(&mP) * XMLoadFloat4x4(&mVP);
	// 中心座標、左下座標、右上座標を2D座標に変換
	static const XMFLOAT3 v[] = {
		XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT3(-m_fRadius, -m_fRadius, 0.0f),
		XMFLOAT3(m_fRadius, m_fRadius, 0.0f),
	};
	XMFLOAT4 vPos[3];
	for (int i = 0; i < 3; ++i) {
		XMStoreFloat4(&vPos[i],
			XMVector3Transform(XMLoadFloat3(&v[i]), matrix));
		// 同次頂点を非同次頂点に変換
		vPos[i].x /= vPos[i].w;
		vPos[i].y /= vPos[i].w;
		vPos[i].z /= vPos[i].w;
	}
	// 照準環中心
	m_vPos.x = vPos[0].x;
	m_vPos.y = vPos[0].y;
	// 照準環サイズ
	m_vSize.x = vPos[2].x - vPos[1].x;
	m_vSize.y = vPos[2].y - vPos[1].y;
	// 照準環文字表示位置(左上)
	m_vLoc.x = vPos[1].x;
	m_vLoc.y = vPos[2].y;
}

// 描画
void CSightRing::DrawString(ID3D11DeviceContext* pDC)
{
	XMFLOAT2 vPos(m_vLoc.x + FONT_WIDTH * 0.5f,
		m_vLoc.y + FONT_HEIGHT * 0.5f);
	CPolygon::SetTexture(m_pTexture[SRT_FONT]);
	CPolygon::SetFrameSize(8.0f / 128.0f, 8.0f / 128.0f);
	CPolygon::SetSize(FONT_WIDTH, FONT_HEIGHT);
	for (const char* pChr = m_szStat; *pChr; ++pChr) {
		if (*pChr == '\n') {
			vPos.x = m_vLoc.x + FONT_WIDTH * 0.5f;
			vPos.y += FONT_HEIGHT;
			continue;
		}
		int nChr = (BYTE)*pChr;
		CPolygon::SetPos(vPos.x, vPos.y);
		CPolygon::SetUV((nChr & 15) / 16.0f, (nChr >> 4) / 16.0f);
		CPolygon::Draw(pDC);
		vPos.x += FONT_WIDTH;
	}
}

void CSightRing::Draw()
{
	if (!m_bVisible || m_bHide) return;

	ID3D11DeviceContext* pDC = GetDeviceContext();

	SetZBuffer(false);
	SetBlendState(BS_ALPHABLEND);
	CPolygon::SetColor(m_vColor);

	DrawString(pDC);

	CPolygon::SetTexture(m_pTexture[SRT_SIGHTRING]);
	CPolygon::SetPos(m_vPos.x, m_vPos.y);
	CPolygon::SetSize(m_vSize.x, m_vSize.y);
	CPolygon::SetUV(0.0f, 0.0f);
	CPolygon::SetFrameSize(1.0f, 1.0f);
	CPolygon::Draw(pDC);

	CPolygon::SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	SetBlendState(BS_NONE);
	SetZBuffer(true);
}

// 生成
CSightRing* CSightRing::Create(XMFLOAT3 pos, float fRadius)
{
	CSightRing* pSightRing = new CSightRing();
	pSightRing->m_bVisible = false;
	pSightRing->m_bHide = false;
	pSightRing->m_vColor = XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);
	pSightRing->m_vPos = XMFLOAT2(0.0f, 0.0f);
	pSightRing->m_vSize = XMFLOAT2(0.0f, 0.0f);
	pSightRing->m_vWorldPos = pos;
	pSightRing->m_fRadius = fRadius;
	pSightRing->m_szStat[0] = '\0';
	pSightRing->Update();
	return pSightRing;
}

// 移動
void CSightRing::Move(XMFLOAT3 pos)
{
	m_vWorldPos = pos;
}

// 色設定
void CSightRing::SetColor(XMFLOAT4 color)
{
	m_vColor = color;
}

// 解放
void CSightRing::Release()
{
	delete this;
}

// テクスチャ読込
HRESULT CSightRing::LoadTexture()
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr = S_OK;

	ReleaseTexture();
	// テクスチャの読み込み
	for (int i = 0; i < MAX_SRT; ++i) {
		hr = CreateTextureFromFile(pDevice, PATH_TEXTURE[i], &m_pTexture[i]);
		if (FAILED(hr)) {
			break;
		}
	}
	return hr;
}

// テクスチャ解放
void CSightRing::ReleaseTexture()
{
	for (int i = 0; i < MAX_SRT; ++i) {
		SAFE_RELEASE(m_pTexture[i]);
	}
}
