/**
* @file Scene.h
* @brief �V�[���̐e
* @author shiihara_kiyosumi
* @date 2022_08_02
*/

// ���d�C���N���[�h�h�~
#pragma once
#include <list>
#include "Game/Component/Object.h"

// using�錾
using std::list;

/**
	* @enum TAG_SCENE
	* @brief �V�[���̎��
	*/
enum class TAG_SCENE
{
	TITLE,
	PLAY,
	CLEAR,
	OVER,
	END,
	TEST,

	NONE
};

/**
* @class Scene
* @brief �V�[���̐e�N���X
*/
class Scene
{
public:
	/**
	* @fn Scene
	* @brief �R���X�g���N�^
	*/
	Scene() {}

	/**
	* @fn ~Scene
	* @brief �f�X�g���N�^(���z�֐�)
	*/
	virtual ~Scene()
	{
		if (!m_objectList.empty())
		{
			for (auto obj : m_objectList)
			{
				delete obj;
			}
			m_objectList.clear();
		}
	}

	/**
	* @fn Update
	* @brief �X�V(�������z�֐�)
	* @return TAG_SCENE �V�[���̎��
	*/
	virtual TAG_SCENE Update() = 0;

	/**
	* @fn Draw
	* @brief �`��(�������z�֐�)
	*/
	virtual void Draw() = 0;
protected:

	list<Object*> m_objectList; // �I�u�W�F�N�g���X�g
};