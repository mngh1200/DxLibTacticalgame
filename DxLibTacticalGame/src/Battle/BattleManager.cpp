#include "BattleManager.h"
#include "Screen/BattleScreen.h"

namespace Battle {
	/**
	 * @fn
	 * ��������
	 * @param (map) �}�b�v�̃|�C���^
	 */
	void BattleManager::init(shared_ptr<Entity::Map> map)
	{
		map_ = map;
		battleUI.init();
		fight_.init(map_);

		// �X�e�[�W�f�[�^�ǂݍ���
		string title;
		string hint;
		std::array < std::array <int, MAP_MASS_W>, MAP_MASS_H > mapData;
		vector<vector<int>> units;
		vector<int> checkWinData;
		Utility::ResourceManager::loadStageData("stage", 1, &title, &hint, &mapData, &checkWinData, &units);

		map->loadStageData(mapData);
		map->loadUnits(units);
		checkWin_.loadData(checkWinData);

		// ���b�Z�[�W�����I�u�W�F�N�g�Ƃ��Ēǉ�
		message = make_shared<Message>();
		FrameWork::Game& game = FrameWork::Game::getInstance();
		game.objectsControl.addObject(Screen::BattleScreen::Layer::TOP_UI, Screen::BattleScreen::TopUiId::MESSAGE, message);
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
			checkWin_.checkWin(map_);
		}

	}

	/**
	 * @fn
	 * �Q�[���I������
	 * @return �I����:true
	*/
	int BattleManager::checkEnd()
	{
		if (checkWin_.getWinner() != Winner::UNDECIDED)
		{
			return checkWin_.getWinner();
		}
		return Winner::UNDECIDED;
	}

	/**
	 * @fn
	 * �^�[���X�^�[�g������
	 * @param (isPlayer) �v���C���[�^�[�����J�n����ꍇ true
	*/
	void BattleManager::onStartTurn(bool isPlayer)
	{
		// �s�����I�����j�b�g�������ꍇ�A�ҋ@�A�N�V����������
		if (phase_ == Phase::SELECT_ACTION)
		{
			waitAction();
		}

		isPlayerTurn_ = isPlayer;

		for (auto itr = map_->unitsBegin(); itr != map_->unitsEnd(); ++itr)
		{
			itr->second->turnEnd(itr->second->isEnemy() == isPlayer);
		}

		// �o�g��UI����
		battleUI.onStartTurn(isPlayer);

		phase_ = Phase::NORMAL;

		// �^�[���o��
		++turnNumEach_;

		// ���s����
		checkWin_.checkWin(getNowTurn());

		// �c��^�[�����b�Z�[�W
		if (isPlayerTurn_)
		{
			checkWin_.showRemainingTurnMessage(message, getNowTurn());
		}
		
	}

	/**
	 * @fn
	 * �s���I���t�F�C�Y�J�n
	*/
	void BattleManager::startSelectActionPhase()
	{
		if (isSelectedUnitActive())
		{
			map_->clearMassState();
			phase_ = Phase::SELECT_ACTION; // �s���I�� 
			map_->displayAtackAbleRange(selectedUnit_, selectedUnit_->getMassX(), selectedUnit_->getMassY());
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
		checkWin_.checkWin(map_);
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
				battleUI.setTargetUnit(selectedUnit_);

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
				battleUI.resetTargetUnit();
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
		if (isSelectedUnitActive())
		{
			map_->confirmMove(selectedUnit_);
			fight_.start();
			map_->clearMassState();
			deselectUnit();
			phase_ = Phase::FIGHT;
		}
	}

	/**
	 * @fn
	 * �ҋ@�A�N�V����
	*/
	void BattleManager::waitAction()
	{
		if (isSelectedUnitActive())
		{
			map_->confirmMove(selectedUnit_);
			selectedUnit_->endAction();
			endSelectActionPhase();
		}
	}

	/**
	 * @fn
	 * ���j�b�g�ړ��A�N�V����
	 * @param (massX) �}�X���WX
	 * @param (massY) �}�X���WY
	*/
	void BattleManager::moveAction(int massX, int massY)
	{
		selectedUnit_->move(massX, massY); // �ړ�
		phase_ = Phase::MOVE;
	}

	/**
	 * @fn
	 * �ړ��L�����Z��
	*/
	void BattleManager::moveCancel()
	{
		selectedUnit_->back(); // �ړ��L�����Z��
		endSelectActionPhase(); // �s���I���I��
	}


	/**
	 * @fn
	 * �퓬�\���𐶐�
	 * @param (targetUnit) �U���Ώۃ}�X
	*/
	void BattleManager::setFightPredict(shared_ptr<Unit> targetUnit)
	{
		if (isSelectedUnitActive())
		{
			if (fight_.setPrepare(selectedUnit_, targetUnit))
			{
				battleUI.setFightPredict(&fight_);

				if (!isPlayerTurn_)
				{
					// �G�R�̑���̏ꍇ�A�U���\�G���A�̕\�����U���Ώۂ݂̂ɂ���
					map_->clearMassState();
					shared_ptr<Mass> mass = map_->getMass(targetUnit->getMassX(), targetUnit->getMassY());
					if (mass)
					{
						mass->state = Mass::ATK_ABLE;
					}
				}
			}
		}
	}

	/**
	 * @fn
	 * �퓬�\�����N���A
	*/
	void BattleManager::resetFightPredict()
	{
		if (phase_ != Phase::FIGHT)
		{
			fight_.reset();
			battleUI.resetFightPredict();
		}
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
			return isSelectedUnitActive() && targetUnit->isEnemy() == isPlayerTurn_ && mass->state == Mass::ATK_ABLE;
		}
		return false;
	}

	/**
	 * @fn
	 * �I�𒆂̃��j�b�g���s���\�ł��邩
	*/
	bool BattleManager::isSelectedUnitActive() const
	{
		return selectedUnit_ && !selectedUnit_->isEnemy() == isPlayerTurn_ && !selectedUnit_->isActed();
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