// �Ə��� [SightRing.cpp]
#include <stdio.h>
#include "SightRing.h"
#include "Texture.h"
#include "Polygon.h"
#include "Camera.h"

// �O���[�o���ϐ�
namespace {
	LPCTSTR PATH_TEXTURE[MAX_SRT] = {
		_T("data/texture/sight_ring.png"),
		_T("data/texture/PressStart2Ph.png"),
	};
	float FONT_WIDTH = 16.0f;
	float FONT_HEIGHT = 16.0f;
}

// �ÓI�����o�ϐ�
ID3D11ShaderResourceView* CSightRing::m_pTexture[MAX_SRT];

// �X�V
void CSightRing::Update()
{
	if (!m_bVisible) return;

	CCamera* pCamera = CCamera::Get();
	XMFLOAT4X4& mV = pCamera->GetViewMatrix();
	XMVECTOR vZ = XMVectorSet(mV._13, mV._23, mV._33, 0.0f);
	XMFLOAT3& vCam = pCamera->GetPos();
	XMVECTOR vT = XMVectorSet(m_vWorldPos.x - vCam.x,
		m_vWorldPos.y - vCam.y, m_vWorldPos.z - vCam.z, 0.0f);
	float fDot;
	XMStoreFloat(&fDot, XMVector3Dot(vZ, vT));
	if (fDot <= 0.0f) {
		m_bHide = true;
		return;
	}
	float fDist;
	XMStoreFloat(&fDist, XMVector3Length(vT));
	sprintf_s(m_szStat, sizeof(m_szStat), "%.1f", fDist);
	m_bHide = false;
	XMFLOAT4X4& mP = pCamera->GetProjMatrix();
	// �r���{�[�h�p���[���h�ϊ�
	XMFLOAT4X4 mW(mV._11, mV._21, mV._31, 0.0f,
		mV._12, mV._22, mV._32, 0.0f,
		mV._13, mV._23, mV._33, 0.0f,
		m_vWorldPos.x, m_vWorldPos.y, m_vWorldPos.z, 1.0f);
	// �r���[�|�[�g�ϊ�
	XMFLOAT4X4 mVP(SCREEN_WIDTH / 2.0f, 0.0f, 0.0f, 0.0f,
		0.0f, SCREEN_HEIGHT / 2.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	// �}�g���b�N�X������
	XMMATRIX matrix = XMLoadFloat4x4(&mW) * XMLoadFloat4x4(&mV)
		* XMLoadFloat4x4(&mP) * XMLoadFloat4x4(&mVP);
	// ���S���W�A�������W�A�E����W��2D���W�ɕϊ�
	static const XMFLOAT3 v[] = {
		XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT3(-m_fRadius, -m_fRadius, 0.0f),
		XMFLOAT3(m_fRadius, m_fRadius, 0.0f),
	};
	XMFLOAT4 vPos[3];
	for (int i = 0; i < 3; ++i) {
		XMStoreFloat4(&vPos[i],
			XMVector3Transform(XMLoadFloat3(&v[i]), matrix));
		// �������_��񓯎����_�ɕϊ�
		vPos[i].x /= vPos[i].w;
		vPos[i].y /= vPos[i].w;
		vPos[i].z /= vPos[i].w;
	}
	// �Ə����S
	m_vPos.x = vPos[0].x;
	m_vPos.y = vPos[0].y;
	// �Ə��T�C�Y
	m_vSize.x = vPos[2].x - vPos[1].x;
	m_vSize.y = vPos[2].y - vPos[1].y;
	// �Ə������\���ʒu(����)
	m_vLoc.x = vPos[1].x;
	m_vLoc.y = vPos[2].y;
}

// �`��
void CSightRing::DrawString(ID3D11DeviceContext* pDC)
{
	XMFLOAT2 vPos(m_vLoc.x + FONT_WIDTH * 0.5f,
		m_vLoc.y + FONT_HEIGHT * 0.5f);
	CPolygon::SetTexture(m_pTexture[SRT_FONT]);
	CPolygon::SetFrameSize(8.0f / 128.0f, 8.0f / 128.0f);
	CPolygon::SetSize(FONT_WIDTH, FONT_HEIGHT);
	for (const char* pChr = m_szStat; *pChr; ++pChr) {
		if (*pChr == '\n') {
			vPos.x = m_vLoc.x + FONT_WIDTH * 0.5f;
			vPos.y += FONT_HEIGHT;
			continue;
		}
		int nChr = (BYTE)*pChr;
		CPolygon::SetPos(vPos.x, vPos.y);
		CPolygon::SetUV((nChr & 15) / 16.0f, (nChr >> 4) / 16.0f);
		CPolygon::Draw(pDC);
		vPos.x += FONT_WIDTH;
	}
}

void CSightRing::Draw()
{
	if (!m_bVisible || m_bHide) return;

	ID3D11DeviceContext* pDC = GetDeviceContext();

	SetZBuffer(false);
	SetBlendState(BS_ALPHABLEND);
	CPolygon::SetColor(m_vColor);

	DrawString(pDC);

	CPolygon::SetTexture(m_pTexture[SRT_SIGHTRING]);
	CPolygon::SetPos(m_vPos.x, m_vPos.y);
	CPolygon::SetSize(m_vSize.x, m_vSize.y);
	CPolygon::SetUV(0.0f, 0.0f);
	CPolygon::SetFrameSize(1.0f, 1.0f);
	CPolygon::Draw(pDC);

	CPolygon::SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	SetBlendState(BS_NONE);
	SetZBuffer(true);
}

// ����
CSightRing* CSightRing::Create(XMFLOAT3 pos, float fRadius)
{
	CSightRing* pSightRing = new CSightRing();
	pSightRing->m_bVisible = false;
	pSightRing->m_bHide = false;
	pSightRing->m_vColor = XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);
	pSightRing->m_vPos = XMFLOAT2(0.0f, 0.0f);
	pSightRing->m_vSize = XMFLOAT2(0.0f, 0.0f);
	pSightRing->m_vWorldPos = pos;
	pSightRing->m_fRadius = fRadius;
	pSightRing->m_szStat[0] = '\0';
	pSightRing->Update();
	return pSightRing;
}

// �ړ�
void CSightRing::Move(XMFLOAT3 pos)
{
	m_vWorldPos = pos;
}

// �F�ݒ�
void CSightRing::SetColor(XMFLOAT4 color)
{
	m_vColor = color;
}

// ���
void CSightRing::Release()
{
	delete this;
}

// �e�N�X�`���Ǎ�
HRESULT CSightRing::LoadTexture()
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr = S_OK;

	ReleaseTexture();
	// �e�N�X�`���̓ǂݍ���
	for (int i = 0; i < MAX_SRT; ++i) {
		hr = CreateTextureFromFile(pDevice, PATH_TEXTURE[i], &m_pTexture[i]);
		if (FAILED(hr)) {
			break;
		}
	}
	return hr;
}

// �e�N�X�`�����
void CSightRing::ReleaseTexture()
{
	for (int i = 0; i < MAX_SRT; ++i) {
		SAFE_RELEASE(m_pTexture[i]);
	}
}
