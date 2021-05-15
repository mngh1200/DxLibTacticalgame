#include "EnemyBattleController.h"

namespace Battle {
	/**
	 * @fn
	 * ��������
	 * @param (map) �}�b�v�̃|�C���^
	 * @param (aiKind) AI�̎��
	 */
	void EnemyBattleController::init(shared_ptr<Map> map, int aiKind)
	{
		map_ = map;
		ai_ = make_unique<EnemyAI>();
		ai_->init(map);

		if (aiKind == AiKind::SELF_DEFENCE_ONLY) // �h�q�̂�AI�̏ꍇ
		{
			Oriented oriented = {};
			oriented.stay = 100;
			oriented.atack = 1;
			ai_->setOriented(oriented);
		}
		else if (aiKind == AiKind::AGGRESSIVE) // �U���I��AI
		{
			Oriented oriented = {};
			oriented.atack = 20;
			oriented.stay = 1;
			oriented.friendship = 1;
			ai_->setOriented(oriented);
		}
	}

	/**
	 * @fn
	 * �X�V����
	 * @param (bm) �o�g���Ǘ��N���X�̃|�C���^
	 * @return �G�^�[���I������true��Ԃ�
	 */
	bool EnemyBattleController::update(BattleManager* bm)
	{
		// �e�X�g����
		/*
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();
		for (auto itr = ai_->massBaseScoreMap.begin(); itr != ai_->massBaseScoreMap.end(); ++itr)
		{
			int x = itr->first.first;
			int y = itr->first.second;
			int score = itr->second;

			DxLib::DrawStringFToHandle(x * CHIP_SIZE, y * CHIP_SIZE, to_string(score).c_str(), 0xff0000, rm.getHdlFont(FontType::NORMAL_S32));
		}
		DxLib::ScreenFlip();
		*/

		// �A�j���[�V������
		if (bm->isAnimation())
		{
			return false;
		}

		// �^�C�}�[�C�x���g�`�F�b�N
		if (timerEvent_ != TimerEvent::TIMER_EVENT_NONE)
		{
			if (timer_.checkAndCountTimer())
			{
				if (timerEvent_ == TimerEvent::ATACK) // �U��
				{
					bm->atackAction();
				}
				timerEvent_ = TimerEvent::TIMER_EVENT_NONE;
			}
			return false; // �^�C�}�[�C�x���g���͑��얽�߂̐i�s�͂��Ȃ�
		}


		// ���얽�߃`�F�b�N
		Order order = ai_->getNextOrder(); // ���̑��얽�߂��擾

		if (order.kind == ActionKind::NO_ACTION) // ���߂Ȃ��̏ꍇ
		{
			if (ai_->createOrders(map_)) // �V�������얽�ߐ���
			{
				order = ai_->getNextOrder();
			}
			else
			{
				return true; // ���얽�߂��Ȃ��Ȃ������ߏI��
			}
		}

		if (order.kind == ActionKind::MOVE_ACT) // �ړ�
		{
			bm->selectUnit(order.unit);
			bm->moveAction(order.massX, order.massY);
		}
		else if (order.kind == ActionKind::ATACK_ACT) // �U��
		{
			bm->setFightPredict(map_->getUnit(order.massX, order.massY));
			setTimerEvent(1500, TimerEvent::ATACK); // �퓬�\���o���Ă����莞�Ԍ�ɍU�����s
			
		}
		else if (order.kind == ActionKind::WAIT_ACT) // �ҋ@
		{
			bm->waitAction();
		}

		return false;
	}

	/**
	 * @fn
	 * �^�C�}�[�C�x���g�ǉ�
	 * @param (ms) �Z�b�g����(ms)
	 * @param (kind) �C�x���g�̎��
	 */
	void EnemyBattleController::setTimerEvent(int ms, TimerEvent kind)
	{
		timer_.setTimer(EnemyTurnCont::getAnimationMs(ms));
		timerEvent_ = kind;
	}
}