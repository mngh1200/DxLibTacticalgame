#include "Unit.h"

namespace Entity {
	/**
	 * @fn
	 * �R���X�g���N�^ �e�X�g����
	 */
	Unit::Unit() : 
		x_(0), y_(0), imageId_(0), animation_{},
		hp_(0), hpm_(0), atk_(0), def_(0), mov_(0),
		isSelected_(false)
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();
		
		imageId_ = rm.getResource(Utility::ResourceManager::ResourceType::PLAYER, rm.LANCER, 0);

		shape_.set(x_ * CHIP_SIZE, y_ * CHIP_SIZE, CHIP_SIZE, CHIP_SIZE);
	}

	/**
	 * @fn
	 * �`��
	 */
	void Unit::render() const
	{
		DxLib::DrawGraph(shape_.x, shape_.y, imageId_, TRUE);
	}

	/**
	 * @fn
	 * �A�j���[�V�����X�V
	 */
	bool Unit::animationUpdate()
	{
		return false;
	}

	/**
	 * @fn
	 * �ړ�
	 */
	void Unit::move(int x, int y)
	{
		x_ = x;
		y_ = y;
		joinAnimationList(AnimationKind::MOVE);
	}

	void Unit::damage(int damage)
	{
		hp_ -= damage;

		if (hp_ <= 0)
		{
			hp_ = 0;
			dead();
		}
		else
		{
			joinAnimationList(AnimationKind::DAMAGE);
		}
	}

	void Unit::dead()
	{
		joinAnimationList(AnimationKind::DESTROY);
	}

	void Unit::select(bool isSelect)
	{
		isSelected_ = isSelect;
	}

	bool Unit::createAnimation(int animationId)
	{
		return false;
	}

}