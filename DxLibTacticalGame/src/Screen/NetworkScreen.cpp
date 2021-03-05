#include "NetworkScreen.h"

namespace Screen
{
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
	 * ��������
	*/
	void NetworkScreen::init()
	{
		FrameWork::Game& game = FrameWork::Game::getInstance();
		Entity::ObjectsControl& objectsControl = game.objectsControl;

		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		objectsControl.setLayer(Layer::LEN);

		// �w�i
		objectsControl.addObject(Layer::BACK, 0, make_shared<Entity::Back>());

		// ���������{�^��
		shared_ptr<Entity::TextButton> createRoomButton = make_shared<Entity::TextButton>();
		createRoomButton->setText("���������", FontType::BLACK_S48);
		createRoomButton->setShape(BUTTON_X, BUTTON_Y, BUTTON_W, BUTTON_H);
		createRoomButton->setColor(ColorType::MAIN_COLOR, ColorType::SUB_COLOR, Entity::TextButton::State::NORMAL);
		createRoomButton->setColor(ColorType::SUB_COLOR, ColorType::MAIN_COLOR, Entity::TextButton::State::MOUSE_DOWN);
		createRoomButton->setColor(ColorType::SUB_COLOR, ColorType::MAIN_COLOR, Entity::TextButton::State::MOUSE_OVER);

		objectsControl.addObject(Layer::UI, UIid::CREATE_ROOM_BUTTON, createRoomButton);

		// ������T���{�^��
		shared_ptr<Entity::TextButton> searchRoomButton = make_shared<Entity::TextButton>();
		searchRoomButton->setText("������T��", FontType::BLACK_S48);
		searchRoomButton->setShape(BUTTON_X, BUTTON_Y + BUTTON_H + BUTTON_MARGIN, BUTTON_W, BUTTON_H);
		searchRoomButton->setColor(ColorType::MAIN_COLOR, ColorType::SUB_COLOR, Entity::TextButton::State::NORMAL);
		searchRoomButton->setColor(ColorType::SUB_COLOR, ColorType::MAIN_COLOR, Entity::TextButton::State::MOUSE_DOWN);
		searchRoomButton->setColor(ColorType::SUB_COLOR, ColorType::MAIN_COLOR, Entity::TextButton::State::MOUSE_OVER);

		objectsControl.addObject(Layer::UI, UIid::SEARCH_ROOM_BUTTON, searchRoomButton);

		// IP�A�h���X���̓G���A
		shared_ptr<TextBox> nextInput; // ����IP�A�h���X���͗�
		for (int i = 3; i >= 0; --i)
		{
			shared_ptr<Entity::TextBox> ipInput = make_shared<Entity::TextBox>();
			ipInput->setShape(BUTTON_X + i * (100 + 10), BUTTON_Y + (BUTTON_H + BUTTON_MARGIN) * 2, 100, BUTTON_H);
			ipInput->setMaxLength(3);
			ipInput->setDataType(TextBox::DataType::NUM);
			objectsControl.addObject(Layer::UI, UIid::IP_INPUT_1 + i, ipInput);

			if (nextInput)
			{
				ipInput->setNextInput(nextInput);
			}
			nextInput = ipInput;
			ipAdressInputList_.push_back(ipInput);
		}


		// �߂�{�^��
		shared_ptr<Entity::TextButton> backBtn = make_shared<Entity::TextButton>(ColorType::MAIN_COLOR, ColorType::SUB_COLOR);
		backBtn->setShape(WIN_W - BACK_SIZE, 0, BACK_SIZE, BACK_SIZE);
		backBtn->setColor(ColorType::SUB_COLOR, ColorType::MAIN_COLOR, Entity::TextButton::State::MOUSE_OVER);
		backBtn->setColor(ColorType::SUB_COLOR, ColorType::MAIN_COLOR, Entity::TextButton::State::MOUSE_DOWN);
		backBtn->setText("�~", FontType::NORMAL_S32);
		objectsControl.addObject(Layer::UI, UIid::QUIT_BUTTON, backBtn);

		// IP�A�h���X�擾
		DxLib::printfDx(string("������IP�A�h���X" + getIpAdress()).c_str());

		// �I�[�o�[���C�Z�b�g
		createOverlay(true);
	}

	/**
	 * @fn
	 * �C�x���g�擾��̃X�N���[���X�V����
	 * @param (hitObjWp) �ڐG�I�u�W�F�N�g�̎�Q��
	 * @param (x) �}�E�X��x���W
	 * @param (y) �}�E�X��y���W
	 * @param (button) �{�^���̎��
	 * @param (eventType) �C�x���g�̎�ށi�}�E�X�_�E�� or �}�E�X�A�b�v or �}�E�X�N���b�N�j
	*/
	void NetworkScreen::updateByEvents(weak_ptr<Entity::Object> hitObjWp, int x, int y, int button, int eventType)
	{
		shared_ptr<Entity::Object> hitObjSp = hitObjWp.lock();

		// IP�A�h���X���X�g�̍X�V����
		for (auto itr = ipAdressInputList_.begin(); itr != ipAdressInputList_.end(); ++itr)
		{
			(*itr)->update();
		}

		if (hitObjSp)
		{
			// �{�^���̃N���b�N�C�x���g
			if (hitObjSp->getType() == Entity::Figure::BUTTON && eventType == MOUSE_INPUT_LOG_CLICK)
			{
				int objId = hitObjSp->getObjectId();

				if (objId == UIid::CREATE_ROOM_BUTTON) // �u���������v�{�^��
				{
					createRoom();
				}
				else if (objId == UIid::SEARCH_ROOM_BUTTON) // �u������T���v�{�^��
				{
					searchRoom();
				}
				else if (objId == UIid::QUIT_BUTTON) // �I���{�^��
				{
					// ��ʑJ��
					nextScreen_ = new MenuScreen();
					createOverlay(false);
				}
			}
		}
	}

	/**
	 * @fn
	 * �A�j���[�V�����I����̃X�N���[���X�V����
	*/
	void NetworkScreen::updateByAnimation()
	{
		isOpenOverlayEnded();

		if (isCloseOverlayEnded())
		{
			FrameWork::Game::getInstance().setScreen(nextScreen_); // ��ʑJ��
		}
		
	}

	/**
	 * @fn
	 * ���������
	*/
	void NetworkScreen::createRoom()
	{
		char StrBuf[256];        // �f�[�^�o�b�t�@
		int NetHandle, LostHandle;    // �l�b�g���[�N�n���h��
		int DataLength;            // ��M�f�[�^�ʕۑ��p�ϐ�
		IPDATA Ip;            // �ڑ���h�o�A�h���X�f�[�^

		// �ڑ����Ă���̂�҂�Ԃɂ���
		DxLib::PreparationListenNetWork(9850);

		// �ڑ����Ă��邩�d�r�b�L�[���������܂Ń��[�v
		NetHandle = -1;
		while (!ProcessMessage() && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
		{
			// �V�����ڑ����������炻�̃l�b�g���[�N�n���h���𓾂�
			NetHandle = GetNewAcceptNetWork();
			if (NetHandle != -1) break;
		}

		// �ڑ�����Ă����玟�ɐi��
		if (NetHandle != -1)
		{
			// �ڑ��̎�t���I������
			StopListenNetWork();

			// �ڑ����Ă����}�V���̂h�o�A�h���X�𓾂�
			GetNetWorkIP(NetHandle, &Ip);

			// �f�[�^�������ė���܂ő҂�
			while (!ProcessMessage())
			{
				// �擾���Ă��Ȃ���M�f�[�^�ʂ��O�ȊO�̂Ƃ��̓��[�v���甲����
				if (GetNetWorkDataLength(NetHandle) != 0) break;
			}

			// �f�[�^��M
			DataLength = GetNetWorkDataLength(NetHandle);    // �f�[�^�̗ʂ��擾
			NetWorkRecv(NetHandle, StrBuf, DataLength);    // �f�[�^���o�b�t�@�Ɏ擾

			// ��M�����f�[�^��`��
			DxLib::printfDx(StrBuf);

			// ��M�����̃f�[�^�𑗐M
			NetWorkSend(NetHandle, "�q���������`�I�I", 17);

			// ���肪�ʐM��ؒf����܂ő҂�
			while (!ProcessMessage())
			{
				// �V���ɐؒf���ꂽ�l�b�g���[�N�n���h���𓾂�
				LostHandle = GetLostNetWork();

				// �ؒf���ꂽ�ڑ������܂ŒʐM���Ă����肾�����ꍇ���[�v�𔲂���
				if (LostHandle == NetHandle) break;
			}
		}

		DxLib::printfDx("�ؒf���܂���");
	}

	/**
	 * @fn
	 * �����T��
	*/
	void NetworkScreen::searchRoom()
	{
		char StrBuf[256];    // �f�[�^�o�b�t�@
		IPDATA Ip;        // �ڑ��p�h�o�A�h���X�f�[�^
		int NetHandle;        // �l�b�g���[�N�n���h��
		int DataLength;        // ��M�f�[�^�ʕۑ��p�ϐ�

		// �h�o�A�h���X��ݒ�( �����ɂ���S�̂h�o�l�͉��ł� )
		Ip.d1 = 192;
		Ip.d2 = 168;
		Ip.d3 = 207;
		Ip.d4 = 203;

		// �ʐM���m��
		NetHandle = ConnectNetWork(Ip, 9850);

		// �m�������������ꍇ�̂ݒ��̏���������
		if (NetHandle != -1)
		{
			// �f�[�^���M
			NetWorkSend(NetHandle, "�q���������`�I�H", 17);

			// �f�[�^������̂�҂�
			while (!ProcessMessage())
			{
				// �擾���Ă��Ȃ���M�f�[�^�ʂ𓾂�
				DataLength = GetNetWorkDataLength(NetHandle);

				// �擾���ĂȂ���M�f�[�^�ʂ��O����Ȃ��ꍇ�̓��[�v�𔲂���
				if (DataLength != 0) break;
			}

			// �f�[�^��M
			NetWorkRecv(NetHandle, StrBuf, DataLength);    // �f�[�^���o�b�t�@�Ɏ擾

			// ��M�����f�[�^��`��
			DxLib::printfDx(StrBuf);

			// �L�[���͑҂�
			WaitKey();

			// �ڑ���f��
			CloseNetWork(NetHandle);
		}
	}
}