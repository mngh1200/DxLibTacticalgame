#include "Unit.h"

namespace Entity {
	/**
	 * @fn
	 * ��������
	 */
	void Unit::init(int x, int y, bool isEnemy)
	{
		type_ = Figure::UNIT;

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

		// �e�X�g�����i���X�e�[�^�X�j
		hpm_ = hp_ = 10;
		atk_ = 2;
		def_ = 1;
		mov_ = 2;
	}

	/**
	 * @fn
	 * �`��
	 */
	void Unit::render() const
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		if (state_ == State::SELECTED) // �I��
		{
			Map::drawMoveableMass(shape_.x, shape_.y);
			Map::drawSelectedMass(shape_.x, shape_.y);
		}

		if (isMouseOver_) // �z�o�[��
		{
			Map::drawHoverMass(shape_.x, shape_.y);
		}

		DxLib::DrawGraph(shape_.x, shape_.y, imageId_, TRUE);

		// HP�o�[
		int hpx1 = shape_.x + HP_PADDING;
		int hpy1 = shape_.y + HP_Y;
		int hpx2 = shape_.getX2() - HP_PADDING;
		int hpy2 = hpy1 + HP_H;

		int hpw = (int)((float)(CHIP_SIZE - HP_PADDING * 2) * ((float)hp_ / (float)hpm_));

		DxLib::DrawBox(hpx1, hpy1, hpx2, hpy2, rm.getColor(ColorType::NEGATIVE_COLOR), TRUE);
		DxLib::DrawBox(hpx1, hpy1, hpx1 + hpw, hpy2, rm.getColor(ColorType::POSITIVE_LITE_COLOR), TRUE);
		
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
				return true;
			}
		}
		else if (animationId_ == AnimationKind::ATACK) // �U��
		{
			int baseX = Map::getRealX(x_);
			int baseY = Map::getRealY(y_);
			return animation_.update(&shape_.x, &shape_.y, baseX, baseY, targetRealX_, targetRealY_);
		}
		else if (animationId_ == AnimationKind::DAMAGE) // �_���[�W
		{
			int baseX = Map::getRealX(x_);
			if (animation_.update(&shape_.x, baseX - ANIME_DAMAGE_MOVE, baseX + ANIME_DAMAGE_MOVE))
			{
				if (hp_ <= 0)
				{
					changeAnimation(AnimationKind::DESTROY); // ���S�A�A�j���[�V�����p��
				}
				else
				{
					return true;
				}
			}
		}
		else if (animationId_ == AnimationKind::DESTROY) // ���S
		{
			if (animation_.increaseFrame())
			{
				destroy();
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
		else if (animationId == AnimationKind::ATACK) // �U��
		{
			animation_ = Animation(ANIME_ATACK_MS, Animation::Direction::AlTERNATE, 2);
			return true;
		}
		else if (animationId == AnimationKind::DAMAGE) // �_���[�W
		{
			animation_ = Animation(ANIME_DAMAGE_MS, 0, 4, ANIME_ATACK_MS / 2);
			int baseX = Map::getRealX(x_);
			animation_.adjustFrame(shape_.x, baseX - ANIME_DAMAGE_MOVE, baseX + ANIME_DAMAGE_MOVE);
			animation_.adjustLastFrame(shape_.x, baseX - ANIME_DAMAGE_MOVE, baseX + ANIME_DAMAGE_MOVE);
			return true;
		}
		else if (animationId == AnimationKind::DESTROY) // ���S
		{
			animation_ = Animation(200);
			return true;
		}
		return false;
	}

	/**
	 * @fn
	 * �ړ�
	 * @param (x) �ړ����x���W(�}�X�)
	 * @param (y) �ړ����y���W(�}�X�)
	 */
	void Unit::move(int x, int y)
	{
		x_ = x;
		y_ = y;
		joinAnimationList(AnimationKind::MOVE);
	}

	/**
	 * @fn
	 * �ړ����ɖ߂�
	 */
	void Unit::back()
	{
		setPos(baseX_, baseY_);
	}

	/**
	 * @fn
	 * �ʒu�w��
	 * @param (x) x���W(�}�X�)
	 * @param (y) y���W(�}�X�)
	 */
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
		}
		joinAnimationList(AnimationKind::DAMAGE);
	}

	/**
	 * @fn
	 * �I���܂��͑I������
	 * @param (isSelect) �I������Ƃ���true�A�I����������Ƃ���false
	 * @return �I����Ԃ��ύX���ꂽ�Ƃ���true��Ԃ�
	 */
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
				if (animationId_ == AnimationKind::MOVE)
				{
					animation_.forceFinish
					();
				}
				
				return true;
			}
		}
		return false;
	}

	/**
	 * @fn
	 * �U��
	 * @return �U����
	 */
	int Unit::atack(int targetRealX, int targetRealY)
	{
		targetRealX_ = shape_.x + (targetRealX - shape_.x) / 2;
		targetRealY_ = shape_.y + (targetRealY - shape_.y) / 2;
		joinAnimationList(AnimationKind::ATACK);
		return getAtk();
	}
}