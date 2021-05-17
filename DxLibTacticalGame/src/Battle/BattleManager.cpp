#include "BattleManager.h"
#include "Screen/BattleScreen.h"

namespace Battle {
	/**
	 * @fn
	 * ���ʂ̏�������
	 * @param (map) �}�b�v�̃|�C���^
	 * @param (stageId) �X�e�[�WID
	 * @param (isSetUnit) ���j�b�g�z�u�V�[���̗L���擾�p
	 */
	void BattleManager::initCommon(shared_ptr<Entity::Map> map)
	{
		this->map = map;
		battleUI.init();
		fight_.init(this->map);

		// ���b�Z�[�W�����I�u�W�F�N�g�Ƃ��Ēǉ�
		message = make_shared<Message>();
		FrameWork::Game& game = FrameWork::Game::getInstance();
		game.objectsControl.addObject(Screen::BattleScreen::Layer::TOP_UI, Screen::BattleScreen::TopUiId::MESSAGE, message);
	}

	/**
	 * @fn
	 * �W�����̏�������
	 * @param (map) �}�b�v�̃|�C���^
	 * @param (stageId) �X�e�[�WID
	 * @param (isSetUnit) ���j�b�g�z�u�V�[���̗L���擾�p
	 */
	void BattleManager::init(shared_ptr<Entity::Map> map, int stageId, int* setUnitNum, int* aiKind)
	{
		initCommon(map);

		// �X�e�[�W�f�[�^�ǂݍ���
		string title;
		string hint;
		std::array < std::array <int, MAP_MASS_W>, MAP_MASS_H > mapData;
		vector<vector<int>> units;
		vector<int> checkWinData;
		vector<int> extraRules;
		Utility::ResourceManager::loadStageData("stage", stageId, &title, &hint, &checkWinData, &extraRules, &mapData, &units);

		this->map->loadStageData(mapData);
		this->map->loadUnits(units);
		checkWin_.loadData(checkWinData);

		// �`���[�g���A��
		tutorial.init(stageId, message);

		// ���j�b�g�z�u�\���̊m�F
		*setUnitNum = 0;
		if (extraRules.size() > 0)
		{
			int countMax = extraRules[0];

			if (countMax > 0)
			{
				battleUI.startSelectUnitMode(countMax);
				tutorial.onEvent(TutorialManager::TutorialId::FREE_SET_SELECT, this);
				*setUnitNum = countMax;
			}
		}

		// AI�̎�ގ擾
		*aiKind = 0;
		if (extraRules.size() > 1)
		{
			*aiKind = extraRules[1];
		}

		// �`���[�g���A���P�`�R�͓G�P�΂�HP������Ă���
		if (stageId < 3)
		{
			for (auto itr = map->unitsBegin(); itr != map->unitsEnd(); ++itr)
			{
				shared_ptr<Unit> unit = itr->second;
				if (unit->isEnemy())
				{
					unit->forceDecreaseHp(15);
					break;
				}
			}
		}
	}

	/**
	 * @fn
	 * �ʐM�ΐ펞�̏�������
	 * @param (map) �}�b�v�̃|�C���^
	 * @param (stageId) �X�e�[�WID
	 * @param (isSetUnit) ���j�b�g�z�u�V�[���̗L���擾�p
	 */
	void BattleManager::init(shared_ptr<Entity::Map> map, int stageId, int setUnitNum, bool isServer, shared_ptr<Network::SendManager> sender)
	{
		initCommon(map);

		// �X�e�[�W�t�@�C����
		string fileName = "match-s";

		if (!isServer)
		{
			fileName = "match-c";
		}

		// �X�e�[�W�f�[�^�ǂݍ���
		string title;
		string hint;
		std::array < std::array <int, MAP_MASS_W>, MAP_MASS_H > mapData;
		vector<vector<int>> units;
		vector<int> checkWinData;
		vector<int> extraRules;
		Utility::ResourceManager::loadStageData(fileName, stageId, &title, &hint, &checkWinData, &extraRules, &mapData, &units);

		this->map->loadStageData(mapData);
		this->map->loadUnits(units);
		checkWin_.loadData(checkWinData);

		battleUI.startSelectUnitMode(setUnitNum);

		// ���M�Ǘ��N���X
		sender_ = sender;
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
				tutorial.onEvent(TutorialManager::TutorialId::MOVE_CONFIRM, this);

				if (isMoveImmdiateConfirm_) // �ړ������m��
				{
					waitAction();
				}
			}
		}
		else if (phase_ == Phase::FIGHT && fight_.checkUpdate()) // �U��
		{
			// �U���I��

			if (sender_ && isPlayerTurn_) // �f�[�^���M
			{
				FightData actData = fight_.getFightData(true);
				FightData psvData = fight_.getFightData(false);

				ContLog contLog = ContLog{ psvData.unit->getMassX(), psvData.unit->getMassY(), actData.unit->getObjectId(), ActionKind::ATACK_ACT, actData.hitState, psvData.hitState };
				sender_->sendPlayerContLog(contLog);
			}

			phase_ = Phase::NORMAL;
			tutorial.onFight(&fight_, TutorialManager::FightPhase::END, this);
			checkWin_.checkWin(map);
			fight_.reset();
		}

	}

	/**
	 * @fn
	 * �Q�[���I������
	 * @return �I����:true
	*/
	int BattleManager::checkEnd()
	{
		if (checkWin_.getWinner() != CheckWin::Winner::UNDECIDED)
		{
			return checkWin_.getWinner();
		}
		return CheckWin::Winner::UNDECIDED;
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
		else
		{
			deselectUnit();
		}

		isPlayerTurn_ = isPlayer;

		for (auto itr = map->unitsBegin(); itr != map->unitsEnd(); ++itr)
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

		if (isPlayerTurn_)
		{
			// �c��^�[�����b�Z�[�W
			checkWin_.showRemainingTurnMessage(message, getNowTurn());

			// �`���[�g���A��
			tutorial.onPlayerTurnStart(this);

			// �G�̍U���͈͔�����X�V
			map->updateAttackedArea(true);
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
			map->clearMassState();
			phase_ = Phase::SELECT_ACTION; // �s���I�� 
			if (isPlayerTurn_)
			{
				map->displayAtackAbleRange(selectedUnit_, selectedUnit_->getMassX(), selectedUnit_->getMassY());
			}
		}
	}

	/**
	 * @fn
	 * �s���I���I��
	*/
	void BattleManager::endSelectActionPhase()
	{
		map->clearMassState();
		phase_ = Phase::NORMAL;
		deselectUnit();
		checkWin_.checkWin(map);
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

				if (isPlayerTurn_ && !unit->isActed())
				{
					map->displayMovableRange(unit);
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
				map->clearMassState();
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
	 * @param (actHitState) �U���d�|�������̖����󋵁i�m�肵�Ă���ꍇ����������ǉ��j
	 * @param (psvHitState) �U�����ꂽ���̖����󋵁i�m�肵�Ă���ꍇ����������ǉ��j
	*/
	void BattleManager::atackAction(int actHitState, int psvHitState)
	{
		if (isSelectedUnitActive())
		{
			battleUI.onStartFight();
			map->confirmMove(selectedUnit_, sender_);
			fight_.setHitState(actHitState, psvHitState);
			fight_.start();
			map->clearMassState();
			deselectUnit();
			phase_ = Phase::FIGHT;
			tutorial.onFight(&fight_, TutorialManager::FightPhase::START, this);

			if (isPlayerTurn_)
			{
				map->updateAttackedArea(true);
			}
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
			map->confirmMove(selectedUnit_, sender_);

			if (sender_ && isPlayerTurn_) // �f�[�^���M
			{
				// �ҋ@
				ContLog contLog = ContLog{ 0, 0, selectedUnit_->getObjectId(), ActionKind::WAIT_ACT };
				sender_->sendPlayerContLog(contLog);
			}

			selectedUnit_->endAction();
			endSelectActionPhase();

			if (isPlayerTurn_)
			{
				map->updateAttackedArea(true);
			}
		}
	}

	/**
	 * @fn
	 * ���j�b�g�ړ��A�N�V����
	 * @param (massX) �}�X���WX
	 * @param (massY) �}�X���WY
	 * @param (isMoveImmdiateConfirm) true�̏ꍇ�A�ړ������m��
	*/
	void BattleManager::moveAction(int massX, int massY, bool isMoveImmdiateConfirm)
	{
		selectedUnit_->move(massX, massY); // �ړ�
		phase_ = Phase::MOVE;
		isMoveImmdiateConfirm_ = isMoveImmdiateConfirm;
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
		if (!isSelectedUnitActive())
		{
			return;
		}

		if (fight_.setPrepare(selectedUnit_, targetUnit))
		{
			battleUI.setFightPredict(&fight_);

			if (!isPlayerTurn_)
			{
				// �G�R�̑���̏ꍇ�A�U���\�G���A�̕\�����U���Ώۂ݂̂ɂ���
				map->clearMassState();
				shared_ptr<Mass> mass = map->getMass(targetUnit->getMassX(), targetUnit->getMassY());
				if (mass)
				{
					mass->state = Mass::ATK_ABLE;
				}
			}
			else
			{
				tutorial.onFight(&fight_, TutorialManager::FightPhase::PREDICT, this);
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
			shared_ptr<Mass> mass = map->getMass(targetUnit->getMassX(), targetUnit->getMassY());
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