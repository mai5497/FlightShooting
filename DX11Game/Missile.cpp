//********************************************************************************
//
// �ǔ��~�T�C���N���X����
// �ҏW�ҁF�ɒn�c�^��
//
//********************************************************************************
#include "Missile.h"
#include "Game.h"

// �O���[�o���ϐ�
namespace {
	const float VALUE_MOVE_MISSILE = 0.5f;
	const float VALUE_ROTATE_MISSILE = 0.7f;
	const int MAX_LIFE = 600;
}

// �R���X�g���N�^
CMissile::CMissile(CScene* pScene) : CModel(pScene) {
	m_id = GOT_MISSILE;
	m_nLife = 0;
	m_pTarget = nullptr;
}

// �f�X�g���N�^
CMissile::~CMissile() {
}

// ������
HRESULT CMissile::Init() {
	HRESULT hr = CModel::Init();
	SetModel(MODEL_MISSILE);
	m_nLife = MAX_LIFE;
	m_pTarget = nullptr;
	switch (((CGame*)m_pScene)->GetBoundary()) {
	case 0:	// ���E�\������
		SetBSphereVisible(false);
		break;
	case 1:	// ���E��
		SetBSphereVisible();
		break;
	}
	return hr;
}

// �X�V
void CMissile::Update() {
	// �������ԃ`�F�b�N
	--m_nLife;
	if (m_nLife <= 0 || !IsVisible()) {
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
	// �^�[�Q�b�g���L��?
	if (m_pTarget) {
		if (!m_pTarget->IsVisible()) {
			m_pTarget = nullptr;
		}
	}
	// �^�[�Q�b�g������?
	if (m_pTarget == nullptr) {
		// �ł��߂��G�@��ݒ�
		float d = FLT_MAX;
		for (CGameObj* pObj = m_pScene->GetObj(); pObj; pObj = pObj->GetNext()) {
			if (pObj->GetID() != GOT_ENEMY || !pObj->IsVisible())
				continue;
			// ����(��2��)�����߂�
			XMFLOAT3& vPos = pObj->GetPos();
			float dx = m_vPos.x - vPos.x;
			float dy = m_vPos.y - vPos.y;
			float dz = m_vPos.z - vPos.z;
			float r = dx * dx + dy * dy + dz * dz;
			// ���߂���΍X�V
			if (d > r) {
				d = r;
				m_pTarget = pObj;
			}
		}
	}
	// �^�[�Q�b�g���L��?
	if (m_pTarget) {
		// �^�[�Q�b�g�ւ̃x�N�g��
		XMFLOAT3& vPos = m_pTarget->GetPos();
		XMFLOAT3 vTarget;
		vTarget.x = vPos.x - m_vPos.x;
		vTarget.y = vPos.y - m_vPos.y;
		vTarget.z = vPos.z - m_vPos.z;
		// ��]�������߂�
		XMVECTOR vAxis;
		vAxis = XMVector3Normalize(XMVector3Cross(XMLoadFloat3(&vZ), XMLoadFloat3(&vTarget)));
		// 90����]�}�g���b�N�X
		XMMATRIX mR = XMMatrixRotationAxis(vAxis, XMConvertToRadians(90));
		// �i�s������90����]�x�N�g��
		XMVECTOR vR = XMVector3TransformNormal(XMLoadFloat3(&vZ), mR);
		// ���ς����߂�
		float fDot;
		XMStoreFloat(&fDot, XMVector3Dot(vR, XMLoadFloat3(&vTarget)));
		// �����Ŕ���
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
			// �ʒu��0�N���A
			m_mWorld._41 = 0.0f;
			m_mWorld._42 = 0.0f;
			m_mWorld._43 = 0.0f;
			// ��]�𔽉f
			XMStoreFloat4x4(&m_mWorld,
				XMMatrixMultiply(XMLoadFloat4x4(&m_mWorld), mR));
		}
	}
	// ���x
	m_vVel.x = vZ.x * VALUE_MOVE_MISSILE;
	m_vVel.y = vZ.y * VALUE_MOVE_MISSILE;
	m_vVel.z = vZ.z * VALUE_MOVE_MISSILE;
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

// ����
CMissile* CMissile::Fire(CScene* pScene, XMFLOAT4X4& mtxWorld, CGameObj* pTarget) {
	CMissile* pMissile = new CMissile(pScene);
	pMissile->Init();
	pMissile->SetWorld(mtxWorld);
	pMissile->m_pTarget = pTarget;
	return pMissile;
}
