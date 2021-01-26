#include "CheckWin.h"

namespace Battle {

	/**
	 * @fn
	 * 勝敗判定
	 * @param (map) Mapのスマートポインタ 
	*/
	void CheckWin::checkWin(shared_ptr<Map> map)
	{
		bool existEnemyUnit = false;
		bool existPlayerUnit = false;

		// どちらかのユニットが全ていなくなっているかチェック
		for (auto itr = map->unitsBegin(); itr != map->unitsEnd(); ++itr)
		{
			if (itr->second->isEnemy())
			{
				existEnemyUnit = true;
			}
			else
			{
				existPlayerUnit = true;
			}

			if (existEnemyUnit && existPlayerUnit)
			{
				return;
			}
		}

		if (!existPlayerUnit)
		{
			winner_ = Winner::ENEMY;
		}
		else if (!existEnemyUnit)
		{
			winner_ = Winner::PLAYER;
		}
	}
}