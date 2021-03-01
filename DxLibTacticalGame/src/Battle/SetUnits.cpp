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
}