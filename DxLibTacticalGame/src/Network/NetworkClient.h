#pragma once
#include <climits>
#include "FrameWork/Game.h"
#include "Entity/UI/Button/TextButton.h"
#include "Entity/UI/ModalFrame.h"
#include "Entity/View/Text.h"
#include "Entity/UI/TextBox.h"

using namespace std;
using namespace Entity;

/**
 * @file NetworkClient.h
 * @brief �l�b�g���[�N�ڑ��҂��i���������j���̏����Ǘ�
 */

namespace Network
{
	class NetworkClient
	{
	public:
		NetworkClient() : state_(State::NONE), netHandle_(-1), ableConnecButton_(false) {};
		~NetworkClient() {};

		void start();
		void end();


		// checkAndUpdate�̕Ԃ��l�p�iScreen�N���X�ɏ󋵂�Ԃ��j
		enum Result
		{
			CONTINUE,	//! �p��
			CANCEL,		//! �L�����Z��
			BATTLE		//! �o�g����ʂ�
		};

		int checkAndUpdate(weak_ptr<Entity::Object> hitObjWp, int x, int y, int button, int eventType);

	private:
		void connectNetwork();
		void getIpData(IPDATA& ipData);

		constexpr static int MODAL_W = 600; //! ����� ��
		constexpr static int MODAL_H = 400; //! ����� ����
		constexpr static int MODAL_X = (WIN_W - MODAL_W) / 2; //! ����� x���W
		constexpr static int MODAL_Y = (WIN_H - MODAL_H) / 2; //! ����� y���W

		constexpr static int MODAL_PADDING = 20; //! ����ʗ]��

		constexpr static int CONTENT_X = MODAL_X + MODAL_PADDING; //! ����ʗv�f��x���W
		constexpr static int CONTENT_Y = MODAL_Y + MODAL_PADDING + ModalFrame::HEAD_H; //! ����ʗv�f��y���W

		constexpr static int IP_INPUT_W = 85; //! IP�A�h���X���̕�
		constexpr static int IP_INPUT_H = 50;  //! IP�A�h���X���̍���
		constexpr static int IP_INPUT_MARGIN = 20; //! IP�A�h���X���̗]��
		constexpr static int IP_INPUT_Y = CONTENT_Y + 24 + 20; //! IP�A�h���X����y���W


		constexpr static int BUTTON_H = 75; //! �{�^���̍���
		constexpr static int BUTTON_Y = MODAL_Y + MODAL_H - MODAL_PADDING - BUTTON_H; //! �{�^����y���W
		constexpr static int BUTTON_MARGIN_X = 20; //! �{�^���̗]��
		constexpr static int BUTTON_W = (MODAL_W - MODAL_PADDING * 2 - BUTTON_MARGIN_X) / 2; //! �{�^���̕�

		constexpr static int STATUS_LINE_H = 50; //! �󋵂̈�s�̍���

		const static int LAYER_FRAME; //! �t���[���̃��C���[ID
		const static int LAYER_CONTENT; //! ���[�_�����̓��e�̃��C���[ID

		shared_ptr<Text> statusText_; //! �ڑ��󋵂������e�L�X�g
		shared_ptr<TextButton> connectButton_; //! �ڑ��{�^��

		vector<shared_ptr<TextBox>> ipAdressInputList_; //! IP�A�h���X���͗��̃��X�g

		// �R���e���g�̃I�u�W�F�N�gID
		enum ContentId
		{
			CANCEL_BTN
		};

		// �󋵂̎��
		enum State
		{
			NONE,
			CONNECTED	//! �ڑ��ς݁i�z�X�g�̃��[���ݒ�ҋ@���j
		};
		
		void setState(State state);
		void adjustConnectButtonValid();

		State state_; //! ��

		int netHandle_; //! �l�b�g���[�N�n���h��

		bool ableConnecButton_; //! IP�A�h���X���͍ς݂ł��邩
	};


}