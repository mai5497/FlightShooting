//=============================================================================
//
// �I�u�W�F�N�g �N���X���� [GameObj.cpp]
// Author : �ɒn�c�^��
//
//=============================================================================
#include "GameObj.h"
#include "Scene.h"

// �O���[�o���ϐ�
namespace {
	const CMeshMaterial g_material = {
		XMFLOAT4(0.0f,1.0f,0.0f,0.3f),
		XMFLOAT4(0.0f,1.0f,0.0f,0.3f),
		XMFLOAT4(0.0f,0.0f,0.0f,1.0f),
		XMFLOAT4(0.0f,0.0f,0.0f,1.0f),
		1.0f
	};
	const XMFLOAT4 g_vRed(1.0f, 0.0f, 0.0f, 0.3f);
	const XMFLOAT4 g_vGreen(0.0f, 1.0f, 0.0f, 0.3f);
}

// �R���X�g���N�^
CGameObj::CGameObj(CScene* pScene) : m_pScene(pScene)
{
	m_id = GOT_GAMEOBJ;

	m_pBack = NULL;
	CGameObj* pTop = m_pScene->GetObj();
	m_pNext = pTop;
	if (pTop) {
		pTop->m_pBack = this;
	}
	m_pScene->SetObj(this);

	m_vPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_vVel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_vAccel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_vAngle = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_vScale = XMFLOAT3(1.0f, 1.0f, 1.0f);

	XMStoreFloat4x4(&m_mWorld, XMMatrixIdentity());

	m_vCenter = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_vBBox = XMFLOAT3(0.5f, 0.5f, 0.5f);
	m_fRadius = 0.5f;

	m_pBSphere = nullptr;
	m_bBSphere = false;
	m_bBColor = false;

	m_bVisible = true;
}

// �f�X�g���N�^
CGameObj::~CGameObj(void)
{
	if (m_pNext) {
		m_pNext->m_pBack = m_pBack;
	}
	if (m_pBack) {
		m_pBack->m_pNext = m_pNext;
	} else {
		m_pScene->SetObj(m_pNext);
	}
}

// ������
HRESULT CGameObj::Init()
{
	return S_OK;
}

// ���
void CGameObj::Fin()
{
	SAFE_DELETE(m_pBSphere);
}

// �X�V
void CGameObj::Update()
{
	m_vVel.x += m_vAccel.x;
	m_vVel.y += m_vAccel.y;
	m_vVel.z += m_vAccel.z;

	m_vPos.x += m_vVel.x;
	m_vPos.y += m_vVel.y;
	m_vPos.z += m_vVel.z;

	XMMATRIX mtxWorld, mtxScl, mtxRot, mtxTranslate;

	// ���[���h�}�g���b�N�X�̏�����
	mtxWorld = XMMatrixIdentity();

	// �X�P�[���𔽉f
	mtxScl = XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// ��]�𔽉f
	mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_vAngle.x),
		XMConvertToRadians(m_vAngle.y), XMConvertToRadians(m_vAngle.z));
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// �ړ��𔽉f
	mtxTranslate = XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ���[���h�}�g���b�N�X�ݒ�
	XMStoreFloat4x4(&m_mWorld, mtxWorld);
}

// �`��
void CGameObj::Draw()
{
}

// �������`��
void CGameObj::DrawAlpha()
{
	XMVECTOR vCenter = XMLoadFloat3(&m_vCenter);
	XMMATRIX mWorld = XMLoadFloat4x4(&m_mWorld);
	vCenter = XMVector3TransformCoord(vCenter, mWorld);
	mWorld = XMMatrixTranslationFromVector(vCenter);
	XMFLOAT4X4 mW;
	XMStoreFloat4x4(&mW, mWorld);
	if (m_pBSphere) {
		m_pBSphere->SetWorld(&mW);
		m_pBSphere->Draw();
	}
}

// �S������
HRESULT CGameObj::InitAll(CGameObj* pTop)
{
	HRESULT hr = S_OK;
	for (CGameObj* pObj = pTop; pObj; pObj = pObj->m_pNext) {
		hr = pObj->Init();
		if (FAILED(hr)) {
			return hr;
		}
	}
	return hr;
}

// �S�X�V
void CGameObj::UpdateAll(CGameObj*& pTop)
{
	for (CGameObj* pObj = pTop; pObj; pObj = pObj->m_pNext) {
		pObj->m_bBColor = false;
		if (pObj->m_pBSphere) {
			pObj->m_pBSphere->SetMaterial(&g_material);
		}
	}

	CGameObj* pNext;
	for (CGameObj* pObj = pTop; pObj; pObj = pNext) {
		pNext = pObj->m_pNext;
		pObj->Update();
	}

	CMeshMaterial mate = g_material;
	mate.m_Diffuse = mate.m_Ambient = g_vRed;
	for (CGameObj* pObj = pTop; pObj; pObj = pObj->m_pNext) {
		if (pObj->m_bBColor && pObj->m_pBSphere) {
			pObj->m_pBSphere->SetMaterial(&mate);
		}
	}
}

// �S�`��
void CGameObj::DrawAll(CGameObj* pTop)
{
	// �s���������`��
	for (CGameObj* pObj = pTop; pObj; pObj = pObj->m_pNext) {
		if (pObj->m_bVisible) {
			pObj->Draw();
		}
	}

	// �����������`��
	SetBlendState(BS_ALPHABLEND);
	SetZWrite(false);
	for (CGameObj* pObj = pTop; pObj; pObj = pObj->m_pNext) {
		if (pObj->m_bVisible) {
			pObj->DrawAlpha();
		}
	}
	SetZWrite(true);
	SetBlendState(BS_NONE);
}

// �S�I������
void CGameObj::FinAll(CGameObj* pTop)
{
	CGameObj* pNext;
	for (CGameObj* pObj = pTop; pObj; pObj = pNext) {
		pNext = pObj->m_pNext;
		pObj->Fin();
		delete pObj;
	}
}

bool CGameObj::CollisionBSphere(CGameObj*pOther) {
	if (pOther == this) {
		return false;
	}
	if (!m_bVisible || !pOther->m_bVisible) {
		return false;
	}
	// ���S���W�����߂�
	XMFLOAT3 vA, vB;
	XMStoreFloat3(&vA, XMVector3TransformCoord(XMLoadFloat3(&m_vCenter), XMLoadFloat4x4(&m_mWorld)));
	XMStoreFloat3(&vB, XMVector3TransformCoord(XMLoadFloat3(&pOther->m_vCenter), XMLoadFloat4x4(&pOther->m_mWorld)));

	// �Փ˔���
	float dx = vA.x - vB.x;
	float dy = vA.y - vB.y;
	float dz = vA.z - vB.z;
	float dr = m_fRadius + pOther->m_fRadius;
	return dx * dx + dy * dy + dz * dz <= dr * dr;
}

// ���E���\������
void CGameObj::SetBSphereVisible(bool bVisible) {
	m_bBSphere = bVisible;
}

// ���E���\��
void CGameObj::SetRadius(float fRadius) {
	if (m_pBSphere) {
		m_pBSphere->Fin();
		SAFE_DELETE(m_pBSphere);
	}

	m_fRadius = fRadius;

	m_pBSphere = new CSphere();
	HRESULT hr = m_pBSphere->Init(32, 16, fRadius);
	if (FAILED(hr)) {
		m_pBSphere->Fin();
		SAFE_DELETE(m_pBSphere);
		return;
	}
	m_pBSphere->SetMaterial(&g_material);
}
