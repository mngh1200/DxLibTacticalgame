#include "BattleManager.h"

namespace Battle {
	/**
	 * @fn
	 * �R���X�g���N�^
	 */
	BattleManager::BattleManager(shared_ptr<Entity::Map> map)
	{
		map_ = map;
	}

	/**
	 * @fn
	 * �}�X�Ƀ��j�b�g�ւ̎Q�Ƃ��Z�b�g
	 * @param (unit) ���j�b�g�̎Q��
	 * @return �ǉ����� : true
	 */
	bool BattleManager::setUnit(shared_ptr<Entity::Unit> unit)
	{
		int x = unit->getX();
		int y = unit->getY();

		if (map_->isRange(x, y))
		{
			auto ret = mapUnits_.emplace(make_pair(x, y), unit); // �V�K�ǉ��̂�
			return ret.second;
		}
		return false;
	}

	void BattleManager::onClickPlayerUnit(int x, int y)
	{
		if (deselectUnit()) // �I������
		{
			// ���j�b�g�I��
			shared_ptr<Entity::Unit> unit = getUnitWp(Map::getMassX(x), Map::getMassY(y)).lock();
			if (unit && unit->select(true))
			{
				selectedUnit_ = unit;
			}
		}
	}

	void BattleManager::onClickMass(int x, int y)
	{
		shared_ptr<Entity::Unit> selectedUnitSp = selectedUnit_.lock();
		if (selectedUnitSp)
		{
			// �ړ�
			selectedUnitSp->move(Map::getMassX(x), Map::getMassY(y));
		}
	}

	weak_ptr<Unit> BattleManager::getUnitWp(int massX, int massY)
	{
		try
		{
			return mapUnits_.at(make_pair(massX, massY));
		}
		catch (out_of_range&) {}

		return weak_ptr<Unit>();
	}

	/**
	 * @fn
	 * ���j�b�g�̑I����Ԃ�����
	 * @return �����ł����ꍇ�ƑI���ς݂̃��j�b�g���Ȃ��ꍇ��true��Ԃ�
	*/
	bool BattleManager::deselectUnit()
	{
		if (selectedUnit_.expired())
		{
			return true;
		}
		shared_ptr<Entity::Unit> prevSelectedUnit = selectedUnit_.lock();
		if (prevSelectedUnit)
		{
			if (prevSelectedUnit->select(false))
			{
				selectedUnit_.reset();
				return true;
			}
			return false;
		}

		// �I���ς݂̃��j�b�g���Ȃ��ꍇ��ture��Ԃ�
		return true;
	}
}