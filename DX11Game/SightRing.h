// è∆èÄä¬ [SightRing.h]
#pragma once
#include "main.h"

enum {
	SRT_SIGHTRING,	// è∆èÄä¬
	SRT_FONT,		// ÉtÉHÉìÉg

	MAX_SRT
};

class CSightRing
{
private:
	static ID3D11ShaderResourceView* m_pTexture[MAX_SRT];

	bool m_bVisible;
	bool m_bHide;
	XMFLOAT3 m_vWorldPos;
	float m_fRadius;
	XMFLOAT2 m_vPos;
	XMFLOAT2 m_vSize;
	XMFLOAT4 m_vColor;
	XMFLOAT2 m_vLoc;
	char m_szStat[1024];

public:
	void Update();
	void Draw();

	static HRESULT LoadTexture();
	static void ReleaseTexture();

	static CSightRing* Create(XMFLOAT3 pos, float fRadius);
	void Move(XMFLOAT3 pos);
	void SetColor(XMFLOAT4 color);
	void Release();
	void SetVisible(bool bVisible = true) { m_bVisible = bVisible; }

private:
	void DrawString(ID3D11DeviceContext* pDC);
};
