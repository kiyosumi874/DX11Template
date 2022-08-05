/**
* @file Input.h
* @brief �L�[�{�[�h��R���g���[���[�̓���
* @details Input�N���X�̓V���O���g���Ȃ̂Œ���,�R���g���[��������
* @author shiihara_kiyosumi
* @date 2022_08_05
*/

// ���d�C���N���[�h�h�~
#pragma once

// enum�ɂ��x��������
#pragma warning(disable:26812)

/**
* @enum ButtonID
* @brief �ق��̃N���X���炱����w�肵�ē��͏����擾����
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
* @brief �����̃N���X��m_keys�̏������Ɏg��
*/
enum PAD_ID
{
    PAD_ID_DOWN = (0x0001),// ���`�F�b�N�}�X�N
    PAD_ID_LEFT = (0x0002),// ���`�F�b�N�}�X�N
    PAD_ID_RIGHT = (0x0004),// ���`�F�b�N�}�X�N
    PAD_ID_UP = (0x0008),// ���`�F�b�N�}�X�N
    PAD_ID_A = (0x0010),// �`�{�^���`�F�b�N�}�X�N
    PAD_ID_B = (0x0020),// �a�{�^���`�F�b�N�}�X�N
    PAD_ID_X = (0x0040),// �w�{�^���`�F�b�N�}�X�N
    PAD_ID_Y = (0x0080),// �x�{�^���`�F�b�N�}�X�N
    PAD_ID_L = (0x0100),// �k�{�^���`�F�b�N�}�X�N
    PAD_ID_R = (0x0200),// �q�{�^���`�F�b�N�}�X�N
    PAD_ID_BACK = (0x0400),// BACK
    PAD_ID_START = (0x0800)	 // �r�s�`�q�s�{�^���`�F�b�N�}�X�N

};

// �z��(m_keys)�̓Y�����Ƃ��Ďg�p����
const int KEYBOARD = 0;
const int CONTROLLER_1P = 1;
const int CONTROLLER_2P = 2;

/**
* @class Input
* @brief �L�[�{�[�h�Ȃǂ̓��͏����Ǘ�����N���X(�V���O���g��)
*/
class Input
{
private:
    static Input* m_this; // �ÓI�Ȏ����̃|�C���^

    // ���͏����܂Ƃ߂��\����
    struct KeyInformation
    {
        int keyCode; // VK_SHIFT�Ƃ�'A'�̂��h
        int pressCount; // ���
    };

    // �z��̈�ڂ͎g�p������͋@��̐�(�L�[�{�[�h��R���g���[���[�Ȃ�)
    // �z��̓�ڂ̓{�^���̎��
    KeyInformation m_keys[2][BUTTON_ID_MAX];

    /**
    * @fn Input
    * @brief �R���X�g���N�^
    */
    Input();

    /**
    * @fn ~Input
    * @brief �f�X�g���N�^
    */
    ~Input();


public:
    Input(const Input&) = delete; // �R�s�[�R���X�g���N�^�� delete �w��B
    Input& operator=(const Input&) = delete; // �R�s�[������Z�q�� delete �w��B
    Input(Input&&) = delete; // ���[�u�R���X�g���N�^�� delete �w��B
    Input& operator=(Input&&) = delete; // ���[�u������Z�q�� delete �w��B

    /**
    * @fn CreateInstance
    * @brief �����̃C���X�^���X�𐶐�
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
    * @brief �����̃C���X�^���X���폜
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
    * @brief �X�V
    * @attention �P���[�v�Ɉ��K���Ăяo������
    */
    static void Update();


    /**
    * @fn IsDown
    * @brief �������u��
    * @param[in] buttonID ���ׂ����{�^����ID
    * @return �������u�Ԃł����true�����łȂ��Ȃ�false
    */
    static bool IsDown(const ButtonID buttonID) { return ((m_this->m_keys[KEYBOARD][buttonID].pressCount == 1) || (m_this->m_keys[CONTROLLER_1P][buttonID].pressCount == 1)); }
    
    /**
    * @fn IsPress
    * @brief �����Ă���Ƃ�
    * @param[in] buttonID ���ׂ����{�^����ID
    * @return �����Ă���Ƃ��ł����true�����łȂ��Ȃ�false
    */
    static bool IsPress(const ButtonID buttonID) { return m_this->m_keys[KEYBOARD][buttonID].pressCount > 1 || m_this->m_keys[CONTROLLER_1P][buttonID].pressCount > 1; }
    
    /**
    * @fn IsUp
    * @brief �������u��
    * @param[in] buttonID ���ׂ����{�^����ID
    * @return �������u�Ԃł����true�����łȂ��Ȃ�false
    */
    static bool IsUp(const ButtonID buttonID) { return m_this->m_keys[KEYBOARD][buttonID].pressCount == 0 || m_this->m_keys[CONTROLLER_1P][buttonID].pressCount == 0; }
    
    /**
    * @fn IsRelease
    * @brief �����Ă���Ƃ�
    * @param[in] buttonID ���ׂ����{�^����ID
    * @return �����Ă���Ƃ��ł����true�����łȂ��Ȃ�false
    */
    static bool IsRelease(const ButtonID buttonID) { return m_this->m_keys[KEYBOARD][buttonID].pressCount < 0 || m_this->m_keys[CONTROLLER_1P][buttonID].pressCount < 0; }
};
