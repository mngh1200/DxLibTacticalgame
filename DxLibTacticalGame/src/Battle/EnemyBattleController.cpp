#include "EnemyBattleController.h"

namespace Battle {
	float EnemyBattleController::timerRate = 1.0f;

	/**
	 * @fn
	 * ��������
	 * @param (map) �}�b�v�̃|�C���^
	 */
	void EnemyBattleController::init(shared_ptr<Map> map)
	{
		map_ = map;
		ai_ = make_unique<EnemyAI>();
		ai_->init(map);
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

		int phase = bm->getPhase();

		// �A�j���[�V������
		if (phase == BattleManager::Phase::FIGHT || phase == BattleManager::Phase::MOVE)
		{
			return false;
		}

		// �^�C�}�[�C�x���g�`�F�b�N
		if (timerEvent_ != TimerEvent::NONE)
		{
			if (timer_.checkAndCountTimer())
			{
				if (timerEvent_ == TimerEvent::ATACK) // �U��
				{
					bm->atackAction();
				}
				timerEvent_ = TimerEvent::NONE;
			}
			return false; // �^�C�}�[�C�x���g���͑��얽�߂̐i�s�͂��Ȃ�
		}


		// ���얽�߃`�F�b�N
		Order order = ai_->getNextOrder(); // ���̑��얽�߂��擾

		if (order.kind == Order::Kind::NONE) // ���߂Ȃ��̏ꍇ
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

		if (order.kind == Order::Kind::MOVE) // �ړ�
		{
			bm->selectUnit(order.unit);
			bm->moveAction(order.massX, order.massY);
		}
		else if (order.kind == Order::Kind::ATACK) // �U��
		{
			bm->setFightPredict(map_->getUnit(order.massX, order.massY));
			setTimerEvent(1500, TimerEvent::ATACK); // �퓬�\���o���Ă����莞�Ԍ�ɍU�����s
			
		}
		else if (order.kind == Order::Kind::WAIT) // �ҋ@
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
		timer_.setTimer((int)(ms * timerRate));
		timerEvent_ = kind;
	}
}