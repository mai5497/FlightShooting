//=============================================================================
//
// 弾クラス実装 [Bullet.cpp]
// Author : 伊地田真衣
//
//=============================================================================
#include "Bullet.h"
#include "Game.h"

// グローバル変数
namespace {
	const float VALUE_MOVE_BULLET = 1.0f;
	const int MAX_LIFE = 300;
}

// コンストラクタ
CBullet::CBullet(CScene* pScene) : CModel(pScene)
{
	m_id = GOT_BULLET;
	m_nLife = 0;
}

// デストラクタ
CBullet::~CBullet()
{
}

// 初期化
HRESULT CBullet::Init()
{
	HRESULT hr = CModel::Init();
	SetModel(MODEL_BULLET);
	m_nLife = MAX_LIFE;
	return hr;
}

// 終了処理
void CBullet::Fin()
{
	CModel::Fin();
}

// 更新
void CBullet::Update()
{
	// 生存期間チェック
	--m_nLife;
	if (m_nLife <= 0) {
		Fin();
		delete this;
		return;
	}
	// 位置を抽出
	m_vPos.x = m_mWorld._41;
	m_vPos.y = m_mWorld._42;
	m_vPos.z = m_mWorld._43;
	// ローカルZ軸を抽出
	XMFLOAT3 vZ(m_mWorld._31, m_mWorld._32, m_mWorld._33);
	// 速度
	m_vVel.x = vZ.x * VALUE_MOVE_BULLET;
	m_vVel.y = vZ.y * VALUE_MOVE_BULLET;
	m_vVel.z = vZ.z * VALUE_MOVE_BULLET;
	// 移動
	m_vPos.x += m_vVel.x;
	m_vPos.y += m_vVel.y;
	m_vPos.z += m_vVel.z;
	// 位置を反映
	m_mWorld._41 = m_vPos.x;
	m_mWorld._42 = m_vPos.y;
	m_mWorld._43 = m_vPos.z;

	// 衝突判定
	CGameObj* pNext;
	for (CGameObj* pObj = m_pScene->GetObj(); pObj; pObj = pNext) {
		pNext = pObj->GetNext();
		if (pObj->GetID() != GOT_ENEMY) {
			continue;
		}
		switch (((CGame*)m_pScene)->GetBoundary()) {
		case 0:
			if (CollisionBSphere(pObj)) {
				pObj->SetVisible(false);
				Fin();
				delete this;
				return;
			}
			break;
		case 1:
			if (CollisionBSphere(pObj)) {
				pObj->SetBColor();
				SetBColor();
			}
			break;
		}
	}
}

// 発射
CBullet* CBullet::Fire(CScene* pScene, XMFLOAT4X4& mtxWorld)
{
	CBullet* pBullet = new CBullet(pScene);
	pBullet->Init();
	pBullet->SetWorld(mtxWorld);
	return pBullet;
}
