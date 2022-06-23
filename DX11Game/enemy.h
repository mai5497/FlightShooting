//=============================================================================
//
// 敵 クラス定義 [Enemy.h]
// Author : 伊地田真衣
//
//=============================================================================
#pragma once
#include "Model.h"
class CPlayer;
class CEnemy : public CModel
{
private:
	CPlayer *m_pPlayer;
public:
	CEnemy(CScene* pScene);
	virtual ~CEnemy();

	virtual HRESULT Init();
	virtual void Fin();
	virtual void Update();
	virtual void Draw();
	void SetPlayer(CPlayer*pPlayer) {
		m_pPlayer = pPlayer;
	}
};
