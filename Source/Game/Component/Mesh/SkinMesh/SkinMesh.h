/**
* @file SkinMesh.h
* @brief SkinMeshの描画(まだ完全ではない)
* @author shiihara_kiyosumi
* @date 2022_08_18
*/

// ヘッダーファイルのインクルード
#pragma once
#include "Game/Component/Mesh/Mesh.h"
#include "Game/Component/Component.h"

/**
* @class SkinMesh
* @brief ボーン付きのアニメーションできる
*/
class SkinMesh : public Component
{
public:
	/**
	* @fn Start
	* @brief 最初に一度だけ行う処理
	*/
	void Start()override;

	/**
	* @fn Terminate
	* @brief 最後に一度だけ行う処理
	*/
	void Terminate()override;

	/**
	* @fn Update
	* @brief 更新
	*/
	void Update()override;

	/**
	* @fn Draw
	* @brief 描画
	*/
	void Draw()override;

	/**
	* @fn Init
	* @breif 初期化
	* @param[in] fileName モデルのファイル名
	* @return HRESULT S_OKで成功
	*/
	HRESULT Init(LPCSTR fileName);
private:
	/**
	* @fn CreateIndexBuffer
	* @brief インデックスバッファー作成
	* @param[in] dwSize バッファーサイズ
	* @param[in] pIndex 初期化データへのポインター
	* @param[out, optional] ppIndexBuffer 作成されたバッファーオブジェクトのID3D11Bufferインターフェイスへのポインターのアドレス
	* @return HRESULT S_OKで成功
	*/
	HRESULT CreateIndexBuffer(DWORD dwSize, int* pIndex, ID3D11Buffer** ppIndexBuffer);

	/**
	* @fn CreateFromX
	* @brief xfileからスキンメッシュを作成する
	* @param[in] szFileName モデルのファイル名
	* @return HRESULT S_OKで成功
	*/
	HRESULT CreateFromX(LPCSTR szFileName);

	/**
	* @fn ReadSkinInfo
	* @brief xfileからスキン関連の情報を読み出す
	* @param[in] pvVB 頂点データの構造体のポインタ
	* @return HRESULT S_OKで成功
	*/
	HRESULT ReadSkinInfo(MY_SKINVERTEX* pvVB);

	/**
	* @fn SetNewPoseMatrices
	* @brief ボーンを次のポーズ位置にセットする
	* @return HRESULT S_OKで成功
	*/
	void SetNewPoseMatrices();

	/**
	* @fn GetBindPoseMatrix
	* @brief バインドポーズ行列を返す
	* @param[in] index ボーン配列の添え字
	* @return D3DXMATRIX バインドポーズ行列
	*/
	D3DXMATRIX GetBindPoseMatrix(int index);

	/**
	* @fn GetCurrentPoseMatrix
	* @brief 次の(現在の)ポーズ行列を返す
	* @param[in] index ボーン配列の添え字
	* @return D3DXMATRIX 次の(現在の)ポーズ行列
	*/
	D3DXMATRIX GetCurrentPoseMatrix(int index);

	/**
	* @fn GetBoneNames
	* @brief ボーンの名前取得
	* @param[in] iBoneIndex ボーン配列の添え字
	* @return CHAR* ボーンの名前
	*/
	CHAR* GetBoneNames(int iBoneIndex);

	//Dx11
	ID3D11SamplerState* m_pSampler; // textureを貼るのに使ったりする
	ID3D11VertexShader* m_pVertexShader; // 頂点シェーダーを管理する
	ID3D11PixelShader* m_pPixelShader; // ピクセルシェーダーを管理する
	ID3D11InputLayout* m_pVertexLayout; // 頂点データをグラフィックスパイプラインのInputAssembler(IA)ステージに供給する方法の定義を保持する
	ID3D11Buffer* m_pConstantBufferLight; // シェーダーに渡す定数(SHADER_SKIN_GLOBAL0)
	ID3D11Buffer* m_pConstantBufferWVPMaterial; // シェーダーに渡す定数(SHADER_SKIN_GLOBAL1)
	ID3D11Buffer* m_pConstantBufferBone; // シェーダーに渡す定数(SHADER_GLOBAL_BONES)

	//メッシュ
	D3DXPARSER* m_pMeshParser; // xfileの解析クラス
	ID3D11Buffer* m_pVertexBuffer; // 頂点データを格納するバッファ
	ID3D11Buffer** m_ppIndexBuffer; // 頂点の並び順を格納するバッファ
	MY_SKINMATERIAL* m_pMaterial; // マテリアルの構造体
	DWORD m_dwNumMaterial; // マテリアルの数

	// ボーン関連
	int m_iNumBone; // ボーンの数
	BONE* m_BoneArray; // ボーン構造体の動的配列
};