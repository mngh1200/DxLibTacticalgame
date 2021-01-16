#include "Unit.h"

namespace Entity {
	const vector<string> Unit::LEN_TEXT = {"�Z", "��", "��"};
	float Unit::animatinTimeRate = 1.0f;

	/**
	 * @fn
	 * ��������
	 */
	void Unit::init(int x, int y, int kind, bool isEnemy)
	{
		type_ = Figure::UNIT;

		setPos(x, y);

		isEnemy_ = isEnemy;

		setKind(kind);
	}

	/**
	 * @fn
	 * ���j�b�g�̎�ނ��Z�b�g
	 */
	void Unit::setKind(int kind)
	{
		info_.kind = kind;

		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		if (kind == UnitKey::CAVALRY) // �R��
		{
			info_.name = "�R��";
			info_.hpm = 30;
			info_.atk = 8;
			info_.def = 1;
			info_.mov = 4;
			info_.len = 0;
			info_.ability = Ability::Kind::RUSH;
		}
		else if (kind == UnitKey::GUNNER) // �e��
		{
			info_.name = "�e��";
			info_.hpm = 20;
			info_.atk = 9;
			info_.def = 0;
			info_.mov = 2;
			info_.len = 2;
			info_.range = 3;
			info_.ability = Ability::Kind::THROUGH;
		}
		else // kind == UnitKey::LANCER �̓��� // ����
		{
			info_.name = "����";
			info_.hpm = 30;
			info_.atk = 6;
			info_.def = 2;
			info_.mov = 2;
			info_.len = 1;
			info_.kind = UnitKey::LANCER;
			info_.ability = Ability::Kind::AMBUSH;
		}

		viewHp_ = info_.hp = info_.hpm;

		imageId_ = rm.getImage(ImageType::PLAYER, info_.kind, isEnemy_ ? 0 : 2);
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
			// Map::drawMoveableMass(shape_.x, shape_.y);
			Map::drawSelectedMass(shape_.x, shape_.y);
		}

		if (isMouseOver_) // �z�o�[��
		{
			Map::drawHoverMass(shape_.x, shape_.y);
		}

		if (alpha_ != 255) // �s�����x
		{
			DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_);
		}

		DxLib::DrawGraph(shape_.x, shape_.y, imageId_, TRUE);

		// HP�o�[
		int hpx1 = shape_.x + HP_PADDING;
		int hpy1 = shape_.y + HP_Y;
		int hpx2 = shape_.getX2() - HP_PADDING;
		int hpy2 = hpy1 + HP_H;

		int hpw = (int)((float)(CHIP_SIZE - HP_PADDING * 2) * ((float)viewHp_ / (float)info_.hpm));

		DxLib::DrawBox(hpx1, hpy1, hpx2, hpy2, rm.getColor(ColorType::NEGATIVE_COLOR), TRUE);
		DxLib::DrawBox(hpx1, hpy1, hpx1 + hpw, hpy2, rm.getColor(ColorType::POSITIVE_LITE_COLOR), TRUE);

		if (alpha_ != 255) // �`�惂�[�h��W���ɖ߂�
		{
			DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
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
			animationSub_.update(&viewHp_, prevHp_, info_.hp);

			int baseX = Map::getRealX(x_);

			if (animation_.update(&shape_.x, baseX - ANIME_DAMAGE_MOVE, baseX + ANIME_DAMAGE_MOVE))
			{
				viewHp_ = info_.hp;
				animationSub_.forceFinish();
				return !checkDead();
			}
		}
		else if (animationId_ == AnimationKind::AVOID) // ���
		{
			int baseX = Map::getRealX(x_);
			int baseY = Map::getRealY(y_);
			return animation_.update(&shape_.x, &shape_.y, baseX, baseY, baseX - shape_.w / 2 , baseY);
		}
		else if (animationId_ == AnimationKind::DESTROY) // ���S
		{
			if (animation_.update(&alpha_, 255, 0))
			{
				destroy();
				return true;
			}
		}
		return false;
	}

	/**
	 * @fn
	 * �w��̔{���Ōv�Z�����A�j���[�V�������Ԃ�Ԃ�
	 * @param (baseMs) ��̃~���b
	 * @return �A�j���[�V�������ԁims�j
	 */
	static int getAnimationMS(int baseMs)
	{
		return (int)(Unit::animatinTimeRate * baseMs);
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
			animation_ = Animation(getAnimationMS(100));
			shape_.disabledHit = true; // �C�x���g����
			return true;
		}
		else if (animationId == AnimationKind::ATACK) // �U��
		{
			animation_ = Animation(getAnimationMS(ANIME_ATACK_MS), Animation::Direction::AlTERNATE, 2);
			return true;
		}
		else if (animationId == AnimationKind::DAMAGE) // �_���[�W
		{
			animation_ = Animation(getAnimationMS(ANIME_DAMAGE_MS / ANIME_DAMAGE_REPEAT), 0, ANIME_DAMAGE_REPEAT, getAnimationMS(ANIME_ATACK_MS));
			int baseX = Map::getRealX(x_);
			animation_.adjustFrame(shape_.x, baseX - ANIME_DAMAGE_MOVE, baseX + ANIME_DAMAGE_MOVE);
			animation_.adjustLastFrame(shape_.x, baseX - ANIME_DAMAGE_MOVE, baseX + ANIME_DAMAGE_MOVE);

			// HP�o�[�A�j���[�V����
			animationSub_ = Animation(getAnimationMS(ANIME_DAMAGE_MS), 0, 1, getAnimationMS(ANIME_ATACK_MS));

			return true;
		}
		else if (animationId == AnimationKind::AVOID) // ���
		{
			animation_ = Animation(getAnimationMS(ANIME_DAMAGE_MS / 2), Animation::Direction::AlTERNATE, 2, getAnimationMS(ANIME_ATACK_MS - 100));
			return true;
		}
		else if (animationId == AnimationKind::DESTROY) // ���S
		{
			animation_ = Animation(getAnimationMS(600));
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


	/**
	 * @fn
	 * �˒��̃e�L�X�g��Ԃ�
	 * @return �e�L�X�g
	 */
	string Unit::getLenText() const
	{
		try
		{
			return LEN_TEXT.at(info_.len);
		}
		catch (out_of_range&) {}
		
		return "";
	}


	/**
	 * @fn
	 * �_���[�W�l
	 * @return ���S�� true��Ԃ�
	 */
	bool Unit::damage(int damage)
	{
		prevHp_ = info_.hp;
		info_.hp -= damage;
		joinAnimationList(AnimationKind::DAMAGE);

		// �_���[�W�G�t�F�N�g
		DamageEffect::makeDamageEffect(shape_.x, shape_.y, damage);
		
		if (info_.hp <= 0)
		{
			info_.hp = 0;
			return true;
		}
		
		return false;
	}

	/**
	 * @fn
	 * ���
	 */
	void Unit::avoid()
	{
		joinAnimationList(AnimationKind::AVOID);
		DamageEffect::makeDamageEffect(shape_.x, shape_.y, DamageEffect::MISS);
	}

	/**
	 * @fn
	 * ���j�b�g���S
	 * @return ���j�b�g���S�� true
	 */
	bool Unit::checkDead()
	{
		if (info_.hp <= 0)
		{
			changeAnimation(AnimationKind::DESTROY); // ���S�A�A�j���[�V�����p��
			return true;
		}
		return false;
		
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
	 * �^�[���I�����̏���
	 */
	void Unit::turnEnd()
	{
		isActed_ = false;
		alpha_ = 255; // �e�X�g����
	}

	/**
	 * @fn
	 * �s���I��
	 */
	void Unit::endAction()
	{
		isActed_ = true;
		alpha_ = 100; // �e�X�g����
	}

	/**
	 * @fn
	 * �U��
	 * @param (targetRealX) �U���Ώۂ�X���W
	 * @param (targetRealY) �U���Ώۂ�Y���W
	 */
	void Unit::atack(int targetRealX, int targetRealY)
	{
		targetRealX_ = shape_.x + (targetRealX - shape_.x) / 2;
		targetRealY_ = shape_.y + (targetRealY - shape_.y) / 2;
		joinAnimationList(AnimationKind::ATACK);
	}
}