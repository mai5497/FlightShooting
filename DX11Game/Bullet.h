//=============================================================================
//
// �e�N���X��` [Bullet.h]
// Author : �ɒn�c�^��
//
//=============================================================================
#pragma once
#include "Model.h"

class CBullet : public CModel
{
private:
	int m_nLife;

public:
	CBullet(CScene* pScene);
	virtual ~CBullet();

	virtual HRESULT Init();
	virtual void Fin();
	virtual void Update();

	static CBullet* Fire(CScene* pScene, XMFLOAT4X4& mtxWorld);
};
