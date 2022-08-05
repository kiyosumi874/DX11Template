/**
* @file Fps.h
* @brief Fpsの固定
* @details Fpsクラスはシングルトンなので注意
* @author shiihara_kiyosumi
* @date 2022_08_05
*/

// ヘッダーファイルのインクルード
#pragma once
#include <Windows.h>

/**
* @class Fps
* @brief Fpsを固定するクラス(シングルトン)
*/
class Fps
{
public:
	/**
	* @fn CreateInstance
	* @brief 自分のインスタンスを生成
	*/
	static void CreateInstance()
	{
		if (!m_this)
		{
			m_this = new Fps;
		}
	}

	/**
	* @fn DeleteInstance
	* @brief 自分のインスタンスを削除
	*/
	static void DeleteInstance()
	{
		if (m_this)
		{
			delete m_this;
			m_this = nullptr;
		}
	}

	/**
	* @fn Update
	* @brief 更新
	* @attention １ループに一回必ず呼び出すこと
	*/
	static void Update();

	/**
	* @fn Draw
	* @brief 出力(VisualStudio)に現在のFpsを描画
	*/
	static void Draw();

private:
	/**
	* @fn Fps
	* @brief コンストラクタ
	*/
	Fps();

	/**
	* @fn ~Fps
	* @brief デストラクタ
	*/
	~Fps();


	float m_fps; // 今のfps
	float m_frameTime; // 1ループの経過時間
	LARGE_INTEGER m_timeStart; // 1ループの最初に計測した時間
	LARGE_INTEGER m_timeEnd; // 1ループの最後に計測した時間
	LARGE_INTEGER m_timeFreq; // 周波数

	static Fps* m_this; // 静的な自分のポインタ

};