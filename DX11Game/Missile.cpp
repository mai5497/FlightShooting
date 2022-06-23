//********************************************************************************
//
// 追尾ミサイルクラス実装
// 編集者：伊地田真衣
//
//********************************************************************************
#include "Missile.h"
#include "Game.h"

// グローバル変数
namespace {
	const float VALUE_MOVE_MISSILE = 0.5f;
	const float VALUE_ROTATE_MISSILE = 0.7f;
	const int MAX_LIFE = 600;
}

// コンストラクタ
CMissile::CMissile(CScene* pScene) : CModel(pScene) {
	m_id = GOT_MISSILE;
	m_nLife = 0;
	m_pTarget = nullptr;
}

// デストラクタ
CMissile::~CMissile() {
}

// 初期化
HRESULT CMissile::Init() {
	HRESULT hr = CModel::Init();
	SetModel(MODEL_MISSILE);
	m_nLife = MAX_LIFE;
	m_pTarget = nullptr;
	switch (((CGame*)m_pScene)->GetBoundary()) {
	case 0:	// 境界表示無し
		SetBSphereVisible(false);
		break;
	case 1:	// 境界球
		SetBSphereVisible();
		break;
	}
	return hr;
}

// 更新
void CMissile::Update() {
	// 生存期間チェック
	--m_nLife;
	if (m_nLife <= 0 || !IsVisible()) {
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
	// ターゲットが有効?
	if (m_pTarget) {
		if (!m_pTarget->IsVisible()) {
			m_pTarget = nullptr;
		}
	}
	// ターゲットが無効?
	if (m_pTarget == nullptr) {
		// 最も近い敵機を設定
		float d = FLT_MAX;
		for (CGameObj* pObj = m_pScene->GetObj(); pObj; pObj = pObj->GetNext()) {
			if (pObj->GetID() != GOT_ENEMY || !pObj->IsVisible())
				continue;
			// 距離(の2乗)を求める
			XMFLOAT3& vPos = pObj->GetPos();
			float dx = m_vPos.x - vPos.x;
			float dy = m_vPos.y - vPos.y;
			float dz = m_vPos.z - vPos.z;
			float r = dx * dx + dy * dy + dz * dz;
			// より近ければ更新
			if (d > r) {
				d = r;
				m_pTarget = pObj;
			}
		}
	}
	// ターゲットが有効?
	if (m_pTarget) {
		// ターゲットへのベクトル
		XMFLOAT3& vPos = m_pTarget->GetPos();
		XMFLOAT3 vTarget;
		vTarget.x = vPos.x - m_vPos.x;
		vTarget.y = vPos.y - m_vPos.y;
		vTarget.z = vPos.z - m_vPos.z;
		// 回転軸を求める
		XMVECTOR vAxis;
		vAxis = XMVector3Normalize(XMVector3Cross(XMLoadFloat3(&vZ), XMLoadFloat3(&vTarget)));
		// 90°回転マトリックス
		XMMATRIX mR = XMMatrixRotationAxis(vAxis, XMConvertToRadians(90));
		// 進行方向の90°回転ベクトル
		XMVECTOR vR = XMVector3TransformNormal(XMLoadFloat3(&vZ), mR);
		// 内積を求める
		float fDot;
		XMStoreFloat(&fDot, XMVector3Dot(vR, XMLoadFloat3(&vTarget)));
		// 符号で判定
		mR = XMMatrixIdentity();
		bool bR = false;
		if (fDot > 0.0f) {
			mR = XMMatrixRotationAxis(vAxis,
				XMConvertToRadians(VALUE_ROTATE_MISSILE));
			bR = true;
		} else if (fDot < 0.0f) {
			mR = XMMatrixRotationAxis(vAxis,
				XMConvertToRadians(-VALUE_ROTATE_MISSILE));
			bR = true;
		} else {
			XMStoreFloat(&fDot,
				XMVector3Dot(XMLoadFloat3(&vZ), XMLoadFloat3(&vTarget)));
			if (fDot < 0.0f) {
				if (rand() & 1) {
					mR = XMMatrixRotationAxis(vAxis,
						XMConvertToRadians(VALUE_ROTATE_MISSILE));
				} else {
					mR = XMMatrixRotationAxis(vAxis,
						XMConvertToRadians(-VALUE_ROTATE_MISSILE));
				}
				bR = true;
			}
		}
		if (bR) {
			// 位置を0クリア
			m_mWorld._41 = 0.0f;
			m_mWorld._42 = 0.0f;
			m_mWorld._43 = 0.0f;
			// 回転を反映
			XMStoreFloat4x4(&m_mWorld,
				XMMatrixMultiply(XMLoadFloat4x4(&m_mWorld), mR));
		}
	}
	// 速度
	m_vVel.x = vZ.x * VALUE_MOVE_MISSILE;
	m_vVel.y = vZ.y * VALUE_MOVE_MISSILE;
	m_vVel.z = vZ.z * VALUE_MOVE_MISSILE;
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
		switch (pObj->GetID()) {
		case GOT_ENEMY:
		case GOT_BULLET:
		case GOT_MISSILE:
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
			break;
		default:
			break;
		}
	}
}

// 発射
CMissile* CMissile::Fire(CScene* pScene, XMFLOAT4X4& mtxWorld, CGameObj* pTarget) {
	CMissile* pMissile = new CMissile(pScene);
	pMissile->Init();
	pMissile->SetWorld(mtxWorld);
	pMissile->m_pTarget = pTarget;
	return pMissile;
}
