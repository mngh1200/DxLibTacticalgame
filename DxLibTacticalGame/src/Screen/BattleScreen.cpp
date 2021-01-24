#include "BattleScreen.h"

namespace Screen
{

	/**
	 * @fn
	 * ��������
	*/
	void BattleScreen::init()
	{
		FrameWork::Game& game = FrameWork::Game::getInstance();
		Entity::ObjectsControl& objectsControl = game.objectsControl;
		objectsControl.setLayer(Layer::LEN);

		// �w�i
		shared_ptr<Entity::Back> back = make_shared<Entity::Back>(Entity::Back::Screen::BATTLE);
		objectsControl.addObject(Layer::BACK, 0, back);

		// �V�X�e�����j��-
		systemMenu_ = make_shared<Entity::ContextMenu>();
		objectsControl.addObject(Layer::CONTEXT_MENU, 0, systemMenu_);

		systemMenu_->addMenuButton(SystemMenuKey::TURN_END, "�^�[���I��");
		systemMenu_->addMenuButton(SystemMenuKey::BACK_SELECT_SCREEN, "�Z���N�g��ʂɖ߂�");
		systemMenu_->addMenuButton(SystemMenuKey::BACK_MENU_SCREEN, "���j���[��ʂɖ߂�");

		// �}�b�v�i�}�X�j
		shared_ptr<Entity::Map> map = make_shared<Entity::Map>(MapId::STAGE1);
		objectsControl.addObject(Layer::MAP, 0, map);
		
		// �o�g���Ǘ��p�N���X�̏�������
		btlMng_.init(map);
		playerBtlCont_.init(map);
		enemyBtlCont_.init(map);

		// ���j�b�g�ݒu(�e�X�g)
		map->setUnit(4, 9, UnitKey::LANCER);
		map->setUnit(5, 9, UnitKey::GUNNER);
		map->setUnit(3, 9, UnitKey::CAVALRY);
		map->setUnit(4, 7, UnitKey::LANCER, true);
		map->setUnit(5, 7, UnitKey::GUNNER, true);
		map->setUnit(3, 7, UnitKey::CAVALRY, true);

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
	void BattleScreen::updateByEvents(weak_ptr<Entity::Object> hitObjWp, int x, int y, int button, int eventType)
	{
		shared_ptr<Entity::Object> hitObjSp = hitObjWp.lock();

		if (hitObjSp)
		{

			if (nowScene_ == Scene::PLAYER_TURN) // �v���C���[�^�[��
			{
				playerBtlCont_.updateByEvents(&btlMng_, hitObjSp, x, y, button, eventType);

				// �V�X�e�����j���[�֘A�C�x���g
				int systemMenuKey = systemMenu_->checkRunButton(x, y, eventType);

				if (systemMenuKey == SystemMenuKey::TURN_END)
				{
					// �^�[���G���h����
					turnEnd();
					systemMenu_->hide();
				}
				else if (systemMenuKey == SystemMenuKey::BACK_SELECT_SCREEN || systemMenuKey == SystemMenuKey::BACK_MENU_SCREEN)
				{
					// �����ʂɖ߂�
					openScreen_ = systemMenuKey;
					// Utility::ResourceManager::playSound(SoundKind::BACK);
					createOverlay(false);
				}
				else if (eventType == MOUSE_INPUT_LOG_UP || (eventType == MOUSE_INPUT_LOG_CLICK && hitObjSp != systemMenu_))
				{
					systemMenu_->hide();

					if (button == MOUSE_INPUT_RIGHT) // �E�}�E�X�_�E��
					{
						systemMenu_->show(x, y);
					}
				}
				
				
				if (eventType == MOUSE_INPUT_LOG_CLICK)
				{
					// �^�[���I���{�^��
					if (hitObjSp->getLayerId() == Layer::UI && hitObjSp->getObjectId() == Battle::BattleUI::BattleUIid::TURN_END_BUTTON)
					{
						turnEnd();
					}
				}
			}
		}
	}

	/**
	 * @fn
	 * �A�j���[�V�����I����̃X�N���[���X�V����
	*/
	void BattleScreen::updateByAnimation()
	{
		btlMng_.animationCheck(); // �o�g���Ǘ��n�̏���

		if (nowScene_ == Scene::ENEMY_TURN) // �G�^�[��
		{
			if (enemyBtlCont_.update(&btlMng_))
			{
				turnEnd();
			}
		}


		if (isOpenOverlayEnded()) // �I�[�o�[���C�J��
		{
			nowScene_ = Scene::PLAYER_TURN;
		}
		
		if (isCloseOverlayEnded()) // �I�[�o���C����
		{
			if (openScreen_ == SystemMenuKey::BACK_MENU_SCREEN)
			{
				// ���j���[��ʂɖ߂�
				FrameWork::Game::getInstance().setScreen(new MenuScreen());
			}
			else if (openScreen_ == SystemMenuKey::BACK_SELECT_SCREEN)
			{
				// �Z���N�g��ʂɖ߂�
				FrameWork::Game::getInstance().setScreen(new SelectScreen());
			}
		}
	}

	/**
	 * @fn
	 * �^�[���I�����i�ʃ^�[���ɐؑ֎��j
	*/
	void BattleScreen::turnEnd()
	{
		Utility::ResourceManager::playSound(SoundKind::CLICK);

		if (nowScene_ == Scene::PLAYER_TURN) // �v���C���[�^�[���I��
		{
			nowScene_ = Scene::ENEMY_TURN;
			btlMng_.onStartTurn(false);
		}
		else if (nowScene_ == Scene::ENEMY_TURN) // �G�^�[���I��
		{
			nowScene_ = Scene::PLAYER_TURN;
			btlMng_.onStartTurn(true);
		}
	}
}