#pragma once
#include <climits>
#include <memory>
#include "MenuScreen.h"
#include "FrameWork/Game.h"
#include "Utility/ResourceManager.h"
#include "Entity/UI/Back.h"
#include "Entity/UI/TextBox.h"
#include "Entity/UI/Button/TextButton.h"
#include "Entity/View/Text.h"
#include "Network/NetworkHost.h"
#include "Network/NetworkClient.h"
#include "Network/RuleSetting.h"

using namespace std;
using namespace Network;

/**
 * @file NetworkScreen.h
 * @brief �ʐM�ΐ폀���p�̉��
 */


namespace Screen
{
	class NetworkScreen : public ScreenBase
	{
	public:
		NetworkScreen() : nextScreen_(), hostManager_{}, clientManager_{} {};
		~NetworkScreen() {};

		// ���C���[�̎��
		enum Layer
		{
			MASK,
			MODAL_CONTENT,
			MODAL_FRAME,
			UI,
			BACK,
			LEN
		};

		void init();
		void updateByEvents(weak_ptr < Entity::Object> hitObjWp, int x, int y, int button, int eventType);
		void updateByAnimation();
	private:
		void startBattle(int netHandler_, const RuleData& ruleData, bool isServer);

		constexpr static int BUTTON_W = 600;	//! �{�^����
		constexpr static int BUTTON_H = 100;		//! �{�^������
		constexpr static int BUTTON_X = (WIN_W - BUTTON_W) / 2;	//! �{�^�����Wx
		constexpr static int BUTTON_Y = 140;	//! �{�^�����Wy
		constexpr static int BUTTON_MARGIN = 30;//! �{�^���]��

		constexpr static int BACK_BTN_W = 300;	//! �߂�{�^���̕�
		constexpr static int BACK_BTN_H = 60;	//! �߂�{�^���̍���
		constexpr static int BACK_BTN_X = (WIN_W - BACK_BTN_W) / 2;	//! �߂�{�^�����Wx
		constexpr static int BACK_BTN_Y = WIN_H - 120;	//! �߂�{�^�����Wy

		constexpr static int COMMENT_MARGIN = 10;	//! �R�����g�̗]��
		constexpr static int COMMENT_LINE_H = 26;		//! �R�����g�̂P�s���̍���

		// �V�[���̎��
		enum Scene
		{
			INIT,
			HOST,
			CLIENT,
			RULE_SET
		};

		// UI�̎��
		enum UIid
		{
			CREATE_ROOM_BUTTON,
			SEARCH_ROOM_BUTTON,
			QUIT_BUTTON
		};

		ScreenBase *nextScreen_; //! ���ɊJ�����

		NetworkHost hostManager_; //! �l�b�g���[�N�ڑ��̃z�X�g�ɂȂ������p�̃N���X
		NetworkClient clientManager_; //! �l�b�g���[�N�ڑ��̃N���C�A���g�ɂȂ������p�̃N���X
		RuleSetting ruleSetting_; //! ���[���ݒ莞�Ɏg�p����N���X
	};
}