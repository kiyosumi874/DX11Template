/**
* @file Input.h
* @brief キーボードやコントローラーの入力
* @details Inputクラスはシングルトンなので注意,コントローラ未実装
* @author shiihara_kiyosumi
* @date 2022_08_05
*/

// 多重インクルード防止
#pragma once

// enumによる警告を消す
#pragma warning(disable:26812)

/**
* @enum ButtonID
* @brief ほかのクラスからこれを指定して入力情報を取得する
*/
enum ButtonID
{
    BUTTON_ID_DOWN,
    BUTTON_ID_LEFT,
    BUTTON_ID_RIGHT,
    BUTTON_ID_UP,
    BUTTON_ID_A,
    BUTTON_ID_B,
    BUTTON_ID_X,
    BUTTON_ID_Y,
    BUTTON_ID_L,
    BUTTON_ID_R,
    BUTTON_ID_BACK,
    BUTTON_ID_START,

    BUTTON_ID_MAX
};

/**
* @enum PAD_ID
* @brief 自分のクラスのm_keysの初期化に使う
*/
enum PAD_ID
{
    PAD_ID_DOWN = (0x0001),// ↓チェックマスク
    PAD_ID_LEFT = (0x0002),// ←チェックマスク
    PAD_ID_RIGHT = (0x0004),// →チェックマスク
    PAD_ID_UP = (0x0008),// ↑チェックマスク
    PAD_ID_A = (0x0010),// Ａボタンチェックマスク
    PAD_ID_B = (0x0020),// Ｂボタンチェックマスク
    PAD_ID_X = (0x0040),// Ｘボタンチェックマスク
    PAD_ID_Y = (0x0080),// Ｙボタンチェックマスク
    PAD_ID_L = (0x0100),// Ｌボタンチェックマスク
    PAD_ID_R = (0x0200),// Ｒボタンチェックマスク
    PAD_ID_BACK = (0x0400),// BACK
    PAD_ID_START = (0x0800)	 // ＳＴＡＲＴボタンチェックマスク

};

// 配列(m_keys)の添え字として使用する
const int KEYBOARD = 0;
const int CONTROLLER_1P = 1;
const int CONTROLLER_2P = 2;

/**
* @class Input
* @brief キーボードなどの入力情報を管理するクラス(シングルトン)
*/
class Input
{
private:
    static Input* m_this; // 静的な自分のポインタ

    // 入力情報をまとめた構造体
    struct KeyInformation
    {
        int keyCode; // VK_SHIFTとか'A'のやつh
        int pressCount; // 状態
    };

    // 配列の一つ目は使用する入力機器の数(キーボードやコントローラーなど)
    // 配列の二つ目はボタンの種類
    KeyInformation m_keys[2][BUTTON_ID_MAX];

    /**
    * @fn Input
    * @brief コンストラクタ
    */
    Input();

    /**
    * @fn ~Input
    * @brief デストラクタ
    */
    ~Input();


public:
    Input(const Input&) = delete; // コピーコンストラクタを delete 指定。
    Input& operator=(const Input&) = delete; // コピー代入演算子も delete 指定。
    Input(Input&&) = delete; // ムーブコンストラクタを delete 指定。
    Input& operator=(Input&&) = delete; // ムーブ代入演算子も delete 指定。

    /**
    * @fn CreateInstance
    * @brief 自分のインスタンスを生成
    */
    static void CreateInstance()
    {
        if (!m_this)
        {
            m_this = new Input;
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
    * @fn IsDown
    * @brief 押した瞬間
    * @param[in] buttonID 調べたいボタンのID
    * @return 押した瞬間であればtrueそうでないならfalse
    */
    static bool IsDown(const ButtonID buttonID) { return ((m_this->m_keys[KEYBOARD][buttonID].pressCount == 1) || (m_this->m_keys[CONTROLLER_1P][buttonID].pressCount == 1)); }
    
    /**
    * @fn IsPress
    * @brief 押しているとき
    * @param[in] buttonID 調べたいボタンのID
    * @return 押しているときであればtrueそうでないならfalse
    */
    static bool IsPress(const ButtonID buttonID) { return m_this->m_keys[KEYBOARD][buttonID].pressCount > 1 || m_this->m_keys[CONTROLLER_1P][buttonID].pressCount > 1; }
    
    /**
    * @fn IsUp
    * @brief 離した瞬間
    * @param[in] buttonID 調べたいボタンのID
    * @return 離した瞬間であればtrueそうでないならfalse
    */
    static bool IsUp(const ButtonID buttonID) { return m_this->m_keys[KEYBOARD][buttonID].pressCount == 0 || m_this->m_keys[CONTROLLER_1P][buttonID].pressCount == 0; }
    
    /**
    * @fn IsRelease
    * @brief 離しているとき
    * @param[in] buttonID 調べたいボタンのID
    * @return 離しているときであればtrueそうでないならfalse
    */
    static bool IsRelease(const ButtonID buttonID) { return m_this->m_keys[KEYBOARD][buttonID].pressCount < 0 || m_this->m_keys[CONTROLLER_1P][buttonID].pressCount < 0; }
};
