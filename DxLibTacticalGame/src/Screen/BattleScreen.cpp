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

		// �}�b�v�i�}�X�j
		shared_ptr<Entity::Map> map = make_shared<Entity::Map>();
		objectsControl.addObject(Layer::MAP, 0, map);

		// �s���I�����j���[
		shared_ptr<Entity::SelectActiveMenu> selectActiveMenu = make_shared<Entity::SelectActiveMenu>();
		objectsControl.addObject(Layer::UI, SELECT_ACTIVE_MENU, selectActiveMenu);

		// �o�g���Ǘ��N���X
		btlMng = Battle::BattleManager(map, selectActiveMenu);

		// ���j�b�g�ݒu(�e�X�g)
		shared_ptr<Entity::Unit> playerUnit = make_shared<Entity::Unit>();
		playerUnit->init(4, 9, false);
		if (btlMng.setUnit(playerUnit))
		{
			objectsControl.addObject(Layer::PLAYER_UNIT, 0, playerUnit);
		}

		shared_ptr<Entity::Unit> playerUnit2 = make_shared<Entity::Unit>();
		playerUnit2->init(5, 9, false);
		if (btlMng.setUnit(playerUnit2))
		{
			objectsControl.addObject(Layer::PLAYER_UNIT, 1, playerUnit2);
		}

		shared_ptr<Entity::Unit> enemy = make_shared<Entity::Unit>();
		enemy->init(4, 0, true);
		if (btlMng.setUnit(enemy))
		{
			objectsControl.addObject(Layer::ENEMY_UNIT, 0, enemy);
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
			Entity::ObjectsControl& objCont = FrameWork::Game::getInstance().objectsControl;

			if (nowScene_ == Scene::PLAYER_TURN) // �v���C���[�^�[��
			{
				if (eventType == MOUSE_INPUT_LOG_CLICK) // �N���b�N
				{
					if (hitObjSp->getLayerId() == Layer::MAP) // �}�b�v�N���b�N
					{
						btlMng.onClickMass(x, y);
					}
					else if (hitObjSp->getType() == Entity::Figure::UNIT) // ���j�b�g�N���b�N
					{
						btlMng.onClickUnit(x, y);
					}
					else if (hitObjSp->getLayerId() == Layer::UI) // UI����
					{
						if (hitObjSp->getObjectId() == UIid::SELECT_ACTIVE_MENU) // �s���I��
						{
							shared_ptr<Entity::SelectActiveMenu> menu = dynamic_pointer_cast<Entity::SelectActiveMenu>(hitObjSp);
							btlMng.onClickActionMenu(menu->getHitButtonKey(x, y));
						}
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
		btlMng.animationCheck();

		if (isOpenOverlayEnded())
		{
			nowScene_ = Scene::PLAYER_TURN;
		}
		isCloseOverlayEnded();
	}
}