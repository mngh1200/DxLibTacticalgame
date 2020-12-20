#include "Map.h"
#include "Entity/Unit/Unit.h"

namespace Entity {
	/**
	 * @fn
	 * �R���X�g���N�^
	 */
	Map::Map()
	{
		// �e�X�g����
		w_ = 20;
		h_ = 10;

		shape_.set(0, 0, w_ * CHIP_SIZE, h_ * CHIP_SIZE);

		mass_.reserve(h_); // �������m��

		// �}�b�v�f�[�^����
		for (int y = 0; y < h_; y++)
		{
			mass_.push_back(vector<Mass>());
			mass_[y].reserve(w_); // �������m��
			
			for (int x = 0; x < w_; x++)
			{
				int kind = Mass::Kind::PLAIN;

				if (x == 0 || x == w_ - 1)
				{
					kind = Mass::Kind::MOUNTAIN;
				}
				else if (y == 5)
				{
					kind = Mass::Kind::RIVER;
				}
				else if (x == 5)
				{
					kind = Mass::Kind::FOREST;
				}

				mass_[y].push_back(Mass(kind));
			}
		}
	}

	/**
	 * @fn
	 * �`��
	 */
	void Map::render() const
	{
		for (int y = 0; y < h_; y++)
		{
			for (int x = 0; x < w_; x++)
			{
				try {
					DxLib::DrawGraph(getRealX(x), getRealY(y), mass_[y][x].getImageId(), FALSE);
				}
				catch (std::out_of_range& oor) {
					std::cerr << "Out of Range: " << oor.what() << std::endl;
				}
				
			}
		}
	}

	/**
	 * @fn
	 * �}�X�Ƀ��j�b�g�ւ̎Q�Ƃ��Z�b�g
	 * @param (x) x���W�i�}�X�j
	 * @param (y) y���W�i�}�X�j
	 */
	void Map::setUnit(int x, int y, shared_ptr<Entity::Unit> unit)
	{
		try {
			mass_[y][x].setUnit(unit);
		}
		catch (std::out_of_range& oor) {
			std::cerr << "Out of Range: " << oor.what() << std::endl;
		}
		
	}

	void Map::onClickPlayerUnit(int x, int y)
	{
		if (deselectUnit()) // �I������
		{
			// ���j�b�g�I��
			shared_ptr<Entity::Unit> unit = mass_[getMassX(y)][getMassX(x)].getUnit();
			if (unit && unit->select(true))
			{
				selectedUnit_ = unit;
			}
		}
	}

	void Map::onClickMass(int x, int y)
	{
		shared_ptr<Entity::Unit> selectedUnitSp = selectedUnit_.lock();
		if (selectedUnitSp)
		{
			// �ړ�
			selectedUnitSp->move(Entity::Map::getMassX(x), Entity::Map::getMassY(y));
		}
	}

	/**
	 * @fn
	 * ���j�b�g�̑I����Ԃ�����
	 * @return �����ł����ꍇ�ƑI���ς݂̃��j�b�g���Ȃ��ꍇ��true��Ԃ�
	*/
	bool Map::deselectUnit()
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

	/**
	 * @fn
	 * �}�X�̍��W���琳���̍��W���擾(x���W)
	 */
	int Map::getRealX(int massX)
	{
		return massX * CHIP_SIZE;
	}

	/**
	 * @fn
	 * �}�X�̍��W���琳���̍��W���擾(y���W)
	 */
	int Map::getRealY(int massY)
	{
		return massY * CHIP_SIZE;
	}

	/**
	 * @fn
	 * �����̍��W����}�X�̍��W���擾(x���W)
	 */
	int Map::getMassX(int realX)
	{
		return realX / CHIP_SIZE;
	}

	/**
	 * @fn
	 * �����̍��W����}�X�̍��W���擾(y���W)
	 */
	int Map::getMassY(int realY)
	{
		return realY / CHIP_SIZE;
	}

}