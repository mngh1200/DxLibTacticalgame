#include "BattleManager.h"
#include "Screen/BattleScreen.h"

namespace Battle {
	/**
	 * @fn
	 * �R���X�g���N�^
	 */
	BattleManager::BattleManager(shared_ptr<Entity::Map> map) : BattleManager()
	{
		map_ = map;
		battleUI_.init();
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
				onClickUnit(hitObj);
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
				if (isAtackAble(unit))
				{
					// �퓬�\���\��
					if (fight_.setPrepare(selectedUnit_, unit))
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
	void BattleManager::onClickUnit(shared_ptr<Object> hitObj)
	{
		shared_ptr<Unit> unit = dynamic_pointer_cast<Unit>(hitObj);
		bool isOwn = !unit->isEnemy(); // �������j�b�g�ł��邩
		

		if (isAtackAble(unit)) // �U���Ώۂ̃��j�b�g�N���b�N
		{
			atackAction(); // �U���A�N�V����

		}
		else if (phase_ == Phase::SELECT_ACTION) // �s���I��
		{
			if (unit == selectedUnit_) // �I�𒆂̃��j�b�g�N���b�N
			{
				waitAction(); // �ҋ@�A�N�V����
			}
			else
			{
				endSelectActionPhase(); // �s���I���I��
				selectUnit(unit); // ���̃��j�b�g�I��
			}
		}
		else
		{
			if (unit == selectedUnit_)
			{
				if (isOwn)
				{
					// �N���b�N�������j�b�g�� �I�𒆂̃��j�b�g�������ꍇ�A�s���I���t�F�C�Y�Ɉڍs
					startSelectActionPhase();
				}
			}
			else
			{
				selectUnit(unit); // ���j�b�g�I��
			}
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

		if (selectedUnit_)
		{

			shared_ptr<Mass> targetMass = map_->getMass(massX, massY);
			
			if (phase_ == Phase::SELECT_ACTION) // �s���I��
			{
				selectedUnit_->back(); // �ړ��L�����Z��
				endSelectActionPhase(); // �s���I���I��
			}
			else if (targetMass->isMovable())
			{
				selectedUnit_->move(massX, massY); // �ړ�
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
			if (selectedUnit_ && !selectedUnit_->isAnimation()) // �ړ��I��
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
	 * �^�[���X�^�[�g������
	*/
	void BattleManager::onStartTurn(bool isPlayer)
	{
		FrameWork::Game& game = FrameWork::Game::getInstance();

		for (auto itr = map_->unitsBegin(); itr != map_->unitsEnd(); ++itr)
		{
			itr->second->turnEnd();
		}
	}

	/**
	 * @fn
	 * �s���I���t�F�C�Y�J�n
	*/
	void BattleManager::startSelectActionPhase()
	{
		if (!selectedUnit_->isEnemy() && !selectedUnit_->isActed())
		{
			map_->clearMassState();
			phase_ = Phase::SELECT_ACTION; // �s���I�� 
			map_->displayAtackRange(selectedUnit_);
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
				battleUI_.setTargetUnit(selectedUnit_);

				if (!unit->isActed())
				{
					map_->displayMovableRange(unit);
				}
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
		if (!selectedUnit_)
		{
			return true;
		}

		if (selectedUnit_)
		{
			if (selectedUnit_->select(false))
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
	void BattleManager::atackAction()
	{
		map_->confirmMove(selectedUnit_);
		fight_.start();
		endSelectActionPhase();
		phase_ = Phase::FIGHT;
	}

	/**
	 * @fn
	 * �ҋ@�A�N�V����
	*/
	void BattleManager::waitAction()
	{
		map_->confirmMove(selectedUnit_);
		selectedUnit_->endAction();
		endSelectActionPhase();
	}

	/**
	 * @fn
	 * �Ώۃ��j�b�g�ɍU���\��
	 * @param (targetUnit) �U���Ώۃ��j�b�g
	*/
	bool BattleManager::isAtackAble(shared_ptr<Unit> targetUnit) const
	{
		if (targetUnit)
		{
			shared_ptr<Mass> mass = map_->getMass(targetUnit->getMassX(), targetUnit->getMassY());
			return isSelectedUnitActive() && targetUnit->isEnemy() && mass->state == Mass::ATK_ABLE;
		}
		return false;
	}

	/**
	 * @fn
	 * �I�𒆂̃��j�b�g���s���\�ł��邩
	*/
	bool BattleManager::isSelectedUnitActive() const
	{
		return selectedUnit_ && !selectedUnit_->isEnemy() && !selectedUnit_->isActed();
	}

	/**
	 * @fn
	 * �I�𒆂̃��j�b�g�ł��邩����
	 * @param (unit) ����Ώۃ��j�b�g
	*/
	bool BattleManager::isSelectedUnit(shared_ptr<Unit> unit) const
	{
		return selectedUnit_ == unit;
	}
}