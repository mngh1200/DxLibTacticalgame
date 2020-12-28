#include "BattleManager.h"

namespace Battle {
	/**
	 * @fn
	 * �R���X�g���N�^
	 */
	BattleManager::BattleManager(shared_ptr<Entity::Map> map, shared_ptr<Entity::SelectActiveMenu> selectActiveMenu) : BattleManager()
	{
		map_ = map;
		selectActiveMenu_ = selectActiveMenu;
	}

	/**
	 * @fn
	 * �}�X�Ƀ��j�b�g�ւ̎Q�Ƃ��Z�b�g
	 * @param (unit) ���j�b�g�̎Q��
	 * @return �ǉ����� : true
	 */
	bool BattleManager::setUnit(shared_ptr<Entity::Unit> unit)
	{
		int x = unit->getMassX();
		int y = unit->getMassY();

		if (map_->isRange(x, y))
		{
			auto ret = mapUnits_.emplace(make_pair(x, y), unit); // �V�K�ǉ��̂�
			return ret.second;
		}
		return false;
	}


	/**
	 * @fn
	 * ���j�b�g �N���b�N������
	 */
	void BattleManager::onClickUnit(int x, int y)
	{
		int massX = Map::getMassX(x); // �}�X��X���W
		int massY = Map::getMassY(y); // �}�X��Y���W
		shared_ptr<Entity::Unit> unit = getUnitWp(massX, massY).lock(); // �N���b�N�������j�b�g
		bool isEnemy = false;
		
		if (unit)
		{
			isEnemy = unit->isEnemy(); // �G���j�b�g�ł��邩
		}
		

		if (phase_ == Phase::SELECT_ACTION) // �s���I��
		{
			if (isEnemy && map_->getMass(massX, massY)->state == Mass::ATK_ABLE)
			{
				// �U��
				atackAction(selectedUnit_.lock(), unit);
			}
		}
		else if (!isEnemy) // �v���C���[���j�b�g
		{
			if (unit == selectedUnit_.lock()) // �N���b�N�������j�b�g�� �I�𒆂̃��j�b�g�������ꍇ
			{
				startSelectActionPhase(); // �s���I���t�F�C�Y
			}
			else if (deselectUnit()) // �I������
			{
				// ���j�b�g�I��
				if (unit && unit->select(true))
				{
					selectedUnit_ = unit;
					displayMovableRange();
				}
			}
		}
		else // �G���j�b�g
		{

		}

	}

	/**
	 * @fn
	 * �}�X �N���b�N������
	 */
	void BattleManager::onClickMass(int x, int y)
	{
		shared_ptr<Entity::Unit> selectedUnitSp = selectedUnit_.lock();
		if (selectedUnitSp)
		{
			int massX = Map::getMassX(x);
			int massY = Map::getMassY(y);
			shared_ptr<Mass> targetMass = map_->getMass(massX, massY);
			
			if (targetMass->isMovable())
			{
				selectedUnitSp->move(massX, massY); // �ړ�
				phase_ = Phase::MOVE;
			}
			else
			{
				deselectUnit(); // �I������
			}
		}
	}

	/**
	 * @fn
	 * �s���I��
	*/
	void BattleManager::onClickActionMenu(int kind)
	{
		if (kind == -1)
		{
			return;
		}

		shared_ptr<Unit> selectedUnit = selectedUnit_.lock();
		if (selectedUnit)
		{
			if (kind == SelectActiveMenu::ButtonKind::WAIT)
			{
				// �ҋ@
				confirmMove(selectedUnit);
			}
			else if (kind == SelectActiveMenu::ButtonKind::CANCEL)
			{
				// �L�����Z��
				selectedUnit->back();

			}
		}
		endSelectActionPhase(); // �s���I���I��
	}

	/**
	 * @fn
	 * �A�j���[�V���������`�F�b�N
	*/
	void BattleManager::animationCheck()
	{
		if (phase_ == Phase::MOVE)
		{
			shared_ptr<Unit> selectedUnit = selectedUnit_.lock();
			if (selectedUnit && !selectedUnit->isAnimation()) // �ړ��I��
			{
				startSelectActionPhase(); // �s���I���t�F�C�Y
			}
		}

	}

	/**
	 * @fn
	 * �s���I���t�F�C�Y�J�n
	*/
	void BattleManager::startSelectActionPhase()
	{
		map_->clearMassState();
		phase_ = Phase::SELECT_ACTION; // �s���I�� 
		displayAtackRange();

		shared_ptr<Entity::Unit> selectedUnit = selectedUnit_.lock();
		if (selectedUnit)
		{
			selectActiveMenu_->start(selectedUnit->getX(), selectedUnit->getY());
		}
		
	}

	/**
	 * @fn
	 * �s���I���I��
	*/
	void BattleManager::endSelectActionPhase()
	{
		map_->clearMassState();
		phase_ = Phase::NORMAL;
		deselectUnit();
		selectActiveMenu_->end();
	}

	/**
	 * @fn
	 * ���j�b�g�̑I����Ԃ�����
	 * @return �����ł����ꍇ�ƑI���ς݂̃��j�b�g���Ȃ��ꍇ��true��Ԃ�
	*/
	bool BattleManager::deselectUnit()
	{
		if (selectedUnit_.expired())
		{
			return true;
		}
		shared_ptr<Entity::Unit> prevSelectedUnit = selectedUnit_.lock();
		if (prevSelectedUnit)
		{
			if (prevSelectedUnit->select(false))
			{
				selectedUnit_.reset();
				map_->clearMassState();
				return true;
			}
			return false;
		}

		// �I���ς݂̃��j�b�g���Ȃ��ꍇ��ture��Ԃ�
		return true;
	}

	/**
	 * @fn
	 * �ړ��\�͈͕\��
	*/
	void BattleManager::displayMovableRange()
	{
		shared_ptr<Unit> unit = selectedUnit_.lock();
		if (unit)
		{
			int move = unit->getMove();
			int x = unit->getMassX();
			int y = unit->getMassY();
			searchMovableMass(x, y, move);
		}
	}

	/**
	 * @fn
	 * �ړ��\�͈͂̒T��
	*/
	void BattleManager::searchMovableMass(int x, int y, int move, bool isInit)
	{
		shared_ptr<Mass> nowMass = map_->getMass(x, y);

		// �}�b�v�O
		if (nowMass->getKind() == Mass::Kind::OUT_OF_MAP)
		{
			return;
		}

		shared_ptr<Unit> massUnit = getUnitWp(x, y).lock();
		bool isPlayerUnitOnMass = false; // �v���C���[���j�b�g���}�X��ɑ��݂��邩
		
		// �G���j�b�g������ꍇ
		if (massUnit)
		{
			if (massUnit->isEnemy())
			{
				return;
			}
			isPlayerUnitOnMass = true && !isInit; // ���g�̏ꍇ�͖���
		}

		// mov�R�X�g����(����̓R�X�g����Ȃ�)
		if (!isInit)
		{
			move = move - nowMass->getCost();
		}

		if (move > nowMass->passingMov)
		{
			if (!isPlayerUnitOnMass)
			{
				nowMass->state = Mass::State::MOVABLE;
			}

			// �}�X�ʉߎ���mov�R�X�g��ێ�
			nowMass->passingMov = move;

			searchMovableMass(x - 1, y, move, false);
			searchMovableMass(x + 1, y, move, false);
			searchMovableMass(x, y - 1, move, false);
			searchMovableMass(x, y + 1, move, false);
		}
	}

	/**
	 * @fn
	 * ���j�b�g�̃}�X�ړ��i�ړ��m�莞�j
	*/
	void BattleManager::confirmMove(shared_ptr<Unit> unit)
	{
		int baseX = unit->getBaseX();
		int baseY = unit->getBaseY();
		int massX = unit->getMassX();
		int massY = unit->getMassY();
		
		if (baseX != massX || baseY != massY) // �ړ����Ă���Ƃ��̂�
		{
			unit->setPos(massX, massY);
			mapUnits_.emplace(make_pair(massX, massY), unit);
			mapUnits_.erase(make_pair(baseX, baseY));
		}
	}


	/**
	 * @fn
	 * �U���͈͕\��
	*/
	void BattleManager::displayAtackRange()
	{
		shared_ptr<Unit> unit = selectedUnit_.lock();
		if (unit)
		{
			int move = unit->getMove();
			int x = unit->getMassX();
			int y = unit->getMassY();
			int range = unit->getRange();

			for (int i = 1; i <= range; i++)
			{
				setAtackMass(x - i, y);
				setAtackMass(x + i, y);
				setAtackMass(x, y - i);
				setAtackMass(x, y + i);
			}
		}
	}

	/**
	 * @fn
	 * �U���\�͈͂̒T��
	*/
	void BattleManager::setAtackMass(int x, int y)
	{
		shared_ptr<Mass> nowMass = map_->getMass(x, y);

		// �}�b�v�O
		if (nowMass->getKind() == Mass::Kind::OUT_OF_MAP)
		{
			return;
		}
		nowMass->state = Mass::ATK_ABLE;
	}


	/**
	 * @fn
	 * �U��
	*/
	void BattleManager::atackAction(shared_ptr<Unit> atkUnit, shared_ptr<Unit> defUnit)
	{
		confirmMove(atkUnit);

		if (atkUnit && defUnit)
		{
			int damage = atkUnit->getAtk();
			defUnit->damage(damage);
		}

		endSelectActionPhase(); // �e�X�g

	}

	/**
	 * @fn
	 * �}�X���W���烆�j�b�g�擾
	*/
	weak_ptr<Unit> BattleManager::getUnitWp(int massX, int massY)
	{
		pair<int,int> pos = make_pair(massX, massY);
		if (mapUnits_.count(pos) > 0)
		{
			return mapUnits_.at(pos);
		}

		return weak_ptr<Unit>();
	}

}