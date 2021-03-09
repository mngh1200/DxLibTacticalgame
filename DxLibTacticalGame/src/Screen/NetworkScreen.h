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
		constexpr static int BUTTON_W = 600;	//! �{�^����
		constexpr static int BUTTON_H = 100;		//! �{�^������
		constexpr static int BUTTON_X = (WIN_W - BUTTON_W) / 2;	//! �{�^�����Wx
		constexpr static int BUTTON_Y = 200;	//! �{�^�����Wy
		constexpr static int BUTTON_MARGIN = 30;//! �{�^���]��

		constexpr static int BACK_SIZE = 60;	//! �߂�{�^���̃T�C�Y

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
	};
}