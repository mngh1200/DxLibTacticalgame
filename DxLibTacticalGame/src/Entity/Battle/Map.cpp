#include "Map.h"

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
				DxLib::DrawGraph(x * CHIP_SIZE, y * CHIP_SIZE, mass_[y][x].getImageId(), FALSE);
			}
		}
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