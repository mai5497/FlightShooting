//=============================================================================
//
// 弾クラス定義 [Bullet.h]
// Author : 伊地田真衣
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
