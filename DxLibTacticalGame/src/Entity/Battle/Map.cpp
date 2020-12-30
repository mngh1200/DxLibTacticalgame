#include "Map.h"
#include "Entity/Unit/Unit.h"

namespace Entity {
	/**
	 * @fn
	 * �R���X�g���N�^
	 */
	Map::Map()
	{
		type_ = Object::Type::MAP;

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

		int mouseX = -1;
		int mouseY = -1;

		if (isMouseOver_) // �z�o�[���̂ݎ擾
		{
			DxLib::GetMousePoint(&mouseX, &mouseY);
			mouseX = getMassX(mouseX);
			mouseY = getMassY(mouseY);
		}

		for (auto line = mass_.begin(); line != mass_.end(); ++line) {
			for (auto cell = (*line).begin(); cell != (*line).end(); ++cell) {
				int realX = getRealX(x);
				int realY = getRealY(y);
				DxLib::DrawGraph(realX, realY, (*cell)->getImageId(), FALSE);

				
				if (isMouseOver_) // �z�o�[���̕`��
				{
					if (x == mouseX && y == mouseY)
					{
						drawHoverMass(realX, realY);
					}
				}

				if ((*cell)->state == Mass::State::NORMAL)
				{
					// �������Ȃ�
				}
				if ((*cell)->state == Mass::State::MOVABLE) // �ړ��͈�
				{
					drawMoveableMass(realX, realY);
				}
				else if ((*cell)->state == Mass::State::ATK_ABLE) // �U���\�͈�
				{
					drawAtackMass(realX, realY);
				}
				++x;
			}
			++y;
			x = 0;
		}
	}

	/**
	 * @fn
	 * �}�X�̃|�C���^��Ԃ�
	 * @param (x) �}�X��x���W
	 * @param (y) �}�X��y���W
	 * @return �}�X
	 */
	shared_ptr<Mass> Map::getMass(int massX, int massY)
	{
		if (isRange(massX, massY))
		{
			try
			{
				return mass_.at(massY).at(massX);
			}
			catch (out_of_range&) {}
		}

		shared_ptr<Mass> mass = make_shared<Mass>();
		return mass;
	}

	/**
	 * @fn
	 * �}�X�̏�Ԃ�����������
	 */
	void Map::clearMassState()
	{
		for (auto line = mass_.begin(); line != mass_.end(); ++line) {
			for (auto cell = (*line).begin(); cell != (*line).end(); ++cell) {
				(*cell)->state = Mass::State::NORMAL;
				(*cell)->passingMov = -1;
			}
		}
	}

	/**
	 * @fn
	 * �}�b�v�͈͓��ł��邩����
	 * @param (x) �}�X��x���W
	 * @param (y) �}�X��y���W
	 * @return �͈͓��ł���ꍇ��true��Ԃ�
	 */
	bool Map::isRange(int x, int y) const
	{
		return 0 <= x && x < w_ && 0 <= y && y < h_;
	}


	/**
	 * @fn
	 * �ړ��\�͈͂̃}�X��`��
	 * @param (realX) x���W
	 * @param (realY) y���W
	 */
	void Map::drawMoveableMass(int realX, int realY)
	{
		DxLib::DrawGraph(realX, realY, Utility::ResourceManager::getInstance().getImage(ImageType::IMAGE, ImageId::MASS_MOVE), TRUE);

		//DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, 50);
		//DxLib::DrawBox(realX, realY, realX + CHIP_SIZE, realY + CHIP_SIZE, rm.getColor(ColorType::PLAYER_COLOR), TRUE);
		//DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		//DxLib::DrawBox(realX, realY, realX + CHIP_SIZE, realY + CHIP_SIZE, rm.getColor(ColorType::PLAYER_COLOR), FALSE);
	}

	/**
	 * @fn
	 * �U���\�͈͂̃}�X��`��
	 * @param (realX) x���W
	 * @param (realY) y���W
	 */
	void Map::drawAtackMass(int realX, int realY)
	{
		DxLib::DrawGraph(realX, realY, Utility::ResourceManager::getInstance().getImage(ImageType::IMAGE, ImageId::MASS_ATACK), TRUE);
	}

	/**
	 * @fn
	 * �z�o�[�}�X��`��
	 * @param (realX) x���W
	 * @param (realY) y���W
	 */
	void Map::drawHoverMass(int realX, int realY)
	{
		DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, MASS_EFFECT_ALPHA);
		DxLib::DrawBox(realX, realY, realX + CHIP_SIZE, realY + CHIP_SIZE, Utility::ResourceManager::getInstance().getColor(ColorType::MAIN_COLOR), TRUE);
		DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	/**
	 * @fn
	 * �I�𒆂̃}�X��`��
	 * @param (realX) x���W
	 * @param (realY) y���W
	 */
	void Map::drawSelectedMass(int realX, int realY)
	{
		DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, MASS_EFFECT_ALPHA);
		DxLib::DrawBox(realX, realY, realX + CHIP_SIZE, realY + CHIP_SIZE, Utility::ResourceManager::getInstance().getColor(ColorType::MAIN_COLOR_ON), TRUE);
		DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
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