//=============================================================================
//
// ゲーム クラス実装 [Game.cpp]
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

// コンストラクタ
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

// デストラクタ
CGame::~CGame()
{
}

// 初期化
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

	// 全オブジェクト初期化
	//if (!CGameObj::InitAll(m_pObj)) {
	if (FAILED(CGameObj::InitAll(m_pObj))) {
		return false;
	}

	m_camera.SetPlayer(m_pPlayer);

	// BGM再生開始
	CSound::Play(BGM_GAME);

	return true;
}

// 終了処理
void CGame::Fin()
{
	// BGM再生停止
	CSound::Stop(BGM_GAME);

	// 全オブジェクト終了処理
	CGameObj::FinAll(m_pObj);
}

// 更新
void CGame::Update()
{
	// 境界表示切替
	if (CInput::GetKeyTrigger(VK_B)) {
		m_nBoundary = (m_nBoundary + 1) % 2;
		UpdateBoundary();
	}

	// 全キャラ更新
	CGameObj::UpdateAll(m_pObj);

	//// 爆発中
	//if (m_uTimer) {
	//	--m_uTimer;
	//	// 爆発終了
	//	if (!m_uTimer) {
	//		m_pPlayer->SetVisible();
	//	}
	//}
	//// 爆発開始
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

	static LPCSTR boundary[] = {"ﾋﾋｮｳｼﾞ","ｷｭｳ"};
	CDebugProc::Print("\nx84\xCE[B]:ｷｮｳｶｲ\x84\xCF\n",boundary[m_nBoundary]);
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
			case 0:	// 境界表示なし
				m_pPlayer->SetBSphereVisible(false);
				for (int i = 0; i < _countof(m_pEnemy); ++i) {
					m_pEnemy[i]->SetBSphereVisible(false);
				}
				break;
			case 1:	// 境界球
				m_pPlayer->SetBSphereVisible();
				for (int i = 0; i < _countof(m_pEnemy); ++i) {
					m_pEnemy[i]->SetBSphereVisible();
				}
				break;
			}
		}
	}
}

// 描画
void CGame::Draw()
{
	// 全キャラ描画
	CGameObj::DrawAll(m_pObj);
}
