//=============================================================================
//
// �Q�[�� �J���� �N���X [GameCamera.h]
// Author : �ɒn�c�^��
//
//=============================================================================
#pragma once
#include "Camera.h"
#include "GameObj.h"

class CGameCamera : public CCamera
{
private:
	CGameObj *m_pPlayer;
	float m_fRadius;	// ���@���a
	int m_nMode;	// �J�������[�h
	float m_fAngle;	// �J�����A���O��
public:
	CGameCamera();
	virtual void Init();
	virtual void Update();
	void SetPlayer(CGameObj*pPlayer);
};

