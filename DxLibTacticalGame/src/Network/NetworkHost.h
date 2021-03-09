#pragma once
#include <climits>
#include "FrameWork/Game.h"
#include "NetworkDefine.h"
#include "Entity/UI/Button/TextButton.h"
#include "Entity/UI/ModalFrame.h"
#include "Entity/View/Text.h"
#include "Utility/Timer.h"

using namespace std;
using namespace Entity;

/**
 * @file NetworkHost.h
 * @brief �l�b�g���[�N�ڑ��҂��i���������j���̏����Ǘ�
 */

namespace Network
{
	class NetworkHost
	{
	public:
		NetworkHost() : isConnected_(false), clientIp_{}, textUpdataTimer_{}, dotCount_(1) {};
		~NetworkHost() {};

		void start();
		void end();

		// checkAndUpdate�̕Ԃ��l�p�iScreen�N���X�ɏ󋵂�Ԃ��j
		enum Result
		{
			CONTINUE,	//! �p��
			CANCEL,		//! �L�����Z��
			SET_RULE	//! ���[���ݒ�֐i��
		};

		int checkAndUpdate(weak_ptr<Entity::Object> hitObjWp, int x, int y, int button, int eventType);

	private:
		void checkAcceptNetwork();

		constexpr static int MODAL_W = 600; //! ����� ��
		constexpr static int MODAL_H = 330; //! ����� ����
		constexpr static int MODAL_X = (WIN_W - MODAL_W) / 2; //! ����� x���W
		constexpr static int MODAL_Y = (WIN_H - MODAL_H) / 2; //! ����� y���W

		constexpr static int MODAL_PADDING = 20; //! ����ʗ]��

		constexpr static int CONTENT_X = MODAL_X + MODAL_PADDING; //! ����ʗv�f��x���W
		constexpr static int CONTENT_Y = MODAL_Y + MODAL_PADDING + ModalFrame::HEAD_H; //! ����ʗv�f��y���W

		constexpr static int BUTTON_H = 75; //! �{�^���̍���
		constexpr static int BUTTON_Y = MODAL_Y + MODAL_H - MODAL_PADDING - BUTTON_H; //! �{�^����y���W
		constexpr static int BUTTON_MARGIN_X = 20; //! �{�^���̗]��
		constexpr static int BUTTON_W = (MODAL_W - MODAL_PADDING * 2 - BUTTON_MARGIN_X) / 2; //! �{�^���̕�

		constexpr static int LINE_H = 32; //! ��s�T�C�Y
		constexpr static int LINE_MARGIN = 30; //! ��s�̗]��

		constexpr static int TIMER_MS = 500; //! �󋵃e�L�X�g�X�V�^�C�}�[


		const static int LAYER_FRAME; //! �t���[���̃��C���[ID
		const static int LAYER_CONTENT; //! ���[�_�����̓��e�̃��C���[ID

		const static string WAIT_CONNECT_TEXT; //! �ڑ��҂��󋵂ł̃e�L�X�g

		// �R���e���g�̃I�u�W�F�N�gID
		enum ContentId
		{
			CANCEL_BTN
		};

		shared_ptr<Text> statusText_; //! �ڑ��󋵂������e�L�X�g
		shared_ptr<TextButton> ruleSetButton_; //! ���[���ݒ�{�^��

		bool isConnected_; //! �ڑ��ς݂ł��邩

		IPDATA clientIp_; //! �N���C�A���g����IP�A�h���X

		int netHandle_; //! �l�b�g���[�N�n���h��

		Utility::Timer textUpdataTimer_; //! �ڑ��󋵃e�L�X�g�̏�Ԃ��X�V���邽�߂̃^�C�}�[

		int dotCount_; //! �ڑ��󋵃e�L�X�g�̃h�b�g��
	};
}