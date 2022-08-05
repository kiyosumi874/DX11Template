/**
* @file Point.h
* @brief 点を描画
* @author shiihara_kiyosumi
* @date 2022_08_05
*/

// ヘッダーファイルのインクルード
#pragma once
#include "System/Math/Math.h"
#include "Game/Primitive/Primitive.h"

// using宣言
using math::Vector3D;

/**
* @class Point
* @brief 点を描画するクラス
*/
class Point : public Primitive
{
public:
	/**
	* @fn Point
	* @brief コンストラクタ
	*/
	Point();

	/**
	* @fn Point
	* @brief 引数付きコンストラクタ
	* @param[in] pos 座標
	* @param[in] is2D trueで2D描画
	*/
	Point(const Vector3D& pos, bool is2D = false);

	/**
	* @fn ~Point
	* @brief デストラクタ
	*/
	~Point() override;

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
