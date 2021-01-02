#include "BattleManager.h"

namespace Battle {
	/**
	 * @fn
	 * �R���X�g���N�^
	 */
	BattleManager::BattleManager(shared_ptr<Entity::Map> map, int uiLayerId) : BattleManager()
	{
		map_ = map;
		battleUI_.init(uiLayerId);
		fight_.init(map);
	}

	/**
	 * @fn
	 * �C�x���g����
	 */
	void BattleManager::updateByEvents(shared_ptr<Object> hitObj, int x, int y, int button, int eventType)
	{
		if (eventType == MOUSE_INPUT_LOG_CLICK) // �N���b�N
		{
			if (hitObj->getType() == Object::Type::UNIT)
			{
				onClickUnit(x, y);
			}
			else if (hitObj->getType() == Object::Type::MAP)
			{
				onClickMass(x, y);
			}
		}

		if (hitObj->getType() == Object::Type::UNIT || hitObj->getType() == Object::Type::MAP)
		{
			// 1�}�b�v��̗v�f���z�o�[
			int massX = Map::getMassX(x);
			int massY = Map::getMassY(y);
			shared_ptr<Mass> mass = map_->getMass(massX, massY);
			shared_ptr<Unit> unit = map_->getUnit(massX, massY);

			if (mass)
			{
				if (phase_ == Phase::SELECT_ACTION && unit && unit->isEnemy() && mass->state == Mass::ATK_ABLE)
				{
					// �퓬�\���\��
					if (fight_.setPrepare(selectedUnit_.lock(), unit))
					{
						battleUI_.setFightPredict(&fight_);
					}
				}
				else
				{
					if (phase_ != Phase::FIGHT)
					{
						fight_.reset();
						battleUI_.resetFightPredict();
					}

					battleUI_.setTargetMass(mass);
				}
			}
		}
		else
		{
			if (phase_ != Phase::FIGHT)
			{
				fight_.reset();
				battleUI_.resetFightPredict();
			}
		}

	}


	/**
	 * @fn
	 * ���j�b�g �N���b�N������
	 */
	void BattleManager::onClickUnit(int x, int y)
	{
		int massX = Map::getMassX(x); // �}�X��X���W
		int massY = Map::getMassY(y); // �}�X��Y���W
		shared_ptr<Entity::Unit> unit = map_->getUnit(massX, massY); // �N���b�N�������j�b�g
		bool isEnemy = false;
		
		if (unit)
		{
			isEnemy = unit->isEnemy(); // �G���j�b�g�ł��邩
		}
		

		if (phase_ == Phase::SELECT_ACTION) // �s���I��
		{

			if (unit && isEnemy && map_->getMass(massX, massY)->state == Mass::ATK_ABLE)
			{
				atackAction(selectedUnit_.lock(), unit); // �U��
			}
			else
			{
				shared_ptr<Unit> selectedUnit = selectedUnit_.lock();
				if (selectedUnit)
				{
					if (selectedUnit->getMassX() == massX && selectedUnit->getMassY() == massY) // �I�𒆂̃��j�b�g�N���b�N
					{
						map_->confirmMove(selectedUnit); // �ҋ@
					}
				}
				endSelectActionPhase(); // �s���I���I��
			}
			

			if (!isEnemy && selectedUnit_.expired())
			{
				selectUnit(unit); // ���̃��j�b�g�I��
			}
		}
		else if (!isEnemy) // �v���C���[���j�b�g
		{
			if (unit == selectedUnit_.lock()) // �N���b�N�������j�b�g�� �I�𒆂̃��j�b�g�������ꍇ
			{
				startSelectActionPhase(); // �s���I���t�F�C�Y
			}
			else
			{
				selectUnit(unit); // ���j�b�g�I��
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
		int massX = Map::getMassX(x);
		int massY = Map::getMassY(y);

		shared_ptr<Entity::Unit> selectedUnitSp = selectedUnit_.lock();
		if (selectedUnitSp)
		{

			shared_ptr<Mass> targetMass = map_->getMass(massX, massY);
			
			if (phase_ == Phase::SELECT_ACTION) // �s���I��
			{
				selectedUnitSp->back(); // �ړ��L�����Z��
				endSelectActionPhase(); // �s���I���I��
			}
			else if (targetMass->isMovable())
			{
				selectedUnitSp->move(massX, massY); // �ړ�
				phase_ = Phase::MOVE;
				return;
			}
			else
			{
				deselectUnit(); // �I������
			}
		}
	}

	/**
	 * @fn
	 * �A�j���[�V���������`�F�b�N
	*/
	void BattleManager::animationCheck()
	{
		if (phase_ == Phase::MOVE) // �ړ�
		{
			shared_ptr<Unit> selectedUnit = selectedUnit_.lock();
			if (selectedUnit && !selectedUnit->isAnimation()) // �ړ��I��
			{
				startSelectActionPhase(); // �s���I���t�F�C�Y
			}
		}
		else if (phase_ == Phase::FIGHT && fight_.checkUpdate()) // �U��
		{
			// �U���I��
			phase_ = Phase::NORMAL;
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
		map_->displayAtackRange(selectedUnit_.lock());
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
	}

	/**
	 * @fn
	 * ���j�b�g�I��
	*/
	void BattleManager::selectUnit(shared_ptr<Unit> unit)
	{
		if (deselectUnit()) // �I������
		{
			// ���j�b�g�I��
			if (unit && unit->select(true))
			{
				selectedUnit_ = unit;
				battleUI_.setTargetUnit(selectedUnit_.lock());
				map_->displayMovableRange(unit);
			}
		}
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
				battleUI_.resetTargetUnit();
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
	 * �U��
	*/
	void BattleManager::atackAction(shared_ptr<Unit> atkUnit, shared_ptr<Unit> defUnit)
	{
		map_->confirmMove(atkUnit);
		map_->clearMassState();
		deselectUnit();

		if (atkUnit && defUnit)
		{
			fight_.start();
			phase_ = Phase::FIGHT;
		}
	}

}