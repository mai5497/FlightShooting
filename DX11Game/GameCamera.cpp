//=============================================================================
//
// ゲーム カメラ クラス [GameCamera.cpp]
// Author :	伊地田真衣
//
//=============================================================================
#include "GameCamera.h"
#include "Input.h"
#include "DebugProc.h"
#include "Model.h"

// グローバル変数
namespace {
	//const float VALUE_MOVE_CAMERA = 0.5f;			// カメラ移動量
	//const float VALUE_ROTATE_CAMERA = 0.5f;			// カメラ回転量
	const float CAM_POS_P_X = 0.0f;		// 定点カメラの視点位置（X座標）
	const float CAM_POS_P_Y = 5.0f;	// 定点カメラの視点位置（Y座標）
	const float CAM_POS_P_Z = -10.0f;	// 定点カメラの視点位置（Z座標）
	const float CAM_POS_R_X = 0.0f;	// 定点カメラの注視点位置（Z座標）
	const float CAM_POS_R_Y = 0.0f;	// 定点カメラの注視点位置（Y座標）
	const float CAM_POS_R_Z = 0.0f;	// 定点カメラの注視点位置（Z座標）

	const float FP_POS_P_X = 0.0f;	// 一人称カメラの視点位置（X座標）
	const float FP_POS_P_Y = 7.0f;	// 一人称カメラの視点位置（Y座標）
	const float FP_POS_P_Z = 10.0f;	// 一人称カメラの視点位置（Z座標）
	const float FP_POS_R_X = 0.0f;	// 一人称カメラの注視点位置（X座標）
	const float FP_POS_R_Y = 7.0f;	// 一人称カメラの注視点位置（Y座標）
	const float FP_POS_R_Z = 20.0f;	// 一人称カメラの注視点位置（Z座標）

	const float TP_POS_P_X = 0.0f;	// 三人称カメラの視点位置（X座標）
	const float TP_POS_P_Y = 5.0f;	// 三人称カメラの視点位置（Y座標）
	const float TP_POS_P_Z = -10.0f;	// 三人称カメラの視点位置（Z座標）
	const float TP_POS_R_X = 0.0f;	// 三人称カメラの注視点位置（X座標）
	const float TP_POS_R_Y = 0.0f;	// 三人称カメラの注視点位置（Y座標）
	const float TP_POS_R_Z = 5.0f;	// 三人称カメラの注視点位置（Z座標）
};


// カメラモード
enum ECamMode {
	CM_FIXED = 0,	// 固定
	CM_FP,			// 一人称
	CM_BT,			// バレットタイム風
	CM_TP,			// 三人称

	MAX_CM
};

CGameCamera::CGameCamera() {
	m_pPlayer = nullptr;
	m_fAngle = 0.0f;
	m_fRadius = 0.5f;
	m_nMode = CM_FIXED;
}


// 初期化
void CGameCamera::Init() {
	CCamera::Init();
	SetSky(CModel::GetAssimp(MODEL_SKY));
}

// 更新
void CGameCamera::Update()
{
	//// 注視点更新
	//if (m_pPlayer) {
	//	m_vTarget = m_pPlayer->GetPos();
	//}
	//CCamera::Update();

	// 視点切り替え
	//if (CInput::GetKeyRelease(VK_0) || CInput::GetKeyRelease(VK_NUMPAD0)) {
	//	m_nMode = 0;
	//}
	//if (CInput::GetKeyRelease(VK_1) || CInput::GetKeyRelease(VK_NUMPAD1)) {
	//	m_nMode = 1;
	//}
	//if (CInput::GetKeyRelease(VK_2) || CInput::GetKeyRelease(VK_NUMPAD2)) {
	//	m_nMode = 2;
	//}
	//if (CInput::GetKeyRelease(VK_3) || CInput::GetKeyRelease(VK_NUMPAD3)) {
	//	m_nMode = 3;
	//}
	m_nMode = CM_TP;

	if (m_pPlayer) {
		XMMATRIX mW;
		switch (m_nMode) {
		case CM_FIXED:	// 定点カメラ
			m_vTarget = m_pPlayer->GetPos();
			m_vPos = XMFLOAT3(CAM_POS_P_X, CAM_POS_P_Y, CAM_POS_P_Z);
			m_vUp = XMFLOAT3(0.0f, 1.0f, 0.0f);
			break;
		case CM_FP:	// 一人称カメラ
			mW = XMLoadFloat4x4(&m_pPlayer->GetWorld());
			XMStoreFloat3(&m_vUp, XMVector3TransformNormal(XMVectorSet(0, 1, 0, 0), mW));
			XMStoreFloat3(&m_vTarget, XMVector3TransformCoord(XMVectorSet(FP_POS_R_X, FP_POS_R_Y, FP_POS_R_Z, 1.0f), mW));
			XMStoreFloat3(&m_vPos, XMVector3TransformCoord(XMVectorSet(FP_POS_P_X, FP_POS_P_Y, FP_POS_P_Z, 1.0f), mW));
			break;
		case CM_BT:	//	バレットタイム風カメラ
			m_fAngle += 1.0f;
			if (m_fAngle >= 180.0f) {
				m_fAngle -= 360;
			}
			m_vTarget = m_pPlayer->GetPos();
			m_vUp = XMFLOAT3(0.0f, 1.0f, 0.0f);
			m_vPos.x = m_vTarget.x + sinf(XMConvertToRadians(m_fAngle)) * (m_fRadius * 2.5f);
			m_vPos.y = m_vTarget.y;
			m_vPos.z = m_vTarget.z + cosf(XMConvertToRadians(m_fAngle)) * (m_fRadius * 2.5f);
			break;
		case CM_TP:	// 三人称カメラ
			mW = XMLoadFloat4x4(&m_pPlayer->GetWorld());
			XMStoreFloat3(&m_vUp, XMVector3TransformNormal(XMVectorSet(0, 1, 0, 0), mW));
			XMStoreFloat3(&m_vTarget, XMVector3TransformCoord(XMVectorSet(TP_POS_R_X, TP_POS_R_Y, TP_POS_R_Z, 1.0f), mW));
			XMStoreFloat3(&m_vPos, XMVector3TransformCoord(XMVectorSet(TP_POS_P_X, TP_POS_P_Y, TP_POS_P_Z, 1.0f), mW));
			break;
		}
	} else {
		m_vPos = XMFLOAT3(CAM_POS_P_X, CAM_POS_P_Y, CAM_POS_P_Z);
		m_vTarget = XMFLOAT3(CAM_POS_R_X, CAM_POS_R_Y, CAM_POS_R_Z);
		m_vUp = XMFLOAT3(0.0f, 1.0f, 0.0f);
	}

	CCamera::Update();

	//　デバッグ表示
	CDebugProc::Print("\x84\xCE*** ｶﾒﾗ ***\n");
	//CDebugProc::Print("[1] : FP View\n");
	//CDebugProc::Print("[2] : Bullet-time style\n");
	//CDebugProc::Print("[3] : TP View\n");
	//CDebugProc::Print("[0] : Fixed point\x84\xCF\n");
	CDebugProc::Print("[ｶﾒﾗｲﾁ : (%f,%f,%f)]\n",m_vPos.x,m_vPos.y,m_vPos.z);
	CDebugProc::Print("[ﾁｭｳｼﾃﾝ : (%f,%f,%f)]\n",m_vTarget.x, m_vTarget.y, m_vTarget.z);
	CDebugProc::Print("\n");
}

//自機設定
void CGameCamera::SetPlayer(CGameObj*pPlayer) {
	m_pPlayer = pPlayer;
	if (m_pPlayer) {
		m_fRadius = m_pPlayer->GetRadius();
	} else {
		m_fRadius = 0.5f;
	}
}