#include "NetworkScreen.h"

namespace Screen
{
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
		createRoomButton->setColor(ColorType::SUB_COLOR_LITE, ColorType::SUB_COLOR, Entity::TextButton::State::NORMAL);
		createRoomButton->setColor(ColorType::SUB_COLOR, ColorType::MAIN_COLOR, Entity::TextButton::State::MOUSE_DOWN);
		createRoomButton->setColor(ColorType::SUB_COLOR, ColorType::MAIN_COLOR, Entity::TextButton::State::MOUSE_OVER);

		objectsControl.addObject(Layer::UI, UIid::CREATE_ROOM_BUTTON, createRoomButton);

		// ������T���{�^��
		shared_ptr<Entity::TextButton> searchRoomButton = make_shared<Entity::TextButton>();
		searchRoomButton->setText("������T��", FontType::BLACK_S48);
		searchRoomButton->setShape(BUTTON_X, BUTTON_Y + BUTTON_H + BUTTON_MARGIN, BUTTON_W, BUTTON_H);
		searchRoomButton->setColor(ColorType::SUB_COLOR_LITE, ColorType::SUB_COLOR, Entity::TextButton::State::NORMAL);
		searchRoomButton->setColor(ColorType::SUB_COLOR, ColorType::MAIN_COLOR, Entity::TextButton::State::MOUSE_DOWN);
		searchRoomButton->setColor(ColorType::SUB_COLOR, ColorType::MAIN_COLOR, Entity::TextButton::State::MOUSE_OVER);

		objectsControl.addObject(Layer::UI, UIid::SEARCH_ROOM_BUTTON, searchRoomButton);

		// ����
		shared_ptr<Text> commentText = make_shared<Text>(("�� �|�[�g�ԍ��u" + to_string(PORT) + "�v���J�����Ă����K�v������܂�").c_str(),
			BUTTON_X, BUTTON_Y + (BUTTON_H + BUTTON_MARGIN) * 2, FontType::NORMAL_S24, ColorType::SUB_COLOR);
		objectsControl.addFigure(Layer::UI, commentText);


		// �߂�{�^��
		shared_ptr<Entity::TextButton> backBtn = make_shared<Entity::TextButton>(ColorType::MAIN_COLOR, ColorType::SUB_COLOR);
		backBtn->setShape(WIN_W - BACK_SIZE, 0, BACK_SIZE, BACK_SIZE);
		backBtn->setColor(ColorType::SUB_COLOR, ColorType::MAIN_COLOR, Entity::TextButton::State::MOUSE_OVER);
		backBtn->setColor(ColorType::SUB_COLOR, ColorType::MAIN_COLOR, Entity::TextButton::State::MOUSE_DOWN);
		backBtn->setText("�~", FontType::NORMAL_S32);
		objectsControl.addObject(Layer::UI, UIid::QUIT_BUTTON, backBtn);

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

		if (nowScene_ == Scene::HOST) // ���������i����ʕ\���������j
		{
			int result = hostManager_.checkAndUpdate(hitObjWp, x, y, button, eventType);

			if (result == NetworkHost::Result::CANCEL) // �L�����Z��
			{
				nowScene_ = Scene::INIT;
			}
			else if (result == NetworkHost::Result::SET_RULE) // ���[���ݒ�
			{
				nowScene_ = Scene::RULE_SET;
				ruleSetting_.start(hostManager_.getNetHandle());
			}
			return;
		}
		if (nowScene_ == Scene::RULE_SET) // ���[���ݒ�i����ʕ\���������j
		{
			int result = ruleSetting_.checkAndUpdate(hitObjWp, x, y, button, eventType);

			if (result == RuleSetting::Result::CANCEL) // ���� or �ؒf
			{
				nowScene_ = Scene::INIT;
			}
			else if (result == RuleSetting::Result::START_BATTLE) // �o�g���J�n
			{
				startBattle(ruleSetting_.getNetHandle(), ruleSetting_.getRuleData(), true);
			}
			return;
		}
		else if (nowScene_ == Scene::CLIENT) // ������T���i����ʕ\���������j
		{
			int result = clientManager_.checkAndUpdate(hitObjWp, x, y, button, eventType);

			if (result == NetworkClient::Result::CANCEL)
			{
				nowScene_ = Scene::INIT;
			}
			else if (result == NetworkClient::Result::START_BATTLE)
			{
				startBattle(clientManager_.getNetHandle(), clientManager_.getRuleData(), false);
			}
			return;
		}

		// �C�x���g�ΏۃI�u�W�F�N�g���Ȃ��ꍇ�A�I��
		if (!hitObjSp)
		{
			return;
		}

		// �{�^���̃N���b�N�C�x���g
		if (hitObjSp->getType() == Entity::Figure::BUTTON && eventType == MOUSE_INPUT_LOG_CLICK)
		{
			int objId = hitObjSp->getObjectId();

			if (nowScene_ == Scene::INIT)
			{
				if (objId == UIid::CREATE_ROOM_BUTTON) // �u���������v�{�^��
				{
					Utility::ResourceManager::playSound(SoundKind::CHECK);
					nowScene_ = Scene::HOST;
					hostManager_.start();
				}
				else if (objId == UIid::SEARCH_ROOM_BUTTON) // �u������T���v�{�^��
				{
					Utility::ResourceManager::playSound(SoundKind::CHECK);
					nowScene_ = Scene::CLIENT;
					clientManager_.start();
				}
				else if (objId == UIid::QUIT_BUTTON) // �I���{�^��
				{
					// ��ʑJ��
					Utility::ResourceManager::playSound(SoundKind::BACK);
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
		if (isOpenOverlayEnded())
		{
			nowScene_ = Scene::INIT;
		}

		if (isCloseOverlayEnded())
		{
			FrameWork::Game::getInstance().setScreen(nextScreen_); // ��ʑJ��
		}
		
	}

	/**
	 * @fn
	 * �o�g����ʂւ̑J�ڏ���
	 * @param (ruleData) ���[���ݒ�f�[�^
	*/
	void NetworkScreen::startBattle(int netHandler_, const RuleData& ruleData, bool isServer)
	{
		BattleScreen* bs = new BattleScreen();
		bs->prepareNetMatch(netHandler_, isServer, ruleData.mapId, ruleData.unitNum, isServer != ruleData.isClientFirst);
		nextScreen_ = bs;

		createOverlay(false);
	}

}