/**
* @file Object.h
* @brief ゲームオブジェクト
* @author shiihara_kiyosumi
* @date 2022_08_05
*/

// ヘッダーファイルのインクルード
#pragma once
#include <list>
#include "Component.h"

// using宣言
using std::list;

/**
* @class Object
* @brief ゲームオブジェクト
*/
class Object
{
public:
    /**
    * @fn Object
    * @brief コンストラクタ
    */
    Object() {}

    /**
    * @fn ~Object
    * @brief デストラクタ
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
    * @brief 更新
    */
    void Update()
    { for (auto com : m_componentList) { com->Update(); } }

    /**
    * @fn Draw
    * @brief 描画
    */
    void Draw()
    { for (auto com : m_componentList) { com->Draw(); } }

    /**
    * @fn GetComponent
    * @brief オブジェクトが持っているコンポーネント取得
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
    * @brief オブジェクトが持っているコンポーネント追加
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

    list<Component*> m_componentList; // コンポーネントリスト
};