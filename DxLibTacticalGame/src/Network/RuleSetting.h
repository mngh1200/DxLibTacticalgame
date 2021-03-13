#pragma once
#include <climits>
#include "FrameWork/Game.h"
#include "NetworkDefine.h"
#include "Entity/UI/Button/TextButton.h"
#include "Entity/UI/RadioButton.h"
#include "Entity/UI/ModalFrame.h"
#include "Entity/View/Text.h"
#include "Utility/Timer.h"

using namespace std;
using namespace Entity;

/**
 * @file RuleSetting.h
 * @brief �ʐM�ΐ�̃��[���ݒ��UI�Z�b�g�Ə���
 */

namespace Network
{
	class RuleSetting
	{
	public:
		RuleSetting() : 
			netHandle_(-1),
			isConnect_(true)
		{};
		~RuleSetting() {};

		void start(int netHandle);
		void end();

		// checkAndUpdate�̕Ԃ��l�p�iScreen�N���X�ɏ󋵂�Ԃ��j
		enum Result
		{
			CONTINUE,		//! �p��
			CANCEL,			//! �L�����Z��
			START_BATTLE	//! �o�g����ʂ֐i��
		};

		int checkAndUpdate(weak_ptr<Entity::Object> hitObjWp, int x, int y, int button, int eventType);

	private:

		constexpr static int MODAL_W = 600; //! ����� ��
		constexpr static int MODAL_H = 500; //! ����� ����
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
		constexpr static int LINE_MARGIN = 15; //! ��s�̗]��


		const static int LAYER_FRAME; //! �t���[���̃��C���[ID
		const static int LAYER_CONTENT; //! ���[�_�����̓��e�̃��C���[ID


		bool isConnect_; //! �ڑ���

		int netHandle_; //! �l�b�g���[�N�n���h��

		shared_ptr<Text> statusText_; //! �ڑ��󋵂������e�L�X�g

		shared_ptr<TextButton> startButton_; //! �J�n�{�^��
		shared_ptr<TextButton> closeButton_; //! ����or�ؒf�{�^��

		shared_ptr<RadioButton> unitNum_;   //! ���j�b�g���w��̃��W�I�{�^��
		shared_ptr<RadioButton> mapSelect_; //! �}�b�v�w��̃��W�I�{�^��

	};
}