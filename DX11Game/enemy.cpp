//=============================================================================
//
// �G �N���X���� [Enemy.cpp]
// Author : �ɒn�c�^��
//
//=============================================================================
#include "Enemy.h"
#include "Game.h"

// �O���[�o���ϐ�
namespace {
	const float VALUE_MOVE_ENEMY = 0.1f;
	const float VALUE_ROLL_ENEMY = 0.5f;
	const float VALUE_PITCH_ENEMY = 0.5f;
}

// �R���X�g���N�^
CEnemy::CEnemy(CScene* pScene) : CModel(pScene) {
	SetRadius(0.1f);
	SetScale(XMFLOAT3(0.5f, 0.5f, 0.5f));
	m_id = GOT_ENEMY;
	m_pPlayer = nullptr;
}

// �f�X�g���N�^
CEnemy::~CEnemy() {
}

// ������
HRESULT CEnemy::Init() {
	HRESULT hr = CModel::Init();
	SetModel(MODEL_ENEMY);
	return hr;
}

// �I������
void CEnemy::Fin() {
	CModel::Fin();
}

// �X�V
void CEnemy::Update() {
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
	if (0) {	//if (CInput::GetKeyPress(VK_LEFT) || CInput::GetKeyPress(VK_A)) {
		mRotate = XMMatrixMultiply(mRotate, XMMatrixRotationAxis(XMLoadFloat3(&vZ), XMConvertToRadians(VALUE_ROLL_ENEMY)));
		bRotate = true;
	}
	if (0) {	//if (CInput::GetKeyPress(VK_RIGHT) || CInput::GetKeyPress(VK_D)) {
		mRotate = XMMatrixMultiply(mRotate, XMMatrixRotationAxis(XMLoadFloat3(&vZ), XMConvertToRadians(-VALUE_ROLL_ENEMY)));
		bRotate = true;
	}
	// �s�b�`
	if (0) {	//if (CInput::GetKeyPress(VK_UP) || CInput::GetKeyPress(VK_W)) {
		mRotate = XMMatrixMultiply(mRotate, XMMatrixRotationAxis(XMLoadFloat3(&vX), XMConvertToRadians(VALUE_PITCH_ENEMY)));
		bRotate = true;
	}
	if (1) {	//if (CInput::GetKeyPress(VK_DOWN) || CInput::GetKeyPress(VK_S)) {
		mRotate = XMMatrixMultiply(mRotate, XMMatrixRotationAxis(XMLoadFloat3(&vX), XMConvertToRadians(-VALUE_PITCH_ENEMY)));
		bRotate = true;
	}
	// ��]�𔽉f
	if (bRotate) {
		XMStoreFloat4x4(&m_mWorld, XMMatrixMultiply(XMLoadFloat4x4(&m_mWorld), mRotate));
	}
	// ���x
	if (::GetKeyState(VK_CAPITAL) & 1) {
		m_vVel = XMFLOAT3(0, 0, 0);
	} else {
		m_vVel.x = vZ.x * VALUE_MOVE_ENEMY;
		m_vVel.y = vZ.y * VALUE_MOVE_ENEMY;
		m_vVel.z = vZ.z * VALUE_MOVE_ENEMY;
	}
	// �ړ�
	m_vPos.x += m_vVel.x;
	m_vPos.y += m_vVel.y;
	m_vPos.z += m_vVel.z;
	// �ʒu�𔽉f
	m_mWorld._41 = m_vPos.x;
	m_mWorld._42 = m_vPos.y;
	m_mWorld._43 = m_vPos.z;
	// �Փ˔���
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
	//XMFLOAT4X4 mW = m_mWorld;	// ���݂̃��[���h�ϊ���ۑ����Ă���
	//XMFLOAT4X4 mWorkW = m_mWorld;	// ���ϊ��p�̃��[�N
	////XMStoreFloat4x4(&mWorkW, XMMatrixMultiply(XMMatrixScaling(0.3f,0.3f,0.3f), XMLoadFloat4x4(&mWorkW)));
	//XMStoreFloat4x4(&mWorkW, XMMatrixMultiply(XMMatrixRotationY(XMConvertToRadians(180.0f)), XMLoadFloat4x4(&mWorkW)));
	//m_mWorld = mWorkW;
	CModel::Draw();
	//m_mWorld = mW;
}