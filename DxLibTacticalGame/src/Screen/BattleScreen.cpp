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
		shared_ptr<Entity::Map> map = make_shared<Entity::Map>();
		objectsControl.addObject(Layer::MAP, 0, map);
		
		// �o�g���Ǘ��N���X
		btlMng = Battle::BattleManager(map, Layer::UI);

		// ���j�b�g�ݒu(�e�X�g)
		shared_ptr<Entity::Unit> playerUnit = make_shared<Entity::Unit>();
		playerUnit->init(4, 9, UnitKey::LANCER);
		if (map->setUnit(playerUnit))
		{
			objectsControl.addObject(Layer::PLAYER_UNIT, 0, playerUnit);
		}

		shared_ptr<Entity::Unit> playerUnit2 = make_shared<Entity::Unit>();
		playerUnit2->init(5, 9, UnitKey::GUNNER);
		if (map->setUnit(playerUnit2))
		{
			objectsControl.addObject(Layer::PLAYER_UNIT, 1, playerUnit2);
		}

		shared_ptr<Entity::Unit> enemy = make_shared<Entity::Unit>();
		enemy->init(4, 7, UnitKey::LANCER, true);
		if (map->setUnit(enemy))
		{
			objectsControl.addObject(Layer::ENEMY_UNIT, 0, enemy);
		}

		shared_ptr<Entity::Unit> enemy2 = make_shared<Entity::Unit>();
		enemy2->init(5, 7, UnitKey::LANCER, true);
		if (map->setUnit(enemy2))
		{
			objectsControl.addObject(Layer::ENEMY_UNIT, 1, enemy2);
		}
		
		

		// �I�[�o�[���C�Z�b�g
		createOverlay(true);
	}

	/**
	 * @fn
	 * �C�x���g�擾��̃X�N���[���X�V����
	*/
	void BattleScreen::updateByEvents(weak_ptr<Entity::Object> hitObjWp, int x, int y, int button, int eventType)
	{
		shared_ptr<Entity::Object> hitObjSp = hitObjWp.lock();

		if (hitObjSp)
		{

			if (nowScene_ == Scene::PLAYER_TURN) // �v���C���[�^�[��
			{
				btlMng.updateByEvents(hitObjSp, x, y, button, eventType);

				// �V�X�e�����j���[�֘A�C�x���g
				int systemMenuKey = systemMenu_->checkRunButton(x, y, eventType);

				if (systemMenuKey == SystemMenuKey::TURN_END)
				{
					// �^�[���G���h����
					systemMenu_->hide();
				}
				else if (systemMenuKey == SystemMenuKey::BACK_SELECT_SCREEN || systemMenuKey == SystemMenuKey::BACK_MENU_SCREEN)
				{
					// �����ʂɖ߂�
					openScreen_ = systemMenuKey;
					createOverlay(false);
				}

				if (eventType == MOUSE_INPUT_LOG_UP || (eventType == MOUSE_INPUT_LOG_CLICK && hitObjSp != systemMenu_))
				{
					systemMenu_->hide();

					if (button == MOUSE_INPUT_RIGHT) // �E�}�E�X�_�E��
					{
						systemMenu_->show(x, y);
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
		btlMng.animationCheck(); // �o�g���Ǘ��n�̏���

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
}