//=============================================================================
//
// �f�o�b�O�\���N���X��` [DebugProc.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once
#include "main.h"

class CDebugProc
{
private:
	static ID3D11ShaderResourceView* m_pTexture;	// �e�N�X�`���ւ̃|�C���^
	static char m_szDebug[8192];					// �f�o�b�O���
	static bool m_bHiragana;						// �������t���O

public:
	static HRESULT Init();
	static void Fin();
	static void Draw();

	static void Start(bool hiragana = false);
	static int Print(const char *fmt, ...);
	static bool SetHiragana(bool hiragana = true);
};
