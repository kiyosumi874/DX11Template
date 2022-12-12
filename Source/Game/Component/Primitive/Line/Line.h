/**
* @file Line.h
* @brief 線を描画(コンポーネント)
* @author shiihara_kiyosumi
* @date 2022_08_08
*/

// ヘッダーファイルのインクルード
#pragma once
#include "System/Vector3D.h"
#include "Game/Component/Primitive/Primitive.h"

/**
* @class Line
* @brief 線を描画するクラス
*/
class Line : public Primitive
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
	* @param[in] pos1 描画する線の起点座標
	* @param[in] pos2 描画する線の終点座標
	*/
	void Init(const Vector3D& pos1, const Vector3D& pos2);

private:
	/**
	* @fn CreateVertexBuffer
	* @brief バーテックスバッファー作成
	* @param[in] pos1 描画する線の起点座標
	* @param[in] pos2 描画する線の終点座標
	*/
	void CreateVertexBuffer(const Vector3D& pos1, const Vector3D& pos2);

	/**
	* @fn SetVertexBuffer
	* @brief バーテックスバッファーをセット
	*/
	void SetVertexBuffer();

};
