//=============================================================================
//
// モデル クラス実装 [Model.cpp]
// Author : 伊地田真衣
//
//=============================================================================
#include "Model.h"
#include "Scene.h"

// グローバル変数
namespace {
	//const float VALUE_MOVE_MODEL = 1.0f;
	//const float VALUE_ROTATE_MODEL = 1.0f;

	// 3Dモデル ファイル名
	LPCSTR g_pszModelPath[MAX_MODEL] = {
		"data/model/airplane.x",
		"data/model/airplane.x",
		"data/model/sky.fbx",
		"data/model/airplane.x",
		"data/model/airplane.x",
	};
}

// 静的メンバ
CAssimpModel* CModel::m_pModels[MAX_MODEL] = { nullptr };

// コンストラクタ
CModel::CModel(CScene* pScene) : CGameObj(pScene)
{
	m_pModel = nullptr;
}

// デストラクタ
CModel::~CModel()
{
}

// 全モデル読込
HRESULT CModel::LoadAll()
{
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDC = GetDeviceContext();

	// Assimp用シェーダ初期化
	if (!CAssimpModel::InitShader(pDevice))
		return E_FAIL;

	// 全モデル読込
	HRESULT hr = S_OK;
	for (int i = 0; i < MAX_MODEL; ++i) {
		SAFE_DELETE(m_pModels[i]);
		m_pModels[i] = new CAssimpModel();
		if (!m_pModels[i]->Load(pDevice, pDC, g_pszModelPath[i])) {
			hr = E_FAIL;
			break;
		}
	}
	return hr;
}

// 全モデル解放
void CModel::ReleaseAll()
{
	// 全モデル解放
	for (int i = 0; i < MAX_MODEL; ++i) {
		if (m_pModels[i]) {
			m_pModels[i]->Release();
			delete m_pModels[i];
			m_pModels[i] = nullptr;
		}
	}

	// Assimp用シェーダ終了処理
	CAssimpModel::UninitShader();
}

// 初期化
HRESULT CModel::Init()
{
	HRESULT hr = CGameObj::Init();
	if (FAILED(hr))
		return hr;

	return hr;
}

// 終了処理
void CModel::Fin()
{
	CGameObj::Fin();
}

// 更新
void CModel::Update()
{

	CGameObj::Update();
}

// 描画
void CModel::Draw()
{
	ID3D11DeviceContext* pDC = GetDeviceContext();

	if (m_pModel) {
		m_pModel->Draw(pDC, GetWorld(), eOpacityOnly);
	}
}

// 半透明部分描画
void CModel::DrawAlpha()
{
	ID3D11DeviceContext* pDC = GetDeviceContext();

	if (m_pModel) {
		m_pModel->Draw(pDC, GetWorld(), eTransparentOnly);
	}

	CGameObj::DrawAlpha();
}

// モデル設定
void CModel::SetModel(EModel model)
{
	if (model < 0 || model >= MAX_MODEL) {
		m_pModel = nullptr;
		SetRadius(0.5f);
		SetBBox(XMFLOAT3(0.5f, 0.5f, 0.5f));
		SetCenter(XMFLOAT3(0, 0, 0));
		//m_fRadius = 0.5f;
		//m_vBBox = XMFLOAT3(0.5f, 0.5f, 0.5f);
		//m_vCenter = XMFLOAT3(0, 0, 0);
	} else {
		m_pModel = m_pModels[model];
		SetRadius(m_pModel->GetRadius());
		SetBBox(m_pModel->GetBBox());
		SetCenter(m_pModel->GetCenter());
		//m_fRadius = m_pModel->GetRadius();
		//m_vBBox = m_pModel->GetBBox();
		//m_vCenter = m_pModel->GetCenter();
	}
}

// Assimpモデル取得
CAssimpModel* CModel::GetAssimp(EModel model)
{
	if (model < 0 || model >= MAX_MODEL)
		return nullptr;
	return m_pModels[model];
}
