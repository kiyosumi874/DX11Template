/**
* @file Object.h
* @brief �Q�[���I�u�W�F�N�g
* @author shiihara_kiyosumi
* @date 2022_08_05
*/

// �w�b�_�[�t�@�C���̃C���N���[�h
#pragma once
#include <list>
#include "Component.h"

// using�錾
using std::list;

/**
* @class Object
* @brief �Q�[���I�u�W�F�N�g
*/
class Object
{
public:
    /**
    * @fn Object
    * @brief �R���X�g���N�^
    */
    Object() {}

    /**
    * @fn ~Object
    * @brief �f�X�g���N�^
    */
    ~Object() 
    { 
        for (auto com : m_componentList)
        {
            com->Terminate();
            delete com;
        } 
        m_componentList.clear();
    }

    /**
    * @fn Update
    * @brief �X�V
    */
    void Update()
    { for (auto com : m_componentList) { com->Update(); } }

    /**
    * @fn Draw
    * @brief �`��
    */
    void Draw()
    { for (auto com : m_componentList) { com->Draw(); } }

    /**
    * @fn GetComponent
    * @brief �I�u�W�F�N�g�������Ă���R���|�[�l���g�擾
    */
    template<class T>
    T* GetComponent()
    {
        for (auto com : m_componentList)
        {
            T* buff = dynamic_cast<T*>(com);
            if (buff != nullptr) 
            { 
                return buff; 
            }
        }
        return nullptr;
    }

    /**
    * @fn GetComponent
    * @brief �I�u�W�F�N�g�������Ă���R���|�[�l���g�ǉ�
    */
    template<class T>
    T* AddComponent()
    {
        T* buff = new T();
        buff->m_parent = this;
        m_componentList.emplace_front(buff);
        buff->Start();
        return buff;
    }

    list<Component*> m_componentList; // �R���|�[�l���g���X�g
};