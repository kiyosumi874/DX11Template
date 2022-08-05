#pragma once
#include <list>
#include "Component.h"

using std::list;

class Object
{
public:
    Object() {}

    ~Object() 
    { for (auto com : m_componentList) { delete com; } }

    void Update()
    { for (auto com : m_componentList) { com->Update(); } }

    void Draw()
    { for (auto com : m_componentList) { com->Draw(); } }

    //�I�u�W�F�N�g�������Ă���R���|�[�l���g���擾
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

    //�I�u�W�F�N�g�������Ă���R���|�[�l���g��ǉ�
    template<class T>
    T* AddComponent()
    {
        T* buff = new T();
        buff->Parent = this;
        m_componentList.push_back(buff);
        buff->Start();
        return buff;
    }

    list<Component*> m_componentList;
};