/**
* @file Component.h
* @brief コンポーネント
* @author shiihara_kiyosumi
* @date 2022_08_05
*/

// 多重インクルード防止
#pragma once

// 前方宣言
class Object;

/**
* @class Component
* @brief コンポーネントクラス(親クラス)
*/
class Component
{
public:
    /**
    * @fn Component
    * @brief コンストラクタ
    */
    Component()
        : m_parent(nullptr)
    {}

    /**
    * @fn ~Component
    * @brief デストラクタ
    */
    virtual ~Component() {}

    /**
    * @fn Start
    * @brief 生成したときに最初に一回だけ走る関数
    */
    virtual void Start() {}

    /**
    * @fn Terminate
    * @brief 削除したときに最初に一回だけ走る関数
    */
    virtual void Terminate() {}

    /**
    * @fn Update
    * @brief 更新
    */
    virtual void Update() {}

    /**
    * @fn Draw
    * @brief 描画
    */
    virtual void Draw() {}

    Object* m_parent; // Objectクラスのポインタ(GetComponentなどに使う)
};