//=============================================================================
//
// プレイヤー クラス実装 [Player.cpp]
// Author : 伊地田真衣
//
//=============================================================================
#include "Player.h"
#include "Input.h"
#include "DebugProc.h"
#include "Bullet.h"
#include "Missile.h"

// グローバル変数
namespace {
	const float VALUE_MOVE_PLAYER = 0.1f;
	const float VALUE_ROLL_PLAYER = 1.0f;
	const float VALUE_PITCH_PLAYER = 1.0f;
	const UINT MAX_BULLET = 120;
	const UINT MAX_MISSILE = 6;
}

// コンストラクタ
CPlayer::CPlayer(CScene* pScene) : CModel(pScene) {
	m_id = GOT_PLAYER;
	m_uTick = 0;
	m_uBullet = 0;
	m_uMissile = 0;
}

// デストラクタ
CPlayer::~CPlayer() {
}

// 初期化
HRESULT CPlayer::Init() {
	HRESULT hr = CModel::Init();
	SetModel(MODEL_PLAYER);
	m_uTick = 0;
	m_uBullet = MAX_BULLET;
	m_uMissile = MAX_MISSILE;
	return hr;
}

// 終了処理
void CPlayer::Fin() {
	CModel::Fin();
}

// 更新
void CPlayer::Update() {
	// タイマ更新
	++m_uTick;
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
	if (CInput::GetKeyPress(VK_LEFT) || CInput::GetKeyPress(VK_A)) {
		mRotate = XMMatrixMultiply(mRotate, XMMatrixRotationAxis(XMLoadFloat3(&vZ), XMConvertToRadians(VALUE_ROLL_PLAYER)));
		bRotate = true;
	}
	if (CInput::GetKeyPress(VK_RIGHT) || CInput::GetKeyPress(VK_D)) {
		mRotate = XMMatrixMultiply(mRotate, XMMatrixRotationAxis(XMLoadFloat3(&vZ), XMConvertToRadians(-VALUE_ROLL_PLAYER)));
		bRotate = true;
	}
	// ピッチ
	if (CInput::GetKeyPress(VK_UP) || CInput::GetKeyPress(VK_W)) {
		mRotate = XMMatrixMultiply(mRotate, XMMatrixRotationAxis(XMLoadFloat3(&vX), XMConvertToRadians(VALUE_PITCH_PLAYER)));
		bRotate = true;
	}
	if (CInput::GetKeyPress(VK_DOWN) || CInput::GetKeyPress(VK_S)) {
		mRotate = XMMatrixMultiply(mRotate, XMMatrixRotationAxis(XMLoadFloat3(&vX), XMConvertToRadians(-VALUE_PITCH_PLAYER)));
		bRotate = true;
	}
	// 回転を反映
	if (bRotate) {
		XMStoreFloat4x4(&m_mWorld, XMMatrixMultiply(XMLoadFloat4x4(&m_mWorld), mRotate));
	}
	// 速度
	if (CInput::GetKeyPress(VK_SHIFT)) {
		m_vVel = XMFLOAT3(0, 0, 0);
	} else {
		m_vVel.x = vZ.x * VALUE_MOVE_PLAYER;
		m_vVel.y = vZ.y * VALUE_MOVE_PLAYER;
		m_vVel.z = vZ.z * VALUE_MOVE_PLAYER;
	}
	// 移動
	m_vPos.x += m_vVel.x;
	m_vPos.y += m_vVel.y;
	m_vPos.z += m_vVel.z;
	// 位置を反映
	m_mWorld._41 = m_vPos.x;
	m_mWorld._42 = m_vPos.y;
	m_mWorld._43 = m_vPos.z;

	// 弾発射
	if (CInput::GetKeyPress(VK_CONTROL) && m_uBullet && (m_uTick & 1)) {
		CBullet::Fire(m_pScene, GetWorld());
		--m_uBullet;
	}
	// 追尾ミサイル発射
	if (CInput::GetKeyTrigger(VK_M) && m_uMissile) {
		CMissile::Fire(m_pScene, GetWorld());
		--m_uMissile;
	}
	// リロード
	if (CInput::GetKeyTrigger(VK_R)) {
		m_uBullet = MAX_BULLET;
		m_uMissile = MAX_MISSILE;
	}
}

void CPlayer::Draw() {
	XMFLOAT4X4 mW = m_mWorld;
	XMFLOAT4X4 mRotW;
	XMStoreFloat4x4(&mRotW, XMMatrixMultiply(XMMatrixRotationY(XMConvertToRadians(180.0f)), XMLoadFloat4x4(&mW)));
	m_mWorld = mRotW;
	CModel::Draw();
	m_mWorld = mW;
}