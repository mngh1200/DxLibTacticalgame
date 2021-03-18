#include "SetUnits.h"

namespace SetUnits {

	/**
	 * @fn
	 * ���j�b�g�z�u�V�[���ł̃N���b�N�C�x���g
	 * @param (x) �N���b�N���Wx
	 * @param (y) �N���b�N���Wy
	 * @param (map) map�̎Q��
	 * @param (battleUI) �o�g���pUI�N���X
	 * @param (tutorial) �`���[�g���A���Ǘ��N���X
	 * @param (bm) �o�g���Ǘ��N���X
	 */
	void onClick(int x, int y, shared_ptr<Map> map, Battle::BattleUI* battleUI, Battle::TutorialManager* tutorial, Battle::BattleManager* bm)
	{
		int massX = Map::getMassX(x);
		int massY = Map::getMassY(y);
		int unitId = battleUI->getSelectedUnitId();

		if (!map->isRange(massX, massY) || unitId == -1)
		{
			return; // �͈͊O�@�܂��́@���j�b�g���I������Ă��Ȃ��ꍇ �I��
		}

		shared_ptr<Mass> mass = map->getMass(massX, massY);
		shared_ptr<Unit> unit = map->getUnit(massX, massY);

		if (!mass->ableUnitSet)
		{
			return; // ���j�b�g�z�u�\�}�X�łȂ��ꍇ�͏I��
		}

		if (unit) // �Ώۃ}�X�Ƀ��j�b�g�����݂���ꍇ�͍폜
		{
			battleUI->removeSetUnitCount();
			map->eraseUnit(unit);
			unit->destroy();
			Utility::ResourceManager::playSound(SoundKind::BACK);
			tutorial->onEvent(Battle::TutorialManager::TutorialId::FREE_SET_FIN, bm);
		}
		else if (battleUI->addSetUnitCount()) // �Ώۃ}�X�Ƀ��j�b�g�����݂��Ȃ��ꍇ�́A�ݒu
		{
			map->setUnit(massX, massY, unitId);
			Utility::ResourceManager::playSound(SoundKind::CHECK);
			tutorial->onEvent(Battle::TutorialManager::TutorialId::FREE_SET_DEL, bm,
				Map::getRealX(massX) + CHIP_SIZE / 2, Map::getRealY(massY));
		}
	}

	/**
	 * @fn
	 * ���j�b�g�z�u�f�[�^�𑗐M
	 * @param (sender) ���M�Ǘ��N���X
	 * @param (map) map�̎Q��
	 */
	void sendSetUnitsData(Network::SendManager* sender, shared_ptr<Map> map)
	{
		for (auto itr = map->unitsBegin(); itr != map->unitsEnd(); ++itr)
		{
			shared_ptr<Unit> unit = (*itr).second;

			if (unit->isEnemy())
			{
				continue;
			}

			Network::ContLog contLog = Network::ContLog();

			contLog.x = unit->getMassX();
			contLog.y = unit->getMassY();
			contLog.unitId = unit->getObjectId();
			contLog.actionKind = ActionKind::SET_ACT;
			contLog.extraValue = unit->getInfo().kind;

			sender->sendPlayerContLog(contLog); // �z�u��񑗐M
		}

		sender->sendSignal(Network::SignalKind::SET_END); // ���R�z�u�I���V�O�i�����M
	}

	/**
	 * @fn
	 * ���j�b�g�z�u�f�[�^����M
	 * @param (receiver) ��M�Ǘ��N���X
	 * @param (map) map�̎Q��
	 */
	void receiveSetUnitsData(Network::ReceiveManager* receiver, shared_ptr<Map> map)
	{
		while (true)
		{
			Network::ContLog contLog = receiver->getNextContLog(false);

			if (contLog.actionKind != ActionKind::SET_ACT) // ���R�z�u�ȊO�̏ꍇ�A�I��
			{
				break;
			}

			map->setUnit(contLog.x, contLog.y, contLog.extraValue, true, contLog.unitId);
			receiver->popContLog(); // �퓬�̑��샍�O�폜
		}
	}
}