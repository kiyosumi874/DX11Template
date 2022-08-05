/**
* @file Primitive.h
* @brief プリミティブ(親クラス)
* @author shiihara_kiyosumi
* @date 2022_08_05
*/

// ヘッダーファイルのインクルード
#pragma once
#include <D3D11.h>
#include "System/Math/Math.h"

// using宣言
using math::Vector3D;

/**
* @class Primitive
* @brief Pointなどの親クラス
*/
class Primitive
{
public:
	/**
	* @fn Primitive
	* @brief コンストラクタ
	*/
	Primitive();

	/**
	* @fn ~Primitive
	* @brief デストラクタ
	*/
	virtual ~Primitive();

	/**
	* @fn Draw
	* @brief 描画
	*/
	virtual void Draw() = 0;

	Vector3D m_pos; // ポジション
protected:
	/**
	* @fn DrawCommon
	* @brief Primitive共通の描画
	*/
	void DrawCommon();
	ID3D11Buffer* m_pVertexBuffer; // バーテックスバッファー
	bool m_is2D; // trueで2D描画 ViewとProjが無視される,ウィンドウサイズをVSに渡して2Dに調節する

};
