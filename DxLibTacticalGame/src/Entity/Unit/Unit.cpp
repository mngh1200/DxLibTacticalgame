#include "Unit.h"

namespace Entity {
	/**
	 * @fn
	 * ��������
	 */
	void Unit::init(int x, int y, bool isEnemy)
	{
		setPos(x, y);

		isEnemy_ = isEnemy;

		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		if (isEnemy)
		{
			imageId_ = rm.getImage(ImageType::PLAYER, LANCER, 0);
		}
		else
		{
			imageId_ = rm.getImage(ImageType::PLAYER, LANCER, 2);
		}
	}

	/**
	 * @fn
	 * �`��
	 */
	void Unit::render() const
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		if (state_ == State::SELECTED)
		{
			// �I�𒆂̕\��(�e�X�g)
			DxLib::DrawBox(shape_.x, shape_.y, shape_.getX2(), shape_.getY2(), rm.getColor(ColorType::POSITIVE_COLOR), FALSE);
		}
		DxLib::DrawGraph(shape_.x, shape_.y, imageId_, TRUE);
	}

	/**
	 * @fn
	 * �A�j���[�V�����X�V
	 */
	bool Unit::animationUpdate()
	{
		// �ړ�
		if (animationId_ == AnimationKind::MOVE)
		{
			if (animation_.update(&shape_.x, &shape_.y, Map::getRealX(baseX_), Map::getRealY(baseY_), Map::getRealX(x_), Map::getRealY(y_)))
			{
				state_ = State::SELECTED;
				shape_.disabledHit = false;
				setPos(x_, y_); // �e�X�g����
				return true;
			}
		}
		return false;
	}

	/**
	 * @fn
	 * �A�j���[�V�����쐬
	 * @param (animationId) �A�j���[�V�����̎��
	 */
	bool Unit::createAnimation(int animationId)
	{
		if (animationId == AnimationKind::MOVE) // �ړ�
		{
			animation_ = Animation(100);
			shape_.disabledHit = true; // �C�x���g����
			return true;
		}
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

	void Unit::setPos(int x, int y)
	{
		baseX_ = x_ = x;
		baseY_ = y_ = y;
		shape_.set(Map::getRealX(x_), Map::getRealY(y_), CHIP_SIZE, CHIP_SIZE);
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

	bool Unit::select(bool isSelect)
	{
		if (isSelect && state_ == State::NORMAL)
		{
			state_ = State::SELECTED;
			return true;
		}
		else if (!isSelect)
		{	
			if (state_ == State::MOVE || state_ == State::SELECTED)
			{
				state_ = State::NORMAL;
				setPos(baseX_, baseY_);
				animation_.forceFinish();
				return true;
			}
		}
		return false;
	}
}