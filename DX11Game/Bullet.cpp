//=============================================================================
//
// �e�N���X���� [Bullet.cpp]
// Author : �ɒn�c�^��
//
//=============================================================================
#include "Bullet.h"
#include "Game.h"

// �O���[�o���ϐ�
namespace {
	const float VALUE_MOVE_BULLET = 1.0f;
	const int MAX_LIFE = 300;
}

// �R���X�g���N�^
CBullet::CBullet(CScene* pScene) : CModel(pScene)
{
	m_id = GOT_BULLET;
	m_nLife = 0;
}

// �f�X�g���N�^
CBullet::~CBullet()
{
}

// ������
HRESULT CBullet::Init()
{
	HRESULT hr = CModel::Init();
	SetModel(MODEL_BULLET);
	m_nLife = MAX_LIFE;
	return hr;
}

// �I������
void CBullet::Fin()
{
	CModel::Fin();
}

// �X�V
void CBullet::Update()
{
	// �������ԃ`�F�b�N
	--m_nLife;
	if (m_nLife <= 0) {
		Fin();
		delete this;
		return;
	}
	// �ʒu�𒊏o
	m_vPos.x = m_mWorld._41;
	m_vPos.y = m_mWorld._42;
	m_vPos.z = m_mWorld._43;
	// ���[�J��Z���𒊏o
	XMFLOAT3 vZ(m_mWorld._31, m_mWorld._32, m_mWorld._33);
	// ���x
	m_vVel.x = vZ.x * VALUE_MOVE_BULLET;
	m_vVel.y = vZ.y * VALUE_MOVE_BULLET;
	m_vVel.z = vZ.z * VALUE_MOVE_BULLET;
	// �ړ�
	m_vPos.x += m_vVel.x;
	m_vPos.y += m_vVel.y;
	m_vPos.z += m_vVel.z;
	// �ʒu�𔽉f
	m_mWorld._41 = m_vPos.x;
	m_mWorld._42 = m_vPos.y;
	m_mWorld._43 = m_vPos.z;

	// �Փ˔���
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

// ����
CBullet* CBullet::Fire(CScene* pScene, XMFLOAT4X4& mtxWorld)
{
	CBullet* pBullet = new CBullet(pScene);
	pBullet->Init();
	pBullet->SetWorld(mtxWorld);
	return pBullet;
}
