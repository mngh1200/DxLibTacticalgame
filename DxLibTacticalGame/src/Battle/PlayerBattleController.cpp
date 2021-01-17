#include "PlayerBattleController.h"

namespace Battle {
	
	/**
	 * @fn
	 * ��������
	 * @param (map) �}�b�v�̃|�C���^
	 */
	void PlayerBattleController::init(shared_ptr<Map> map)
	{
		map_ = map;
	}

	/**
	 * @fn
	 * �C�x���g����
	 */
	void PlayerBattleController::updateByEvents(BattleManager* bm, shared_ptr<Object> hitObj, int x, int y, int button, int eventType)
	{
		if (eventType == MOUSE_INPUT_LOG_CLICK) // �N���b�N
		{
			if (hitObj->getType() == Object::Type::UNIT)
			{
				onClickUnit(bm, hitObj);
			}
			else if (hitObj->getType() == Object::Type::MAP)
			{
				onClickMass(bm, x, y);
			}
		}

		if (hitObj->getType() == Object::Type::UNIT || hitObj->getType() == Object::Type::MAP)
		{
			// �z�o�[������
			onHoverObject(bm, x, y);
		}
		else
		{
			// �퓬�\������
			bm->resetFightPredict();
		}

		bm->battleUI.updateByEvents(hitObj, x, y, button, eventType);

	}

	/**
	 * @fn
	 * �}�b�v�v�f�z�o�[���̏���
	 */
	void PlayerBattleController::onHoverObject(BattleManager* bm, int x, int y)
	{
		// 1�}�b�v��̗v�f���z�o�[
		int massX = Map::getMassX(x);
		int massY = Map::getMassY(y);

		shared_ptr<Mass> mass = map_->getMass(massX, massY);
		shared_ptr<Unit> unit = map_->getUnit(massX, massY);

		// �O��Ɠ������j�b�g���z�o�[���Ă���ꍇ�͏��������{���Ȃ�
		if (mass == hoverMass_ && unit == hoverUnit_)
		{
			return;
		}

		if (mass)
		{
			if (bm->isAtackAble(unit)) // �U���\���j�b�g���w�肵�Ă��鎞
			{
				// �퓬�\��
				bm->setFightPredict(unit);
			}
			else
			{
				// �퓬�\������
				bm->resetFightPredict();

				// �}�X�f�[�^�\��
				bm->battleUI.setTargetMass(mass);
			}
		}
	}

	/**
	 * @fn
	 * ���j�b�g �N���b�N������
	 */
	void PlayerBattleController::onClickUnit(BattleManager* bm, shared_ptr<Object> hitObj)
	{
		shared_ptr<Unit> unit = dynamic_pointer_cast<Unit>(hitObj);
		bool isOwn = !unit->isEnemy(); // �������j�b�g�ł��邩
		

		if (bm->isAtackAble(unit)) // �U���Ώۂ̃��j�b�g�N���b�N
		{
			bm->atackAction(); // �U���A�N�V����

		}
		else if (bm->getPhase() == BattleManager::Phase::SELECT_ACTION) // �s���I��
		{
			if (bm->isSelectedUnit(unit)) // �I�𒆂̃��j�b�g�N���b�N
			{
				bm->waitAction(); // �ҋ@�A�N�V����
			}
			else
			{
				bm->endSelectActionPhase(); // �s���I���I��
				bm->selectUnit(unit); // ���̃��j�b�g�I��
			}
		}
		else
		{
			if (bm->isSelectedUnit(unit))
			{
				if (isOwn)
				{
					// �N���b�N�������j�b�g�� �I�𒆂̃��j�b�g�������ꍇ�A�s���I���t�F�C�Y�Ɉڍs
					bm->startSelectActionPhase();
				}
			}
			else
			{
				bm->selectUnit(unit); // ���j�b�g�I��
			}
		}
	}

	/**
	 * @fn
	 * �}�X �N���b�N������
	 */
	void PlayerBattleController::onClickMass(BattleManager* bm, int x, int y)
	{
		int massX = Map::getMassX(x);
		int massY = Map::getMassY(y);

		if (bm->isSelectedUnitActive())
		{

			shared_ptr<Mass> targetMass = map_->getMass(massX, massY);
			
			if (bm->getPhase() == BattleManager::Phase::SELECT_ACTION ) // �s���I��
			{
				bm->moveCancel(); // �ړ��L�����Z��
			}
			else if (targetMass->isMovable())
			{
				bm->moveAction(massX, massY); //�ړ�
			}
			else
			{
				bm->deselectUnit(); // �I������
			}
		}
		else
		{
			bm->deselectUnit(); // �I������
		}
	}
}