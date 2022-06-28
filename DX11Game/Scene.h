//=============================================================================
//
// シーン クラス定義 [Scene.h]
// Author : 伊地田真衣
//
//=============================================================================
#pragma once
#include "main.h"

// 定数定義
enum EScene {
	SCENE_NONE = 0,		// (シーン無)
	SCENE_TITLE,		// タイトル画面
	SCENE_GAME,			// ゲーム画面

	MAX_SCENE
};

// クラス定義
class CGameObj;
class CScene {
protected:
	EScene m_id;
	CGameObj* m_pObj;

private:
	static CScene* m_pScene;
	static CScene* m_pTop;
	CScene* m_pBack;
	CScene* m_pNext;

public:
	CScene();
	virtual ~CScene();

	virtual bool Init();
	virtual void Fin();
	virtual void Update() = 0;
	virtual void Draw() = 0;

	static bool InitAll();
	static void FinAll();
	static void UpdateAll();
	static void DrawAll();

	static void Change(EScene scene);

	CGameObj* GetObj() { return m_pObj; }
	void SetObj(CGameObj* pObj) { m_pObj = pObj; }
	CGameObj* Find(int id, CGameObj* pStart = nullptr);
};
