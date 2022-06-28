//=============================================================================
//
// ゲーム カメラ クラス [GameCamera.h]
// Author : 伊地田真衣
//
//=============================================================================
#pragma once
#include "Camera.h"
#include "GameObj.h"

class CGameCamera : public CCamera
{
private:
	CGameObj *m_pPlayer;
	float m_fRadius;	// 自機半径
	int m_nMode;	// カメラモード
	float m_fAngle;	// カメラアングル
public:
	CGameCamera();
	virtual void Init();
	virtual void Update();
	void SetPlayer(CGameObj*pPlayer);
};

