#include "Map.h"
#include "Entity/Unit/Unit.h"

namespace Entity {
	/**
	 * @fn
	 * �R���X�g���N�^
	 */
	Map::Map()
	{
		weak_ptr<Unit> tmpUnit;

		if (tmpUnit.expired()) {
			int a = 10;
		}

		// �e�X�g����
		w_ = 20;
		h_ = 10;

		shape_.set(0, 0, w_ * CHIP_SIZE, h_ * CHIP_SIZE);

		mass_.reserve(h_); // �������m��

		// �}�b�v�f�[�^����
		for (int y = 0; y < h_; y++)
		{
			mass_.push_back(vector<shared_ptr<Mass>>());
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

				mass_[y].push_back(make_shared<Mass>(kind));
			}
		}
	}

	/**
	 * @fn
	 * �`��
	 */
	void Map::render() const
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		int x = 0;
		int y = 0;

		for (auto line = mass_.begin(); line != mass_.end(); ++line) {
			for (auto cell = (*line).begin(); cell != (*line).end(); ++cell) {
				int realX = getRealX(x);
				int realY = getRealY(y);
				DxLib::DrawGraph(realX, realY, (*cell)->getImageId(), FALSE);

				// �e�X�g����
				if ((*cell)->state == Mass::State::MOVABLE)
				{
					DxLib::DrawBox(realX, realY, realX + CHIP_SIZE, realY + CHIP_SIZE, rm.getColor(ColorType::PLAYER_COLOR), FALSE);

					DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, 50);
					DxLib::DrawBox(realX, realY, realX + CHIP_SIZE, realY + CHIP_SIZE, rm.getColor(ColorType::PLAYER_COLOR), TRUE);
					DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
				}
				++x;
			}
			++y;
			x = 0;
		}
	}

	shared_ptr<Mass> Map::getMass(int x, int y)
	{
		if (isRange(x, y))
		{
			try
			{
				return mass_.at(y).at(x);
			}
			catch (out_of_range&) {}
		}

		shared_ptr<Mass> mass = make_shared<Mass>();
		return mass;
	}

	void Map::clearMassState()
	{
		for (auto line = mass_.begin(); line != mass_.end(); ++line) {
			for (auto cell = (*line).begin(); cell != (*line).end(); ++cell) {
				(*cell)->state = Mass::State::NORMAL;
				(*cell)->passingMov = -1;
			}
		}
	}

	bool Map::isRange(int x, int y) const
	{
		return 0 <= x && x < w_ && 0 <= y && y < h_;
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