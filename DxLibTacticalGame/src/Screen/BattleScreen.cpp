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
		objectsControl.addObject(Layer::MAP, 0, make_shared<Entity::Map>());

		// ���j�b�g�ݒu(�e�X�g)
		shared_ptr<Entity::Unit> playerUnit = make_shared<Entity::Unit>();
		playerUnit->init(4, 9, false);
		objectsControl.addObject(Layer::PLAYER_UNIT, 0, playerUnit);

		shared_ptr<Entity::Unit> enemy = make_shared<Entity::Unit>();
		enemy->init(4, 0, true);
		objectsControl.addObject(Layer::ENEMY_UNIT, 0, enemy);

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

			if (nowScene_ == Scene::PLAYER_TURN)
			{
				if (eventType == MOUSE_INPUT_LOG_CLICK)
				{
					if (hitObjSp->getLayerId() == Layer::MAP) // �}�b�v�N���b�N
					{
						shared_ptr<Entity::Map> map = dynamic_pointer_cast<Entity::Map>(hitObjSp);

						shared_ptr<Entity::Unit> selectedUnitSp = selectedUnit_.lock();
						if (selectedUnitSp)
						{
							// �ړ�
							selectedUnitSp->move(Entity::Map::getMassX(x), Entity::Map::getMassY(y));
						}

					}
					else if (hitObjSp->getLayerId() == Layer::PLAYER_UNIT) // ���j�b�g�N���b�N
					{
						if (deselectUnit())
						{
							// ���j�b�g�I��
							shared_ptr<Entity::Unit> unit = dynamic_pointer_cast<Entity::Unit>(hitObjSp);
							if (unit->select(true))
							{
								selectedUnit_ = unit;
							}
						}
					}
					else
					{
						deselectUnit();
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
		if (isOpenOverlayEnded())
		{
			nowScene_ = Scene::PLAYER_TURN;
		}
		isCloseOverlayEnded();
	}

	/**
	 * @fn
	 * ���j�b�g�̑I����Ԃ�����
	 * @return �����ł����ꍇ�ƑI���ς݂̃��j�b�g���Ȃ��ꍇ��true��Ԃ�
	*/
	bool BattleScreen::deselectUnit()
	{
		shared_ptr<Entity::Unit> prevSelectedUnit = selectedUnit_.lock();
		if (prevSelectedUnit)
		{
			if (prevSelectedUnit->select(false))
			{
				selectedUnit_.reset();
				return true;
			}
			return false;
		}

		// �I���ς݂̃��j�b�g���Ȃ��ꍇ��ture��Ԃ�
		return true;
	}

}