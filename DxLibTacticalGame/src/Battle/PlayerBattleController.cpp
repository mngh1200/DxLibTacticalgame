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
	void PlayerBattleController::updateByEvents(BattleManager* bm, shared_ptr<Object> hitObj, int x, int y, int button, int* eventType)
	{
		if (*eventType == MOUSE_INPUT_LOG_CLICK && !bm->isAnimation()) // �N���b�N ���@�A�j���[�V�������ł͂Ȃ�
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
		else if (*eventType == MOUSE_INPUT_LOG_UP && button == MOUSE_INPUT_RIGHT && !bm->isAnimation()) // �}�E�X�E�N���b�N�i�����ɂ̓{�^���A�b�v�j
		{
			if (hitObj->getType() == Object::Type::MAP)
			{
				if (checkMoveConfirm(bm, x, y))
				{
					*eventType = MOUSE_INPUT_LOG_USED;
				}
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

		bm->battleUI.updateByEvents(hitObj, x, y, button, *eventType);

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
				bm->tutorial.onEvent(TutorialManager::TutorialId::ATACK);
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
				bm->tutorial.onEvent(TutorialManager::TutorialId::TURN_END);
				Utility::ResourceManager::playSound(SoundKind::WAIT);
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
				bm->tutorial.onEvent(TutorialManager::TutorialId::MOVE);
				Utility::ResourceManager::playSound(SoundKind::SELECT_UNIT);
			}
		}
	}

	/**
	 * @fn
	 * �}�X �N���b�N������
	 * @param (bm) �o�g���Ǘ��N���X
	 * @param (x) x���W
	 * @param (y) y���W
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

	/**
	 * @fn
	 * �}�X �E�N���b�N������
	 * @param (bm) �o�g���Ǘ��N���X
	 * @param (x) x���W
	 * @param (y) y���W
	 * @return �ړ��A�ړ��m������s�����ꍇ true��Ԃ�
	 */
	bool PlayerBattleController::checkMoveConfirm(BattleManager* bm, int x, int y)
	{
		int massX = Map::getMassX(x);
		int massY = Map::getMassY(y);

		if (bm->isSelectedUnitActive())
		{
			shared_ptr<Mass> targetMass = map_->getMass(massX, massY);

			if (bm->getPhase() != BattleManager::Phase::SELECT_ACTION && targetMass->isMovable())
			{
				bm->moveAction(massX, massY, true); //�ړ� & �ړ��m��
				Utility::ResourceManager::playSound(SoundKind::WAIT);
				return true;
			}
		}
		return false;
	}
}