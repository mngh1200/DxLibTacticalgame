#include "Mass.h"
#include "Map.h"
#include "Entity/Unit/Unit.h"

namespace Entity {
	const vector<int> Mass::COST = { 1, 2, 2, INT_MAX, 1, 1 };
	const vector<int> Mass::AGL_UP = { 0, 20, -20, 0, 0, 0 };
	const vector<int> Mass::DEF_UP = { 0,  1,  -2, 0, 2, 3 };
	const vector<string> Mass::TEXT1 = {"����", "�X", "��", "�R", "�ԁi���R�j", "�ԁi�G�R�j"};

	/**
	 * @fn
	 * �R���X�g���N�^
	 * @param (kindId) �}�X�̎��
	 */
	Mass::Mass(int kindId) : Mass()
	{
		kindId_ = kindId;

		if (0 > kindId || kindId >= Kind::LEN)
		{
			kindId_ = Kind::PLAIN;
		}

		// setImageIdFromKind(kindId_, adjacent);
	}


	/**
	 * @fn
	 * ��������
	 * @param (adjacent) ����ރ}�X�Ƃ̗אڏ��
	 */
	void Mass::init(int adjacent)
	{
		setImageIdFromKind(kindId_, adjacent);
	}

	/**
	 * @fn
	 * �ړ��R�X�g�擾
	 * @return �ړ��R�X�g
	 */
	int Mass::getCost() const
	{
		try
		{
			return COST.at(kindId_);
		}
		catch (out_of_range&) {}
		
		return INT_MAX;
	}

	/**
	 * @fn
	 * ���UP�擾
	 * @return ���UP
	 */
	int Mass::getAgl() const
	{
		try
		{
			return AGL_UP.at(kindId_);
		}
		catch (out_of_range&) {}

		return 0;
	}

	/**
	 * @fn
	 * ���UP�擾
	 * @return ���UP
	 */
	int Mass::getDef() const
	{
		try
		{
			return DEF_UP.at(kindId_);
		}
		catch (out_of_range&) {}

		return 0;

	}

	/**
	 * @fn
	 * �}�X�̎�ނ�Ԃ�
	 * @return �}�X�̎��
	 */
	int Mass::getKind() const
	{
		return kindId_;
	}

	/**
	 * @fn
	 * �摜��ID��Ԃ�
	 * @return �摜ID
	 */
	int Mass::getImageId() const
	{
		return imageId_;
	}

	/**
	 * @fn
	 * �n�`���ʐ�������Ԃ�
	 * @param (line) ���s�ڂ̐��������擾���邩
	 * @return ������
	 */
	string Mass::getText(int line) const
	{
		if (kindId_ == Kind::OUT_OF_MAP)
		{
			return "";
		}

		if (line == 0)
		{
			return TEXT1.at(kindId_);
		}
		else if (line == 1)
		{
			int agl = getAgl();
			int def = getDef();

			if (getCost() == INT_MAX)
			{
				return "�N���s��";
			} 
			else if (agl == 0 && def == 0)
			{
				return "���ɂȂ�";
			}
			string aglSign = agl >= 0 ? "+" : "";
			string defSign = def >= 0 ? "+" : "";

			return "��� " + aglSign + to_string(agl) + " �h�� " + defSign + to_string(def);
		}

		return "";
	}

	/**
	 * @fn
	 * ��摜�̏��擾
	 * @param (adjacent) ����ނƂ̗אڏ��
	 * @param (imageId) �摜ID�擾�p
	 * @param (angle) �摜�̉�]�ʎ擾�p
	 */
	void getRiverImage(int adjacent, int* imageId, double* angle)
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		int imgPos = 0;

		if (adjacent == (Direction::TOP + Direction::RIGHT + Direction::BOTTOM + Direction::LEFT))
		{
			imgPos = 3; // �S����
		}
		else if (adjacent & Direction::TOP && adjacent & Direction::BOTTOM) // �c�����ɗא�
		{
			if (adjacent & Direction::LEFT)
			{
				imgPos = 2;
				*angle = - PI / 2.0;
			}
			else if (adjacent & Direction::RIGHT)
			{
				imgPos = 2;
				*angle = PI / 2.0;
			}
			else
			{
				*angle = PI / 2.0;
			}
		}
		else if (adjacent & Direction::LEFT && adjacent & Direction::RIGHT) // �������ɗא�
		{
			if (adjacent & Direction::TOP)
			{
				imgPos = 2;
			}
			else if (adjacent & Direction::BOTTOM)
			{
				imgPos = 2;
				*angle = PI;
			}
		}
		else if (adjacent & Direction::TOP) // �ȉ� �����ɂQ��������
		{
			if (adjacent & Direction::LEFT)
			{
				imgPos = 1;
			}
			else if (adjacent & Direction::RIGHT)
			{
				imgPos = 1;
				*angle = PI / 2.0;
			}
			else
			{
				*angle = PI / 2.0;
			}
		}
		else if (adjacent & Direction::BOTTOM)
		{
			if (adjacent & Direction::LEFT)
			{
				imgPos = 1;
				*angle = - PI / 2.0;
			}
			else if (adjacent & Direction::RIGHT)
			{
				imgPos = 1;
				*angle = PI;
			}
			else
			{
				*angle = PI / 2.0;
			}
		}

		*imageId = rm.getImage(ImageType::MAP, MapImageKind::RIVER, imgPos);
	}

	/**
	 * @fn
	 * �摜�̎�ނ����ɉ摜ID���Z�b�g
	 * @param (kindId) �摜�̎��
	 */
	void Mass::setImageIdFromKind(int kindId, int adjacent)
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		if (kindId == Kind::MOUNTAIN) // �R
		{
			imageId_ = rm.getImage(ImageType::MAP, MapImageKind::MOUNTAIN, adjacent);
		}
		else if (kindId == Kind::FOREST) // �X
		{
			imageId_ = rm.getImage(ImageType::MAP, MapImageKind::ALL, MapChipPos::FOREST_POS);
		}
		else if (kindId == Kind::RIVER) // ��
		{
			getRiverImage(adjacent, &imageId_, &angle_);
		}
		else if (kindId == Kind::FORT_PLAYER) // �ԁi���R�j
		{
			imageId_ = rm.getImage(ImageType::MAP, MapImageKind::ALL, MapChipPos::PLAYER_FORT_POS);
		}
		else if (kindId == Kind::FORT_ENEMY) // �ԁi�G�R�j
		{
			imageId_ = rm.getImage(ImageType::MAP, MapImageKind::ALL, MapChipPos::ENEMY_FORT_POS);
		}
		else // ����
		{
			imageId_ = rm.getImage(ImageType::MAP, MapImageKind::ALL, MapChipPos::PLAIN_POS);
		}
	}
}