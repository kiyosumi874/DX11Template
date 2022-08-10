/**
* @file Triangle.h
* @brief 三角形を描画(コンポーネント)
* @author shiihara_kiyosumi
* @date 2022_08_08
*/

// ヘッダーファイルのインクルード
#pragma once
#include "System/Math/Math.h"
#include "Game/Component/Primitive/Primitive.h"

// using宣言
using math::Vector3D;

/**
* @class Triangle
* @brief 三角形を描画するクラス
*/
class Triangle : public Primitive
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

	/**
	* @fn Init
	* @brief 初期化(絶対呼び出して)
	* @param[in] pos1 描画する三角形の座標
	* @param[in] pos2 描画する三角形の座標
	* @param[in] pos3 描画する三角形の座標
	*/
	void Init(const Vector3D& pos1, const Vector3D& pos2, const Vector3D& pos3);

private:
	/**
	* @fn CreateVertexBuffer
	* @brief バーテックスバッファー作成
	* @param[in] pos1 描画する三角形の座標
	* @param[in] pos2 描画する三角形の座標
	* @param[in] pos3 描画する三角形の座標
	*/
	void CreateVertexBuffer(const Vector3D& pos1, const Vector3D& pos2, const Vector3D& pos3);

	/**
	* @fn SetVertexBuffer
	* @brief バーテックスバッファーをセット
	*/
	void SetVertexBuffer();

};
