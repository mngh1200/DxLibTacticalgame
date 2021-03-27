#pragma once
#include <climits>
#include <map>
#include "BUI/BattleUI.h"
#include "Fight.h"
#include "CheckWin.h"
#include "Battle/TutorialManager.h"
#include "Entity/Battle/Map.h"
#include "Entity/Unit/Unit.h"
#include "BUI/BattleUI.h"
#include "Entity/UI/Message.h"
#include "Network/SendManager.h"

using namespace std;
using namespace Entity;

/**
 * @file TextButton.h
 * @brief �e�L�X�g�t���{�^���v�f
 */

namespace Battle
{
	class BattleManager
	{
	public:
		BattleManager() :
			isPlayerTurn_(true),
			isMoveImmdiateConfirm_(false),
			phase_(Phase::NORMAL),
			turnNumEach_(0),
			checkWin_{},
			battleUI{},
			tutorial{}
		{};
		~BattleManager() {};

		// ��ԑJ�ڏ�
		enum Phase
		{
			NORMAL,
			MOVE,
			SELECT_ACTION,
			FIGHT
		};

		void init(shared_ptr<Entity::Map> map, int stageId, int* setUnitNum, int* aiKind);
		void init(shared_ptr<Entity::Map> map, int stageId, int setUnitNum, bool isServer, shared_ptr<Network::SendManager> sender);

		void animationCheck();
		int checkEnd();

		void onStartTurn(bool isPlayer);

		void startSelectActionPhase();
		void endSelectActionPhase();

		void selectUnit(shared_ptr<Unit> unit);
		bool deselectUnit();

		void atackAction(int actHitState = FightData::HitState::UNSETTLED, int psvHitState = FightData::HitState::UNSETTLED);
		void waitAction();
		void moveAction(int massX, int massY, bool isMoveImmdiateConfirm = false);
		void moveCancel();

		void setFightPredict(shared_ptr<Unit> targetUnit);
		void resetFightPredict();

		bool isPlayerTurn() const { return isPlayerTurn_; } // �v���C���[�ł��邩��Ԃ�
		int getPhase() const { return phase_; }; // �󋵂�Ԃ�
		bool isAnimation() const { return phase_ == Phase::FIGHT || phase_ == Phase::MOVE; }; // �A�j���[�V�������ł��邩��Ԃ�
		bool isAtackAble(shared_ptr<Unit> targetUnit) const;
		bool isSelectedUnitActive() const;
		bool isSelectedUnit(shared_ptr<Unit> unit) const;
		int getNowTurn() const { return turnNumEach_ / 2 + 1; }; // ���݃^�[����Ԃ�
		const CheckWin& getCheckWin() const { return checkWin_; }; // ���s����p�N���X��Ԃ�
		const shared_ptr<Unit> getSelectedUnit() const { return selectedUnit_; } // �I�𒆂̃��j�b�g��Ԃ�

		//! �o�g��UI
		BattleUI battleUI;

		//! �`���[�g���A���Ǘ��N���X
		TutorialManager tutorial;

		//! ���b�Z�[�W
		shared_ptr<Entity::Message> message;

		//! �}�b�v
		shared_ptr<Entity::Map> map;

	private:
		void initCommon(shared_ptr<Entity::Map> map);

		//! �o�߃^�[����(�v���C���[�^�[���A�G�^�[���ł��ꂼ��{�P�ɂ���A�\���^�[���̓�{�ŕ\��)
		int turnNumEach_;

		//! �I�𒆂̃��j�b�g
		shared_ptr<Entity::Unit> selectedUnit_;

		//! �l�퓬�Ǘ��p
		Fight fight_;

		//! �t�F�[�Y
		int phase_;

		//! �G�^�[���ł��邩����
		bool isPlayerTurn_;

		//! ����
		CheckWin checkWin_;

		//! �ړ��𑦊m�肳���邩
		bool isMoveImmdiateConfirm_;

		//! �f�[�^���M�Ǘ��N���X
		shared_ptr<Network::SendManager> sender_;
	};


}