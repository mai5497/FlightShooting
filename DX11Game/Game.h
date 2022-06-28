//=============================================================================
//
// ゲーム クラス定義 [Game.h]
// Author : 伊地田真衣
//
//=============================================================================
#pragma once
#include "Scene.h"
#include "Player.h"
#include "GameCamera.h"
#include "Explosion.h"
#include "SightRing.h"
//#include "Radar.h"

// 定数定義
#define MAX_ENEMY (20)

// 前方定義
class CEnemy;
class CRadar;

// クラス定義
class CGame : public CScene
{
private:
	void UpdateBoundary();

	bool m_bResult;

	CGameCamera m_camera;
	CPlayer* m_pPlayer;
	CEnemy *m_pEnemy[MAX_ENEMY];
	CSightRing* m_pSightRing[MAX_ENEMY];
	CRadar* m_pRader;
	int m_nBoundary;
	//CExplosion *m_pExp;
	//UINT m_uTimer;

public:
	CGame();
	virtual ~CGame();

	virtual bool Init();
	virtual void Fin();
	virtual void Update();
	virtual void Draw();
	int GetBoundary() {
		return m_nBoundary;
	}
	CPlayer* GetPlayer() { return m_pPlayer; }
};
