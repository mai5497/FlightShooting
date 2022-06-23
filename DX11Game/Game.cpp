//=============================================================================
//
// �Q�[�� �N���X���� [Game.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "Game.h"
#include "Input.h"
#include "GameObj.h"
#include "Sound.h"
#include "Fade.h"
#include "DebugProc.h"
#include "enemy.h"

// �R���X�g���N�^
CGame::CGame() : CScene()
{
	m_id = SCENE_GAME;

	m_pPlayer = nullptr;

	//m_pExp = nullptr;
	//m_uTimer = 0;
	for (int i = 0; i < _countof(m_pEnemy); ++i) {
		m_pEnemy[i] = nullptr;
	}
	m_nBoundary = 0;
}

// �f�X�g���N�^
CGame::~CGame()
{
}

// ������
bool CGame::Init()
{
	m_bResult = false;

	m_camera.Init();
	CCamera::Set(&m_camera);

	CLight* pLight = CLight::Get();
	if (pLight) {
		pLight->Init();
	}

	m_pPlayer = new CPlayer(this);
	for (int i = 0; i < _countof(m_pEnemy); ++i) {
		//CEnemy* pEnemy = new CEnemy(this);
		m_pEnemy[i] = new CEnemy(this);
		m_pEnemy[i]->SetPlayer(m_pPlayer);
		XMFLOAT4X4 mW;
		XMStoreFloat4x4(&mW,
			XMMatrixRotationRollPitchYaw(XMConvertToRadians(rand() % 360), XMConvertToRadians(rand() % 360), XMConvertToRadians(rand() % 360)));
		mW._41 = (float)(rand() % 100 - 50);
		mW._43 = (float)(rand() % 100 - 50);
		m_pEnemy[i]->SetWorld(mW);
	}
	//m_camera.SetPlayer(m_pPlayer);
	//m_pExp = new CExplosion(this);
	//m_pExp->SetModel(CModel::GetAssimp(MODEL_PLAYER));

	UpdateBoundary();

	// �S�I�u�W�F�N�g������
	//if (!CGameObj::InitAll(m_pObj)) {
	if (FAILED(CGameObj::InitAll(m_pObj))) {
		return false;
	}

	m_camera.SetPlayer(m_pPlayer);

	// BGM�Đ��J�n
	CSound::Play(BGM_GAME);

	return true;
}

// �I������
void CGame::Fin()
{
	// BGM�Đ���~
	CSound::Stop(BGM_GAME);

	// �S�I�u�W�F�N�g�I������
	CGameObj::FinAll(m_pObj);
}

// �X�V
void CGame::Update()
{
	// ���E�\���ؑ�
	if (CInput::GetKeyTrigger(VK_B)) {
		m_nBoundary = (m_nBoundary + 1) % 2;
		UpdateBoundary();
	}

	// �S�L�����X�V
	CGameObj::UpdateAll(m_pObj);

	//// ������
	//if (m_uTimer) {
	//	--m_uTimer;
	//	// �����I��
	//	if (!m_uTimer) {
	//		m_pPlayer->SetVisible();
	//	}
	//}
	//// �����J�n
	//if (!m_uTimer && CInput::GetKeyTrigger(VK_X)) {
	//	m_uTimer = 180;
	//	m_pExp->Start(&m_pPlayer->GetWorld());
	//	m_pPlayer->SetVisible(false);
	//}

	if (!m_bResult) {
		if (CInput::GetKeyRelease(VK_BACK) ||
			CInput::GetKeyRelease(VK_DELETE)) {
			m_bResult = true;
			CFade::Out(SCENE_TITLE);
		}
	}

	static LPCSTR boundary[] = {"�ˮ���","���"};
	CDebugProc::Print("\nx84\xCE[B]:�����\x84\xCF\n",boundary[m_nBoundary]);
	CDebugProc::Print("\n");
}

void CGame::UpdateBoundary() {
	for (CGameObj* pObj = GetObj(); pObj; pObj = pObj->GetNext()) {
	
		switch (pObj->GetID()) {
		case GOT_PLAYER:
		case GOT_ENEMY:
		case GOT_BULLET:
		case GOT_MISSILE:
			switch (m_nBoundary) {
			case 0:	// ���E�\���Ȃ�
				m_pPlayer->SetBSphereVisible(false);
				for (int i = 0; i < _countof(m_pEnemy); ++i) {
					m_pEnemy[i]->SetBSphereVisible(false);
				}
				break;
			case 1:	// ���E��
				m_pPlayer->SetBSphereVisible();
				for (int i = 0; i < _countof(m_pEnemy); ++i) {
					m_pEnemy[i]->SetBSphereVisible();
				}
				break;
			}
		}
	}
}

// �`��
void CGame::Draw()
{
	// �S�L�����`��
	CGameObj::DrawAll(m_pObj);
}
