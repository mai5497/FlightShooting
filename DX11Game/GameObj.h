//=============================================================================
//
// �I�u�W�F�N�g �N���X��` [GameObj.h]
// Author : �ɒn�c�^��
//
//=============================================================================
#pragma once
#include "main.h"
#include "Sphere.h"


// �萔��`
enum EGameObjType {		// �I�u�W�F�N�g�^
	GOT_GAMEOBJ = 0,	// �^����
	GOT_PLAYER,			// �v���C���[
	GOT_ENEMY,			// �G
	GOT_EXPLOSION,			// ����
	GOT_BULLET,			// �e
	GOT_MISSILE,		// �ǔ��~�T�C��

	MAX_GAMEOBJTYPE
};

// �N���X��`
class CScene;
class CGameObj
{
private:
	CGameObj* m_pBack;		// �O�̃I�u�W�F�N�g
	CGameObj* m_pNext;		// ���̃I�u�W�F�N�g

protected:
	EGameObjType m_id;		// �I�u�W�F�N�g�^

	CScene* m_pScene;		// �����V�[��

	XMFLOAT3 m_vPos;		// ���W
	XMFLOAT3 m_vVel;		// ���x
	XMFLOAT3 m_vAccel;		// �����x
	XMFLOAT3 m_vAngle;		// �p�x
	XMFLOAT3 m_vScale;		// �g�k

	XMFLOAT4X4 m_mWorld;	// ���[���h�ϊ�

	bool m_bVisible;		// �\���t���O

	XMFLOAT3 m_vCenter;		// ���E�{�b�N�X(/��)���S���W
	XMFLOAT3 m_vBBox;		// ���E�{�b�N�X
	float m_fRadius;		// ���E�����a

	CSphere* m_pBSphere;	// ���E���\�����b�V��
	bool m_bBSphere;		// ���E���\���t���O
	bool m_bBColor;			// ���E�\���F

public:
	CGameObj(CScene* pScene);
	virtual ~CGameObj(void);

	virtual HRESULT Init();		// ������
	virtual void Update();		// �X�V
	virtual void Draw();		// �s���������`��
	virtual void DrawAlpha();	// �����������`��
	virtual void Fin();			// �I������

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
