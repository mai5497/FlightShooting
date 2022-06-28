// レーダー [Radar.cpp]
#include "Radar.h"
#include "Texture.h"
#include "Polygon.h"

// グローバル変数
namespace {
	LPCTSTR PATH_TEXTURE[MAX_RT] = {
		_T("data/texture/radar.png"),
		_T("data/texture/bright_spot.png"),
	};
	const XMFLOAT4 BRIGHTSPOT_COLOR = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	const float BRIGHTSPOT_RADIUS = 8.0f;	// 輝点半径
	const float RADAR_RANGE = 15000.0f;		// レーダー探知半径(ワールド座標系)
	const float RADAR_RADIUS = 128.0f;		// レーダー表示半径(スクリーン座標系)
	const XMFLOAT4 RADAR_COLOR = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f);
	const float RADAR_POS_X = SCREEN_WIDTH / -2 + RADAR_RADIUS;
	const float RADAR_POS_Y = SCREEN_HEIGHT / -2 + RADAR_RADIUS;
}

// 静的メンバ変数
ID3D11ShaderResourceView* CRadar::m_pTexture[MAX_RT];

// 初期化
void CRadar::Init(CGame* pScene)
{
	m_pScene = pScene;
	m_bVisible = true;
	for (int i = 0; i < MAX_ENEMY; ++i) {
		m_vPos[i] = XMFLOAT3(RADAR_RANGE * 2, RADAR_RANGE * 2, RADAR_RANGE * 2);
		m_vPos2D[i] = XMFLOAT3(RADAR_RADIUS * 2, RADAR_RADIUS * 2, RADAR_RADIUS * 2);
	}
	m_nEnemy = 0;
	m_pPlayer = m_pScene->GetPlayer();
}

// 更新
void CRadar::Update()
{
	if (!m_bVisible) return;

	m_nEnemy = 0;
	XMFLOAT3& vPos = m_pPlayer->GetPos();
	for (CGameObj* pObj = m_pScene->GetObj(); pObj; pObj = pObj->GetNext()) {
		if (pObj->GetID() != GOT_ENEMY || !pObj->IsVisible())
			continue;
		m_vPos[m_nEnemy] = pObj->GetPos();
		float dx = m_vPos[m_nEnemy].x - vPos.x;
		float dy = m_vPos[m_nEnemy].y - vPos.y;
		float dz = m_vPos[m_nEnemy].z - vPos.z;
		if (dx * dx + dy * dy + dz * dz > RADAR_RANGE * RADAR_RANGE)
			continue;
		++m_nEnemy;
		if (m_nEnemy >= MAX_ENEMY)
			break;
	}
	XMMATRIX mInvWorld = m_pPlayer->GetInvWorld();
	XMFLOAT3 vPos2D;
	float fRange = m_pPlayer->GetRadius() * 10.0f;
	for (int i = 0; i < m_nEnemy; ++i) {
		XMStoreFloat3(&vPos2D,
			XMVector3TransformCoord(XMLoadFloat3(&m_vPos[i]), mInvWorld));
		m_vPos2D[i].x = RADAR_POS_X + vPos2D.x * (RADAR_RADIUS - BRIGHTSPOT_RADIUS) / RADAR_RANGE;
		m_vPos2D[i].y = RADAR_POS_Y + vPos2D.z * (RADAR_RADIUS - BRIGHTSPOT_RADIUS) / RADAR_RANGE;
		m_vPos2D[i].z = (vPos2D.y > fRange) ? 1.0f : ((vPos2D.y < -fRange) ? -1.0f : 0.0f);
	}
}

// 描画
void CRadar::Draw()
{
	if (!m_bVisible) return;

	ID3D11DeviceContext* pDC = GetDeviceContext();

	SetZBuffer(false);
	CPolygon::SetUV(0.0f, 0.0f);
	CPolygon::SetFrameSize(1.0f, 1.0f);

	// レーダー画面
	SetBlendState(BS_ALPHABLEND);
	CPolygon::SetColor(RADAR_COLOR);
	CPolygon::SetTexture(m_pTexture[RT_RADAR]);
	CPolygon::SetSize(RADAR_RADIUS * 2, RADAR_RADIUS * 2);
	CPolygon::SetPos(RADAR_POS_X, RADAR_POS_Y);
	CPolygon::Draw(pDC);

	// 輝点
	SetBlendState(BS_ALPHABLEND);
	CPolygon::SetTexture(m_pTexture[RT_BRIGHTSPOT]);
	CPolygon::SetSize(BRIGHTSPOT_RADIUS * 2, BRIGHTSPOT_RADIUS * 2);
	for (int i = 0; i < m_nEnemy; ++i) {
		CPolygon::SetColor(BRIGHTSPOT_COLOR.x * (m_vPos2D[i].z < 0.0f),
			BRIGHTSPOT_COLOR.y * (m_vPos2D[i].z == 0.0f),
			BRIGHTSPOT_COLOR.z * (m_vPos2D[i].z >= 0.0f),
			BRIGHTSPOT_COLOR.w);
		CPolygon::SetPos(m_vPos2D[i].x, m_vPos2D[i].y);
		CPolygon::Draw(pDC);
	}

	CPolygon::SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	SetBlendState(BS_NONE);
	SetZBuffer(true);
}

// テクスチャ読込
HRESULT CRadar::LoadTexture()
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr = S_OK;

	ReleaseTexture();
	// テクスチャの読み込み
	for (int i = 0; i < MAX_RT; ++i) {
		hr = CreateTextureFromFile(pDevice, PATH_TEXTURE[i], &m_pTexture[i]);
		if (FAILED(hr)) {
			break;
		}
	}
	return hr;
}

// テクスチャ解放
void CRadar::ReleaseTexture()
{
	for (int i = 0; i < MAX_RT; ++i) {
		SAFE_RELEASE(m_pTexture[i]);
	}
}
