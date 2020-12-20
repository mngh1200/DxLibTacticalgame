#include "Mass.h"
#include "Map.h"
#include "Entity/Unit/Unit.h"

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
	 * �}�X�ɔz�u���ꂽ���j�b�g���Z�b�g
	 * @param (unit) ���j�b�g
	 */
	void Mass::setUnit(shared_ptr<Entity::Unit> unit)
	{
		unit_ = unit;
	}

	/**
	 * @fn
	 * �}�X�ɔz�u���ꂽ���j�b�g�̎Q�Ƃ��폜
	 */
	void Mass::resetUnit()
	{
		unit_.reset();
	}

	/**
	 * @fn
	 * �}�X�ɔz�u���ꂽ���j�b�g���擾
	 * @return ���j�b�g
	 */
	shared_ptr<Entity::Unit> Mass::getUnit() const
	{
		return unit_.lock();
	}

	/**
	 * @fn
	 * �摜�̎�ނ����ɉ摜ID���Z�b�g
	 * @param (kindId) �摜�̎��
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