//=============================================================================
//
// プレイヤー クラス定義 [Player.h]
// Author : 伊地田真衣
//
//=============================================================================
#pragma once
#include "Model.h"
class CPlayer : public CModel
{
public:
	CPlayer(CScene* pScene);
	virtual ~CPlayer();

	virtual HRESULT Init();
	virtual void Fin();
	virtual void Update();
	virtual void Draw();
private:
	UINT m_uTick;
	UINT m_uBullet;
	UINT m_uMissile;
};
