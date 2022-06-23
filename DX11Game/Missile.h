//********************************************************************************
//
// 追尾ミサイルクラス定義
// 編集者：伊地田真衣
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
