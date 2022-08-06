/**
* @file Point.h
* @brief 点を描画(コンポーネント)
* @author shiihara_kiyosumi
* @date 2022_08_05
*/

// ヘッダーファイルのインクルード
#pragma once
#include "System/Math/Math.h"
#include "Game/Component/Primitive/Primitive.h"

/**
* @class Point
* @brief 点を描画するクラス
*/
class Point : public Primitive
{
public:
	/**
	* @fn Start
	* @brief 生成したときに最初に一回だけ走る関数
	*/
	void Start() override;

	/**
	* @fn Draw
	* @brief 描画
	*/
	void Draw() override;

private:
	/**
	* @fn CreateVertexBuffer
	* @brief バーテックスバッファー作成
	*/
	void CreateVertexBuffer();

	/**
	* @fn SetVertexBuffer
	* @brief バーテックスバッファーをセット
	*/
	void SetVertexBuffer();

};
