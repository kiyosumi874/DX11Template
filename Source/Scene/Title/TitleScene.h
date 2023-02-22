/**
* @file TitleScene.h
* @brief タイトルシーン
* @details Sceneを継承している
* @author shiihara_kiyosumi
* @date 2022_08_02
*/

// ヘッダーファイルのインクルード
#pragma once
#include "Scene/Scene.h"
#include "System/ImguiWrapper.h"

class Image;
class Camera;

/**
* @class TitleScene
* @brief TitleSceneクラス
* @details Sceneを継承している
*/
class TitleScene : public Scene
{
public:
	/**
	* @fn TitleScene
	* @brief コンストラクタ
	*/
	TitleScene();

	/**
	* @fn ~TitleScene
	* @brief デストラクタ
	*/
	~TitleScene() override;

	/**
	* @fn Update
	* @brief 更新
	* @return TAG_SCENE シーンの種類
	*/
	TAG_SCENE Update() override;

	/**
	* @fn Draw
	* @brief 描画
	*/
	void Draw() override;

private:
#ifdef _DEBUG
	bool ImageImGuiConfig(Image*& pImage, const char* windowName, const char* fileName, const int ID);
#endif // _DEBUG

	Image* m_image;
	Camera* m_camera;
	ImguiWrapper<Image>::Setting* setting;
	void InitCamera();
	void InitStaticMesh();
	void InitHierarchyMesh();
	void InitSkinMesh();
	void InitUI();

};