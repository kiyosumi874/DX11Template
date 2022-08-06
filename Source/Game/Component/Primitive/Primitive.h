/**
* @file Primitive.h
* @brief プリミティブ(親クラス)
* @author shiihara_kiyosumi
* @date 2022_08_05
*/

// ヘッダーファイルのインクルード
#pragma once
#include <D3D11.h>
#include "Game/Component/Component.h"

class Transform;

/**
* @class Primitive
* @brief Pointなどの親クラス
*/
class Primitive : public Component
{
public:
	/**
	* @fn PrimitiveCom
	* @brief コンストラクタ
	*/
	Primitive();

	/**
	* @fn ~PrimitiveCom
	* @brief デストラクタ
	*/
	virtual ~Primitive() override;

	Transform* m_transform; // transform

	/**
	* @fn Is2D
	* @brief trueで2D描画
	* @param[in] is2D trueで2D描画
	*/
	void Is2D(const bool is2D)
	{
		m_is2D = is2D;
	}

protected:
	/**
	* @fn DrawCommon
	* @brief Primitive共通の描画
	*/
	void DrawCommon();
	ID3D11Buffer* m_pVertexBuffer; // バーテックスバッファー
	bool m_is2D; // trueで2D描画 ViewとProjが無視される,ウィンドウサイズをVSに渡して2Dに調節する
};
