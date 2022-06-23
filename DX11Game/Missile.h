//********************************************************************************
//
// �ǔ��~�T�C���N���X��`
// �ҏW�ҁF�ɒn�c�^��
//
//********************************************************************************
#ifndef __MISSILE_H__
#define __MISSILE_H__

#include "Model.h"

class CMissile :public CModel {
private:
	int m_nLife;
	CGameObj* m_pTarget;

public:
	CMissile(CScene* pScene);
	virtual ~CMissile();

	virtual HRESULT Init();
	virtual void Update();

	static CMissile* Fire(CScene* pScene, XMFLOAT4X4& mtxWorld, CGameObj* pTarget = nullptr);
};

#endif
