// レーダー [Radar.h]
#pragma once
#include "main.h"
#include "Game.h"

enum {
	RT_RADAR,		// レーダー
	RT_BRIGHTSPOT,	// 輝点

	MAX_RT
};

class CRadar
{
public:
	void Init(CGame* pScene);
	void Update();
	void Draw();

	static HRESULT LoadTexture();
	static void ReleaseTexture();
private:
	static ID3D11ShaderResourceView* m_pTexture[MAX_RT];

	bool m_bVisible;
	XMFLOAT3 m_vPos[MAX_ENEMY];
	XMFLOAT3 m_vPos2D[MAX_ENEMY];
	CGame* m_pScene;
	int m_nEnemy;
	CPlayer* m_pPlayer;
};
