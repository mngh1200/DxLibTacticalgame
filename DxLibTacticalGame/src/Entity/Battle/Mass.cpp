#include "Mass.h"
#include "Map.h"

namespace Entity {

	/**
	 * @fn
	 * �R���X�g���N�^
	 */
	Mass::Mass(int kindId) : kindId_(kindId)
	{
		if (0 > kindId_ || kindId_ >= Kind::LEN)
		{
			kindId_ = Kind::PLAIN;
		}

		setImageIdFromKind(kindId);
	}

	/**
	 * @fn
	 * �ړ��R�X�g�擾
	 */
	int Mass::getCost() const
	{
		return COST[kindId_];
	}

	/**
	 * @fn
	 * ���UP�擾
	 */
	int Mass::getAgl() const
	{
		return AGL_UP[kindId_];
	}

	/**
	 * @fn
	 * ���UP�擾
	 */
	int Mass::getDef() const
	{
		return DEF_UP[kindId_];
	}

	/**
	 * @fn
	 * �}�X�̎�ނ�Ԃ�
	 */
	int Mass::getKind() const
	{
		return kindId_;
	}

	/**
	 * @fn
	 * �摜��ID��Ԃ�
	 */
	int Mass::getImageId() const
	{
		return imageId_;
	}

	/**
	 * @fn
	 * �摜�̎�ނ����ɉ摜ID���Z�b�g
	 */
	void Mass::setImageIdFromKind(int kindId)
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		// �e�X�g����
		if (kindId == Kind::MOUNTAIN) // �R
		{
			imageId_ = rm.getImage(ImageType::MAP, 0, 0);
		}
		else if (kindId == Kind::FOREST) // �X
		{
			imageId_ = rm.getImage(ImageType::MAP, 0, 2);
		}
		else if (kindId == Kind::RIVER) // ��
		{
			imageId_ = rm.getImage(ImageType::MAP, 0, 3);
		}
		else // ����
		{
			imageId_ = rm.getImage(ImageType::MAP, 0, 1);
		}
	}
}