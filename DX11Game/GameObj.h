//=============================================================================
//
// オブジェクト クラス定義 [GameObj.h]
// Author : 伊地田真衣
//
//=============================================================================
#pragma once
#include "main.h"
#include "Sphere.h"


// 定数定義
enum EGameObjType {		// オブジェクト型
	GOT_GAMEOBJ = 0,	// 型無し
	GOT_PLAYER,			// プレイヤー
	GOT_ENEMY,			// 敵
	GOT_EXPLOSION,			// 爆発
	GOT_BULLET,			// 弾
	GOT_MISSILE,		// 追尾ミサイル

	MAX_GAMEOBJTYPE
};

// クラス定義
class CScene;
class CGameObj
{
private:
	CGameObj* m_pBack;		// 前のオブジェクト
	CGameObj* m_pNext;		// 次のオブジェクト

protected:
	EGameObjType m_id;		// オブジェクト型

	CScene* m_pScene;		// 所属シーン

	XMFLOAT3 m_vPos;		// 座標
	XMFLOAT3 m_vVel;		// 速度
	XMFLOAT3 m_vAccel;		// 加速度
	XMFLOAT3 m_vAngle;		// 角度
	XMFLOAT3 m_vScale;		// 拡縮

	XMFLOAT4X4 m_mWorld;	// ワールド変換

	bool m_bVisible;		// 表示フラグ

	XMFLOAT3 m_vCenter;		// 境界ボックス(/球)中心座標
	XMFLOAT3 m_vBBox;		// 境界ボックス
	float m_fRadius;		// 境界球半径

	CSphere* m_pBSphere;	// 境界旧表示メッシュ
	bool m_bBSphere;		// 境界旧表示フラグ
	bool m_bBColor;			// 境界表示色

public:
	CGameObj(CScene* pScene);
	virtual ~CGameObj(void);

	virtual HRESULT Init();		// 初期化
	virtual void Update();		// 更新
	virtual void Draw();		// 不透明部分描画
	virtual void DrawAlpha();	// 半透明部分描画
	virtual void Fin();			// 終了処理

	static HRESULT InitAll(CGameObj* pTop);
	static void UpdateAll(CGameObj*& pTop);
	static void DrawAll(CGameObj* pTop);
	static void FinAll(CGameObj* pTop);

	void SetPos(XMFLOAT3 vPos) { m_vPos = vPos; }
	XMFLOAT3& GetPos() { return m_vPos; }
	void SetAngle(XMFLOAT3 vAngle) { m_vAngle = vAngle; }
	XMFLOAT3& GetAngle() { return m_vAngle; }
	void SetScale(XMFLOAT3 vScale) { m_vScale = vScale; }
	XMFLOAT3& GetScale() { return m_vScale; }
	void SetWorld(XMFLOAT4X4& mWorld) { m_mWorld = mWorld; }
	XMFLOAT4X4& GetWorld() { return m_mWorld; }
	XMFLOAT3& GetVel() {return m_vVel;}


	EGameObjType GetID() { return m_id; }
	CGameObj* GetNext() { return m_pNext; }

	void SetVisible(bool bVisible = true) { m_bVisible = bVisible; }
	virtual bool IsVisible() {return m_bVisible;}

	void SetCenter(XMFLOAT3 vCenter) { m_vCenter = vCenter; }
	XMFLOAT3& GetCenter() { return m_vCenter; }
	void SetBBox(XMFLOAT3 vBBox) { m_vBBox = vBBox; }
	XMFLOAT3& GetBBox() { return m_vBBox; }
	void SetRadius(float fRadius);
	float GetRadius() { return m_fRadius; }

	virtual bool CollisionBSphere(CGameObj* pObj);
	void SetBSphereVisible(bool bVisible = true);
	void SetBColor(bool bHit = true) {m_bBColor = bHit;	}
};
