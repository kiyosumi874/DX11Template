/**
* @file Component.h
* @brief �R���|�[�l���g
* @author shiihara_kiyosumi
* @date 2022_08_05
*/

// ���d�C���N���[�h�h�~
#pragma once

// �O���錾
class Object;

/**
* @class Component
* @brief �R���|�[�l���g�N���X(�e�N���X)
*/
class Component
{
public:
    /**
    * @fn Component
    * @brief �R���X�g���N�^
    */
    Component()
        : m_parent(nullptr)
    {}

    /**
    * @fn ~Component
    * @brief �f�X�g���N�^
    */
    virtual ~Component() {}

    /**
    * @fn Start
    * @brief ���������Ƃ��Ɉ�񂾂�����֐�
    */
    virtual void Start() {}

    /**
    * @fn Terminate
    * @brief �폜�����Ƃ��Ɉ�񂾂�����֐�
    */
    virtual void Terminate() {}

    /**
    * @fn Update
    * @brief �X�V
    */
    virtual void Update() {}

    /**
    * @fn Draw
    * @brief �`��
    */
    virtual void Draw() {}

    Object* m_parent; // Object�N���X�̃|�C���^(GetComponent�ȂǂɎg��)
};