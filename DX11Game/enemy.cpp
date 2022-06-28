//=============================================================================
//
// 敵 クラス実装 [Enemy.cpp]
// Author : 伊地田真衣
//
//=============================================================================
#include "Enemy.h"
#include "Game.h"

// グローバル変数
namespace {
	const float VALUE_MOVE_ENEMY = 0.1f;
	const float VALUE_ROLL_ENEMY = 0.5f;
	const float VALUE_PITCH_ENEMY = 0.5f;
}

// コンストラクタ
CEnemy::CEnemy(CScene* pScene) : CModel(pScene) {
	SetRadius(0.1f);
	SetScale(XMFLOAT3(0.5f, 0.5f, 0.5f));
	m_id = GOT_ENEMY;
	m_pPlayer = nullptr;
}

// デストラクタ
CEnemy::~CEnemy() {
}

// 初期化
HRESULT CEnemy::Init() {
	HRESULT hr = CModel::Init();
	SetModel(MODEL_ENEMY);
	return hr;
}

// 終了処理
void CEnemy::Fin() {
	CModel::Fin();
}

// 更新
void CEnemy::Update() {
	// 位置を抽出、0クリア
	m_vPos.x = m_mWorld._41;
	m_vPos.y = m_mWorld._42;
	m_vPos.z = m_mWorld._43;
	m_mWorld._41 = 0.0f;
	m_mWorld._42 = 0.0f;
	m_mWorld._43 = 0.0f;
	// ローカルX軸、Z軸を抽出
	XMFLOAT3 vX(m_mWorld._11, m_mWorld._12, m_mWorld._13);
	XMFLOAT3 vZ(m_mWorld._31, m_mWorld._32, m_mWorld._33);
	// 回転
	XMMATRIX mRotate = XMMatrixIdentity();
	bool bRotate = false;
	// ロール
	if (0) {	//if (CInput::GetKeyPress(VK_LEFT) || CInput::GetKeyPress(VK_A)) {
		mRotate = XMMatrixMultiply(mRotate, XMMatrixRotationAxis(XMLoadFloat3(&vZ), XMConvertToRadians(VALUE_ROLL_ENEMY)));
		bRotate = true;
	}
	if (0) {	//if (CInput::GetKeyPress(VK_RIGHT) || CInput::GetKeyPress(VK_D)) {
		mRotate = XMMatrixMultiply(mRotate, XMMatrixRotationAxis(XMLoadFloat3(&vZ), XMConvertToRadians(-VALUE_ROLL_ENEMY)));
		bRotate = true;
	}
	// ピッチ
	if (0) {	//if (CInput::GetKeyPress(VK_UP) || CInput::GetKeyPress(VK_W)) {
		mRotate = XMMatrixMultiply(mRotate, XMMatrixRotationAxis(XMLoadFloat3(&vX), XMConvertToRadians(VALUE_PITCH_ENEMY)));
		bRotate = true;
	}
	if (1) {	//if (CInput::GetKeyPress(VK_DOWN) || CInput::GetKeyPress(VK_S)) {
		mRotate = XMMatrixMultiply(mRotate, XMMatrixRotationAxis(XMLoadFloat3(&vX), XMConvertToRadians(-VALUE_PITCH_ENEMY)));
		bRotate = true;
	}
	// 回転を反映
	if (bRotate) {
		XMStoreFloat4x4(&m_mWorld, XMMatrixMultiply(XMLoadFloat4x4(&m_mWorld), mRotate));
	}
	// 速度
	if (::GetKeyState(VK_CAPITAL) & 1) {
		m_vVel = XMFLOAT3(0, 0, 0);
	} else {
		m_vVel.x = vZ.x * VALUE_MOVE_ENEMY;
		m_vVel.y = vZ.y * VALUE_MOVE_ENEMY;
		m_vVel.z = vZ.z * VALUE_MOVE_ENEMY;
	}
	// 移動
	m_vPos.x += m_vVel.x;
	m_vPos.y += m_vVel.y;
	m_vPos.z += m_vVel.z;
	// 位置を反映
	m_mWorld._41 = m_vPos.x;
	m_mWorld._42 = m_vPos.y;
	m_mWorld._43 = m_vPos.z;
	// 衝突判定
	if (m_pPlayer) {
		switch (((CGame*)m_pScene)->GetBoundary()) {
		case 0:
			break;
		case 1:
			if (CollisionBSphere(m_pPlayer)) {
				m_pPlayer->SetBColor();
				SetBColor();
			}
			break;
		}
	}
}

void CEnemy::Draw() {
	//XMFLOAT4X4 mW = m_mWorld;	// 現在のワールド変換を保存しておく
	//XMFLOAT4X4 mWorkW = m_mWorld;	// 仮変換用のワーク
	////XMStoreFloat4x4(&mWorkW, XMMatrixMultiply(XMMatrixScaling(0.3f,0.3f,0.3f), XMLoadFloat4x4(&mWorkW)));
	//XMStoreFloat4x4(&mWorkW, XMMatrixMultiply(XMMatrixRotationY(XMConvertToRadians(180.0f)), XMLoadFloat4x4(&mWorkW)));
	//m_mWorld = mWorkW;
	CModel::Draw();
	//m_mWorld = mW;
}