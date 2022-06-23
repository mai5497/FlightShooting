//=============================================================================
//
// �f�o�b�O�\���N���X���� [DebugProc.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include <stdarg.h>
#include <string.h>
#include "DebugProc.h"
#include "Polygon.h"
#include "Texture.h"

// �}�N����`
#define	TEXTURE_FILENAME	L"data/texture/PressStart2Ph_e.png"
#define FONT_WIDTH			16
#define FONT_HEIGHT			16

// �ÓI�����o
ID3D11ShaderResourceView* CDebugProc::m_pTexture;	// �e�N�X�`���ւ̃|�C���^
char CDebugProc::m_szDebug[8192];					// �f�o�b�O���
bool CDebugProc::m_bHiragana = false;				// �������t���O

// ������
HRESULT CDebugProc::Init()
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr = S_OK;

	// �e�N�X�`���̓ǂݍ���
	hr = CreateTextureFromFile(pDevice, TEXTURE_FILENAME, &m_pTexture);

	return hr;
}

// �I������
void CDebugProc::Fin()
{
	// �e�N�X�`����@
	SAFE_RELEASE(m_pTexture);
}

// �`��
void CDebugProc::Draw()
{
	SetZBuffer(false);
	SetBlendState(BS_ALPHABLEND);
	CPolygon::SetColor(1.0f, 1.0f, 1.0f, 1.0f);

	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	XMFLOAT2 vPos(SCREEN_WIDTH * -0.5f + FONT_WIDTH * 0.5f,
		SCREEN_HEIGHT * 0.5f - FONT_HEIGHT * 0.5f);
	CPolygon::SetTexture(m_pTexture);
	CPolygon::SetFrameSize(8.0f / 128.0f, 8.0f / 128.0f);
	CPolygon::SetSize(FONT_WIDTH, FONT_HEIGHT);
	for (char* pChr = &m_szDebug[0]; *pChr; ++pChr) {
		if (*pChr == '\n') {
			vPos.x = SCREEN_WIDTH * -0.5f + FONT_WIDTH * 0.5f;
			vPos.y -= FONT_HEIGHT;
			continue;
		}
		int nChr = (BYTE)*pChr;
		if (nChr == 0x0084) {
			nChr = (BYTE)*(pChr + 1);
			if (nChr >= 0x00c0 && nChr <= 0x00cf) {
				const XMFLOAT4 vColor[] = {
					XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f),
					XMFLOAT4(0.0f, 0.0f, 0.5f, 1.0f),
					XMFLOAT4(0.0f, 0.5f, 0.0f, 1.0f),
					XMFLOAT4(0.0f, 0.5f, 0.5f, 1.0f),
					XMFLOAT4(0.5f, 0.0f, 0.0f, 1.0f),
					XMFLOAT4(0.5f, 0.0f, 0.5f, 1.0f),
					XMFLOAT4(0.5f, 0.5f, 0.0f, 1.0f),
					XMFLOAT4(0.752941f, 0.752941f, 0.752941f, 1.0f),
					XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f),
					XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
					XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
					XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f),
					XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
					XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f),
					XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f),
					XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
				};
				CPolygon::SetColor(vColor[nChr & 0x0f]);
				++pChr;
				continue;
			}
		}
		CPolygon::SetPos(vPos.x, vPos.y);
		if (m_bHiragana &&
			(nChr >= 0xa6 && nChr <= 0xaf || nChr >= 0xb1 && nChr <= 0xdd))
			nChr ^= 0x20;
		CPolygon::SetUV((nChr & 15) / 16.0f, (nChr >> 4) / 16.0f);
		CPolygon::Draw(pDeviceContext);
		vPos.x += FONT_WIDTH;
	}
	// �e�N�X�`���ݒ�����ɖ߂�
	CPolygon::SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	CPolygon::SetUV(0.0f, 0.0f);
	CPolygon::SetFrameSize(1.0f, 1.0f);

	SetBlendState(BS_NONE);
	SetZBuffer(true);
}

// �o�^�J�n
void CDebugProc::Start(bool hiragana)
{
	SetHiragana(hiragana);
	m_szDebug[0] = '\0';
}

// �o�^
int CDebugProc::Print(const char *fmt, ...)
{
	va_list list;
	va_start(list, fmt);
	char aBuf[256];
	int nLen = _vsprintf_p(aBuf, sizeof(aBuf), fmt, list);
	va_end(list);

	// �A��
	if ((strlen(m_szDebug) + strlen(aBuf)) < sizeof(m_szDebug) - 1) {
		strcat_s(m_szDebug, sizeof(m_szDebug), aBuf);
	}

	return nLen;
}

// �������ݒ�
bool CDebugProc::SetHiragana(bool hiragana)
{
	bool prev = m_bHiragana;
	m_bHiragana = hiragana;
	return prev;
}
