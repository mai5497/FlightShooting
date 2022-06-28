//=============================================================================
//
// タイトル画面クラス定義 [Title.h]
// Author : 伊地田真衣
//
//=============================================================================
#pragma once
#include "Scene.h"

// クラス定義
class CTitle : public CScene
{
private:
	bool m_bStart;
	ID3D11ShaderResourceView* m_pTexTitle;
	ID3D11ShaderResourceView* m_pTexStart;
	ID3D11ShaderResourceView* m_pTexBG;
	int m_nTimer;

public:
	CTitle();
	virtual ~CTitle();

	virtual bool Init();
	virtual void Fin();
	virtual void Update();
	virtual void Draw();
};
