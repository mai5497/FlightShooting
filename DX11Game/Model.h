//=============================================================================
//
// モデル クラス定義 [Model.h]
// Author : 伊地田真衣
//
//=============================================================================
#pragma once
#include "GameObj.h"
#include "AssimpModel.h"

enum EModel {
	MODEL_PLAYER = 0,	// プレイヤー モデル
	MODEL_ENEMY,		// 敵モデル
	MODEL_SKY,			// スカイドーム
	MODEL_BULLET,		// 弾も出る
	MODEL_MISSILE,		// ミサイルモデル

	MAX_MODEL
};

class CModel : public CGameObj
{
private:
	static CAssimpModel* m_pModels[MAX_MODEL];
	CAssimpModel* m_pModel;

public:
	CModel(CScene* pScene);
	virtual ~CModel();

	static HRESULT LoadAll();
	static void ReleaseAll();

	virtual HRESULT Init();
	virtual void Fin();
	virtual void Update();
	virtual void Draw();
	virtual void DrawAlpha();

	void SetModel(EModel model);
	static CAssimpModel* GetAssimp(EModel model);
};
