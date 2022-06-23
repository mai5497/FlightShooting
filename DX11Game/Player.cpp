//=============================================================================
//
// �v���C���[ �N���X���� [Player.cpp]
// Author : �ɒn�c�^��
//
//=============================================================================
#include "Player.h"
#include "Input.h"
#include "DebugProc.h"
#include "Bullet.h"
#include "Missile.h"

// �O���[�o���ϐ�
namespace {
	const float VALUE_MOVE_PLAYER = 0.1f;
	const float VALUE_ROLL_PLAYER = 1.0f;
	const float VALUE_PITCH_PLAYER = 1.0f;
	const UINT MAX_BULLET = 120;
	const UINT MAX_MISSILE = 6;
}

// �R���X�g���N�^
CPlayer::CPlayer(CScene* pScene) : CModel(pScene) {
	m_id = GOT_PLAYER;
	m_uTick = 0;
	m_uBullet = 0;
	m_uMissile = 0;
}

// �f�X�g���N�^
CPlayer::~CPlayer() {
}

// ������
HRESULT CPlayer::Init() {
	HRESULT hr = CModel::Init();
	SetModel(MODEL_PLAYER);
	m_uTick = 0;
	m_uBullet = MAX_BULLET;
	m_uMissile = MAX_MISSILE;
	return hr;
}

// �I������
void CPlayer::Fin() {
	CModel::Fin();
}

// �X�V
void CPlayer::Update() {
	// �^�C�}�X�V
	++m_uTick;
	// �ʒu�𒊏o�A0�N���A
	m_vPos.x = m_mWorld._41;
	m_vPos.y = m_mWorld._42;
	m_vPos.z = m_mWorld._43;
	m_mWorld._41 = 0.0f;
	m_mWorld._42 = 0.0f;
	m_mWorld._43 = 0.0f;
	// ���[�J��X���AZ���𒊏o
	XMFLOAT3 vX(m_mWorld._11, m_mWorld._12, m_mWorld._13);
	XMFLOAT3 vZ(m_mWorld._31, m_mWorld._32, m_mWorld._33);
	// ��]
	XMMATRIX mRotate = XMMatrixIdentity();
	bool bRotate = false;
	// ���[��
	if (CInput::GetKeyPress(VK_LEFT) || CInput::GetKeyPress(VK_A)) {
		mRotate = XMMatrixMultiply(mRotate, XMMatrixRotationAxis(XMLoadFloat3(&vZ), XMConvertToRadians(VALUE_ROLL_PLAYER)));
		bRotate = true;
	}
	if (CInput::GetKeyPress(VK_RIGHT) || CInput::GetKeyPress(VK_D)) {
		mRotate = XMMatrixMultiply(mRotate, XMMatrixRotationAxis(XMLoadFloat3(&vZ), XMConvertToRadians(-VALUE_ROLL_PLAYER)));
		bRotate = true;
	}
	// �s�b�`
	if (CInput::GetKeyPress(VK_UP) || CInput::GetKeyPress(VK_W)) {
		mRotate = XMMatrixMultiply(mRotate, XMMatrixRotationAxis(XMLoadFloat3(&vX), XMConvertToRadians(VALUE_PITCH_PLAYER)));
		bRotate = true;
	}
	if (CInput::GetKeyPress(VK_DOWN) || CInput::GetKeyPress(VK_S)) {
		mRotate = XMMatrixMultiply(mRotate, XMMatrixRotationAxis(XMLoadFloat3(&vX), XMConvertToRadians(-VALUE_PITCH_PLAYER)));
		bRotate = true;
	}
	// ��]�𔽉f
	if (bRotate) {
		XMStoreFloat4x4(&m_mWorld, XMMatrixMultiply(XMLoadFloat4x4(&m_mWorld), mRotate));
	}
	// ���x
	if (CInput::GetKeyPress(VK_SHIFT)) {
		m_vVel = XMFLOAT3(0, 0, 0);
	} else {
		m_vVel.x = vZ.x * VALUE_MOVE_PLAYER;
		m_vVel.y = vZ.y * VALUE_MOVE_PLAYER;
		m_vVel.z = vZ.z * VALUE_MOVE_PLAYER;
	}
	// �ړ�
	m_vPos.x += m_vVel.x;
	m_vPos.y += m_vVel.y;
	m_vPos.z += m_vVel.z;
	// �ʒu�𔽉f
	m_mWorld._41 = m_vPos.x;
	m_mWorld._42 = m_vPos.y;
	m_mWorld._43 = m_vPos.z;

	// �e����
	if (CInput::GetKeyPress(VK_CONTROL) && m_uBullet && (m_uTick & 1)) {
		CBullet::Fire(m_pScene, GetWorld());
		--m_uBullet;
	}
	// �ǔ��~�T�C������
	if (CInput::GetKeyTrigger(VK_M) && m_uMissile) {
		CMissile::Fire(m_pScene, GetWorld());
		--m_uMissile;
	}
	// �����[�h
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