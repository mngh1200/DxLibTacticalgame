#include "NetworkClient.h"
#include "Screen/NetworkScreen.h"

namespace Network
{
	const int NetworkClient::LAYER_CONTENT = Screen::NetworkScreen::Layer::MODAL_CONTENT;
	const int NetworkClient::LAYER_FRAME = Screen::NetworkScreen::Layer::MODAL_FRAME;

	/**
	 * @fn
	 * �J�n
	*/
	void NetworkClient::start()
	{
		// ����ʐ���
		FrameWork::Game& game = FrameWork::Game::getInstance();
		Entity::ObjectsControl& objectsControl = game.objectsControl;

		// �t���[��
		shared_ptr<ModalFrame> frame = make_shared<Entity::ModalFrame>();
		frame->setColor(ColorType::MAIN_COLOR);
		frame->setShape(MODAL_X, MODAL_Y, MODAL_W, MODAL_H);
		frame->setTitle("������T��");
		objectsControl.addObject(LAYER_FRAME, frame);

		// �e�L�X�g
		objectsControl.addFigure(LAYER_CONTENT, make_shared<Text>("�ڑ����IP�A�h���X����͂��Ă�������",
			CONTENT_X, CONTENT_Y, FontType::NORMAL_S24, ColorType::SUB_COLOR_BIT_LITE));

		// IP�A�h���X���̓G���A
		shared_ptr<TextBox> nextInput; // ����IP�A�h���X���͗�
		for (int i = 3; i >= 0; --i)
		{
			shared_ptr<Entity::TextBox> ipInput = make_shared<Entity::TextBox>();
			int x = CONTENT_X + i * (IP_INPUT_W + IP_INPUT_MARGIN);
			ipInput->setShape(x, IP_INPUT_Y, IP_INPUT_W, IP_INPUT_H);
			ipInput->setMaxLength(3);
			ipInput->setDataType(TextBox::DataType::NUM);
			ipInput->setFont(FontType::NORMAL_S24);
			ipInput->setPaddingLeft(16);

			// �f�t�H���g
			if (i == 0)
			{
				ipInput->setText("192");
			}
			else if (i == 1)
			{
				ipInput->setText("168");
			}

			objectsControl.addObject(LAYER_CONTENT, ipInput);

			if (nextInput)
			{
				ipInput->setNextInput(nextInput);
			}
			nextInput = ipInput;
			ipAdressInputList_.push_back(ipInput);

			// IP�A�h���X�̊Ԃ̃h�b�g
			if (i == 3)
			{
				continue;
			}

			objectsControl.addFigure(LAYER_CONTENT, make_shared<Text>(".", x + IP_INPUT_W + IP_INPUT_MARGIN / 3, IP_INPUT_Y + 20, FontType::NORMAL_S24, ColorType::SUB_COLOR));
		}

		// �󋵃��b�Z�[�W
		statusText_ = make_shared<Text>("", CONTENT_X, BUTTON_Y - STATUS_LINE_H * 2, FontType::NORMAL_S24, ColorType::SUB_COLOR);
		objectsControl.addFigure(LAYER_CONTENT, statusText_);

		// �ڑ��{�^��
		connectButton_ = make_shared<TextButton>();
		connectButton_->setShape(CONTENT_X, BUTTON_Y, BUTTON_W, BUTTON_H);
		connectButton_->setText("�ڑ�", FontType::BLACK_S32);
		objectsControl.addObject(LAYER_CONTENT, connectButton_);

		adjustConnectButtonValid();

		// �L�����Z���{�^��
		shared_ptr<TextButton> canceltButton = make_shared<TextButton>();
		canceltButton->setShape(CONTENT_X + BUTTON_MARGIN_X + BUTTON_W, BUTTON_Y, BUTTON_W, BUTTON_H);
		canceltButton->setText("�L�����Z��", FontType::BLACK_S32);
		canceltButton->setColor(ColorType::SUB_COLOR_LITE, ColorType::SUB_COLOR, TextButton::State::NORMAL);
		canceltButton->setColor(ColorType::SUB_COLOR, ColorType::MAIN_COLOR, TextButton::State::MOUSE_DOWN);
		canceltButton->setColor(ColorType::SUB_COLOR, ColorType::MAIN_COLOR, TextButton::State::MOUSE_OVER);
		objectsControl.addObject(LAYER_CONTENT, ContentId::CANCEL_BTN, canceltButton);

	}

	/**
	 * @fn
	 * �I��
	*/
	void NetworkClient::end()
	{
		state_ = State::NONE;

		FrameWork::Game& game = FrameWork::Game::getInstance();
		Entity::ObjectsControl& objectsControl = game.objectsControl;

		// �S�v�f�폜
		objectsControl.removeObject(LAYER_FRAME);
		objectsControl.removeObject(LAYER_CONTENT);
		objectsControl.removeFigure(LAYER_CONTENT);

		// �ڑ�����
		DxLib::CloseNetWork(netHandle_);
	}

	/**
	 * @fn
	 * �X�V����
	 * @param (hitObjWp) �ڐG�I�u�W�F�N�g�̎�Q��
	 * @param (x) �}�E�X��x���W
	 * @param (y) �}�E�X��y���W
	 * @param (button) �{�^���̎��
	 * @param (eventType) �C�x���g�̎�ށi�}�E�X�_�E�� or �}�E�X�A�b�v or �}�E�X�N���b�N�j
	 * @return �󋵂�Ԃ�
	*/
	int NetworkClient::checkAndUpdate(weak_ptr<Entity::Object> hitObjWp, int x, int y, int button, int eventType)
	{
		// IP�A�h���X���X�g�̍X�V����
		bool isChange = false; //! �ύX����������
		for (auto itr = ipAdressInputList_.begin(); itr != ipAdressInputList_.end(); ++itr)
		{
			if ((*itr)->checkChangeAndUpdate())
			{
				isChange = true;
			}
		}

		if (isChange) // �ύX���������ꍇ�A�ڑ��{�^���̏�Ԃ𒲐�
		{
			adjustConnectButtonValid();
		}

		// �ڑ��ς݂̂Ƃ�
		if (state_ == State::CONNECTED)
		{
			// �擾���Ă��Ȃ���M�f�[�^�ʂ𓾂�(�e�X�g)
			int dataLength = DxLib::GetNetWorkDataLength(netHandle_);
			if (dataLength > 0)
			{
				// �f�[�^��M
				char strBuf[256];
				DxLib::NetWorkRecv(netHandle_, &strBuf, dataLength);    // �f�[�^���o�b�t�@�Ɏ擾
				DxLib::printfDx(strBuf);
			}

			// �ؒf���ꂽ�ꍇ
			if (int lostHandle = DxLib::GetLostNetWork() == netHandle_)
			{
				statusText_->setText("�z�X�g����ؒf����܂���\n�Đڑ����Ă�������");
				setState(State::NONE);
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
					Utility::ResourceManager::playSound(SoundKind::BACK);
					end();
					return Result::CANCEL;
				}
				
				// �������
				if (state_ == State::NONE)
				{
					if (hitObjSp == connectButton_ && ableConnecButton_) // �ڑ�
					{
						Utility::ResourceManager::playSound(SoundKind::CHECK);
						connectNetwork();
						return Result::CONTINUE;
					}
				}

			}
		}

		// �L�[�C�x���g
		FrameWork::Controller& cont = FrameWork::Controller::getInstance();
		if (cont.isKeyPressedNow(KEY_INPUT_RETURN) && ableConnecButton_) // �ڑ��\�Ȏ��ɃG���^�[�L�[�����������ꍇ
		{
			connectNetwork();
		}

		return Result::CONTINUE;
	}


	/**
	 * @fn
	 * �󋵂�ύX
	 * @param (state) ��
	*/
	void NetworkClient::setState(State state)
	{
		state_ = state;
		adjustConnectButtonValid();
	}

	/**
	 * @fn
	 * �ڑ��{�^���̗L��������Ԃ𒲐�
	*/
	void NetworkClient::adjustConnectButtonValid()
	{
		ableConnecButton_ = false; // �L�� / ����

		if (state_ == State::NONE) // �������
		{
			bool isBlank = false; // �󗓂̍��ڂ����邩
			for (auto itr = ipAdressInputList_.begin(); itr != ipAdressInputList_.end(); ++itr)
			{
				string text;
				(*itr)->getText(text);
				if (text == "")
				{
					isBlank = true;
					break;
				}
			}

			ableConnecButton_ = !isBlank;
		}
		
		if (!ableConnecButton_) // ����
		{
			connectButton_->setColor(ColorType::NEGATIVE_COLOR_LITE, ColorType::NEGATIVE_COLOR, TextButton::State::ALL);
		}
		else // �L��
		{
			connectButton_->setColor(ColorType::POSITIVE_LITE_COLOR, ColorType::POSITIVE_COLOR, Entity::TextButton::State::NORMAL);
			connectButton_->setColor(ColorType::POSITIVE_COLOR, ColorType::POSITIVE_LITE_COLOR, Entity::TextButton::State::MOUSE_DOWN);
			connectButton_->setColor(ColorType::POSITIVE_COLOR, ColorType::POSITIVE_LITE_COLOR, Entity::TextButton::State::MOUSE_OVER);
		}
	}

	/**
	 * @fn
	 * �l�b�g���[�N�ɐڑ�
	*/
	void NetworkClient::connectNetwork()
	{
		// �ʐM���m��
		IPDATA ipData;
		getIpData(ipData);
		netHandle_ = DxLib::ConnectNetWork(ipData, PORT);

		// �m�������������ꍇ�̂ݒ��̏���������
		if (netHandle_ != -1)
		{
			statusText_->setText("�z�X�g�ɐڑ��ł��܂���\n�z�X�g�����[���ݒ蒆�ł�");

			// �f�[�^���M
			DxLib::NetWorkSend(netHandle_, "�q���������`�I�H", 17);

			setState(State::CONNECTED);
		}
		else // ���s
		{
			statusText_->setText("�z�X�g�ւ̐ڑ��Ɏ��s���܂���");
		}
	}

	/**
	 * @fn
	 * IP�A�h���X���擾
	 * @param (ipData) IP�A�h���X�̎Q�Ɠn���p
	*/
	void NetworkClient::getIpData(IPDATA& ipData)
	{
		string text;
		ipAdressInputList_.at(3)->getText(text);
		ipData.d1 = atoi(text.c_str());

		ipAdressInputList_.at(2)->getText(text);
		ipData.d2 = atoi(text.c_str());

		ipAdressInputList_.at(1)->getText(text);
		ipData.d3 = atoi(text.c_str());

		ipAdressInputList_.at(0)->getText(text);
		ipData.d4 = atoi(text.c_str());
	}
}