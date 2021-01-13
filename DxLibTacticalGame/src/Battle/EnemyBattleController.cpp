#include "EnemyBattleController.h"

namespace Battle {
	
	/**
	 * @fn
	 * ��������
	 * @param (map) �}�b�v�̃|�C���^
	 */
	void EnemyBattleController::init(shared_ptr<Map> map)
	{
		map_ = map;
		ai_ = make_unique<EnemyAI>();
	}

	/**
	 * @fn
	 * �X�V����
	 * @param (bm) �o�g���Ǘ��N���X�̃|�C���^
	 * @return �G�^�[���I������true��Ԃ�
	 */
	bool EnemyBattleController::update(BattleManager* bm)
	{
		int phase = bm->getPhase();

		// �A�j���[�V������
		if (phase == BattleManager::Phase::FIGHT || phase == BattleManager::Phase::MOVE)
		{
			return false;
		}

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
			bm->atackAction();
		}
		else if (order.kind == Order::Kind::WAIT) // �ҋ@
		{
			bm->waitAction();
		}

		return false;
	}
}