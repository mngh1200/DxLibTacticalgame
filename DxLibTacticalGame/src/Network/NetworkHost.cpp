#include "NetworkHost.h"
#include "Screen/NetworkScreen.h"

namespace Network
{
	const int NetworkHost::LAYER_CONTENT = Screen::NetworkScreen::Layer::MODAL_CONTENT;
	const int NetworkHost::LAYER_FRAME = Screen::NetworkScreen::Layer::MODAL_FRAME;
	const string NetworkHost::WAIT_CONNECT_TEXT = "�N���C�A���g���̐ڑ��҂�";

	/**
	 * @fn
	 * ������IP�A�h���X���擾
	*/
	string getIpAdress()
	{
		IPDATA IPAddress[16];
		int IPNum;

		DxLib::GetMyIPAddress(IPAddress, 16, &IPNum);

		return to_string(IPAddress->d1) + "." +
			to_string(IPAddress->d2) + "." +
			to_string(IPAddress->d3) + "." +
			to_string(IPAddress->d4);
	}

	/**
	 * @fn
	 * �J�n
	*/
	void NetworkHost::start()
	{
		// ����ʐ���
		FrameWork::Game& game = FrameWork::Game::getInstance();
		Entity::ObjectsControl& objectsControl = game.objectsControl;

		// �t���[��
		shared_ptr<Entity::ModalFrame> frame = make_shared<ModalFrame>();
		frame->setColor(ColorType::MAIN_COLOR);
		frame->setShape(MODAL_X, MODAL_Y, MODAL_W, MODAL_H);
		frame->setTitle("���������");
		objectsControl.addObject(LAYER_FRAME, frame);

		//������IP�A�h���X
		shared_ptr<Text> ipText = make_shared<Text>("������IP�A�h���X : " + getIpAdress(), CONTENT_X, CONTENT_Y, FontType::NORMAL_S24, ColorType::SUB_COLOR_BIT_LITE);
		objectsControl.addFigure(LAYER_CONTENT, ipText);

		// �󋵃��b�Z�[�W
		statusText_ = make_shared<Text>(WAIT_CONNECT_TEXT + ".", CONTENT_X, CONTENT_Y + LINE_H + LINE_MARGIN, FontType::NORMAL_S24, ColorType::SUB_COLOR);
		objectsControl.addFigure(LAYER_CONTENT, statusText_);

		// �󋵃��b�Z�[�W�X�V�p�^�C�}�[
		textUpdataTimer_.setTimer(TIMER_MS);

		// ���[���ݒ�{�^��
		ruleSetButton_ = make_shared<TextButton>();
		ruleSetButton_->setShape(CONTENT_X, BUTTON_Y, BUTTON_W, BUTTON_H);
		ruleSetButton_->setText("���[���ݒ�", FontType::BLACK_S32);
		ruleSetButton_->setColor(ColorType::NEGATIVE_COLOR_LITE, ColorType::NEGATIVE_COLOR, TextButton::State::ALL);
		objectsControl.addObject(LAYER_CONTENT, ruleSetButton_);

		// �L�����Z���{�^��
		shared_ptr<TextButton> canceltButton = make_shared<TextButton>();
		canceltButton->setShape(CONTENT_X + BUTTON_W + BUTTON_MARGIN_X, BUTTON_Y, BUTTON_W, BUTTON_H);
		canceltButton->setText("�L�����Z��", FontType::BLACK_S32);
		canceltButton->setColor(ColorType::SUB_COLOR_LITE, ColorType::SUB_COLOR, TextButton::State::NORMAL);
		canceltButton->setColor(ColorType::SUB_COLOR, ColorType::MAIN_COLOR, TextButton::State::MOUSE_DOWN);
		canceltButton->setColor(ColorType::SUB_COLOR, ColorType::MAIN_COLOR, TextButton::State::MOUSE_OVER);
		objectsControl.addObject(LAYER_CONTENT, ContentId::CANCEL_BTN, canceltButton);

		// �ڑ��҂���Ԃɂ���
		DxLib::PreparationListenNetWork(PORT);
	}

	/**
	 * @fn
	 * �I��
	*/
	void NetworkHost::end()
	{
		isConnected_ = false;

		FrameWork::Game& game = FrameWork::Game::getInstance();
		Entity::ObjectsControl& objectsControl = game.objectsControl;

		// �S�v�f�폜
		objectsControl.removeObject(LAYER_FRAME);
		objectsControl.removeObject(LAYER_CONTENT);
		objectsControl.removeFigure(LAYER_CONTENT);
	}

	/**
	 * @fn
	 * �󋵎擾�y�эX�V����
	 * @param (hitObjWp) �ڐG�I�u�W�F�N�g�̎�Q��
	 * @param (x) �}�E�X��x���W
	 * @param (y) �}�E�X��y���W
	 * @param (button) �{�^���̎��
	 * @param (eventType) �C�x���g�̎�ށi�}�E�X�_�E�� or �}�E�X�A�b�v or �}�E�X�N���b�N�j
	 * @return �󋵂�Ԃ�
	*/
	int NetworkHost::checkAndUpdate(weak_ptr<Entity::Object> hitObjWp, int x, int y, int button, int eventType)
	{

		// �ڑ��҂�������
		if (!isConnected_)
		{
			checkAcceptNetwork();

			// �e�L�X�g�X�V
			if (textUpdataTimer_.checkAndCountTimer())
			{
				++dotCount_;
				if (dotCount_ > 3)
				{
					dotCount_ = 1;
				}

				string dotText = "";
				for (int i = 0; i < dotCount_; ++i)
				{
					dotText += ".";
					statusText_->setText(WAIT_CONNECT_TEXT + dotText);
				}
				textUpdataTimer_.setTimer(TIMER_MS);
			}
		}
		else // �ڑ��ς�
		{
			// ���M����Ă����f�[�^��`��i�e�X�g�j
			int dataLength = DxLib::GetNetWorkDataLength(netHandle_);
			if (dataLength > 0)
			{
				char strBuf[256];
				if (DxLib::NetWorkRecv(netHandle_, &strBuf, dataLength) == 0)
				{
					DxLib::printfDx(strBuf);
					// �e�X�g����
					DxLib::NetWorkSend(netHandle_, "�q���������`�I�I", 17);
				}
			}

			// �ؒf���ꂽ�ꍇ
			if (int lostHandle = GetLostNetWork() == netHandle_)
			{
				statusText_->setText("�N���C�A���g���ؒf\n��������蒼���Ă�������");
				DxLib::CloseNetWork(netHandle_);
			}
		}

		shared_ptr<Entity::Object> hitObjSp = hitObjWp.lock();

		if (hitObjSp)
		{
			// �{�^���̃N���b�N�C�x���g
			if (hitObjSp->getType() == Entity::Figure::BUTTON && eventType == MOUSE_INPUT_LOG_CLICK)
			{
				int objId = hitObjSp->getObjectId();

				if (objId == ContentId::CANCEL_BTN) // �L�����Z��
				{
					closeNetwork();
					end();
					Utility::ResourceManager::playSound(SoundKind::BACK);
					return Result::CANCEL;
				}
				else if (isConnected_ && hitObjSp == ruleSetButton_) // ���[���ݒ�
				{
					end();
					Utility::ResourceManager::playSound(SoundKind::CHECK);
					return Result::SET_RULE;
				}
			}
		}

		return Result::CONTINUE;
	}

	/**
	 * @fn
	 * �l�b�g���[�N�ڑ��̎�t
	*/
	void NetworkHost::checkAcceptNetwork()
	{
		// �ڑ����ꂽ��
		netHandle_ = DxLib::GetNewAcceptNetWork();
		if (netHandle_ != -1)
		{
			// �ڑ��̎�t���I������
			DxLib::StopListenNetWork();

			// �ڑ����Ă����}�V���̂h�o�A�h���X�𓾂�
			DxLib::GetNetWorkIP(netHandle_, &clientIp_);

			isConnected_ = true;

			statusText_->setText("�N���C�A���g�Ɛڑ�����");

			ruleSetButton_->setColor(ColorType::POSITIVE_LITE_COLOR, ColorType::POSITIVE_COLOR, Entity::TextButton::State::NORMAL);
			ruleSetButton_->setColor(ColorType::POSITIVE_COLOR, ColorType::POSITIVE_LITE_COLOR, Entity::TextButton::State::MOUSE_DOWN);
			ruleSetButton_->setColor(ColorType::POSITIVE_COLOR, ColorType::POSITIVE_LITE_COLOR, Entity::TextButton::State::MOUSE_OVER);
		}
	}

	/**
	 * @fn
	 * �l�b�g���[�N��ؒf
	*/
	void NetworkHost::closeNetwork()
	{
		// �ڑ�����
		if (!isConnected_)
		{
			DxLib::StopListenNetWork();
		}
		else
		{
			DxLib::CloseNetWork(netHandle_);
		}
	}
}