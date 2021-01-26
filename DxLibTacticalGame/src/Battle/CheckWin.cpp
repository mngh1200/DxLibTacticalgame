#include "CheckWin.h"

namespace Battle {

	/**
	 * @fn
	 * ���s����
	 * @param (map) Map�̃X�}�[�g�|�C���^ 
	*/
	void CheckWin::checkWin(shared_ptr<Map> map)
	{
		bool existEnemyUnit = false;
		bool existPlayerUnit = false;

		// �ǂ��炩�̃��j�b�g���S�Ă��Ȃ��Ȃ��Ă��邩�`�F�b�N
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