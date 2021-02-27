#include "SetUnits.h"

namespace SetUnits {

	/**
	 * @fn
	 * ユニット配置シーンでのクリックイベント
	 * @param (x) クリック座標x
	 * @param (y) クリック座標y
	 * @param (map) mapの参照
	 * @param (battleUI) バトル用UIクラス
	 * @param (tutorial) チュートリアル管理クラス
	 * @param (bm) バトル管理クラス
	 */
	void onClick(int x, int y, shared_ptr<Map> map, Battle::BattleUI* battleUI, Battle::TutorialManager* tutorial, Battle::BattleManager* bm)
	{
		int massX = Map::getMassX(x);
		int massY = Map::getMassY(y);
		int unitId = battleUI->getSelectedUnitId();

		if (!map->isRange(massX, massY) || unitId == -1)
		{
			return; // 範囲外　または　ユニットが選択されていない場合 終了
		}

		shared_ptr<Mass> mass = map->getMass(massX, massY);
		shared_ptr<Unit> unit = map->getUnit(massX, massY);

		if (!mass->ableUnitSet)
		{
			return; // ユニット配置可能マスでない場合は終了
		}

		if (unit) // 対象マスにユニットが存在する場合は削除
		{
			battleUI->removeSetUnitCount();
			map->eraseUnit(unit);
			unit->destroy();
			Utility::ResourceManager::playSound(SoundKind::BACK);
			tutorial->onEvent(Battle::TutorialManager::TutorialId::FREE_SET_FIN, bm);
		}
		else if (battleUI->addSetUnitCount()) // 対象マスにユニットが存在しない場合は、設置
		{
			map->setUnit(massX, massY, unitId);
			Utility::ResourceManager::playSound(SoundKind::CHECK);
			tutorial->onEvent(Battle::TutorialManager::TutorialId::FREE_SET_DEL, bm,
				Map::getRealX(massX) + CHIP_SIZE / 2, Map::getRealY(massY));
		}
	}
}