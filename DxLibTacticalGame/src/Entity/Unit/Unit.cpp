#include "Unit.h"

namespace Entity {
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
			info_.atk = 9;
			info_.def = 3;
			info_.mov = 4;
			info_.len = 0;
			info_.ability = Ability::Kind::RUSH;
		}
		else if (kind == UnitKey::GUNNER) // �e��
		{
			info_.name = "�e��";
			info_.hpm = 20;
			info_.atk = 14;
			info_.def = 2;
			info_.mov = 2;
			info_.len = 2;
			info_.range = 3;
			info_.isCloseAtack = false;
			info_.ability = Ability::Kind::THROUGH;
		}
		else // kind == UnitKey::LANCER �̓��� // ����
		{
			info_.name = "����";
			info_.hpm = 30;
			info_.atk = 9;
			info_.def = 4;
			info_.mov = 2;
			info_.len = 1;
			info_.kind = UnitKey::LANCER;
			info_.ability = Ability::Kind::AMBUSH;
		}

		viewHp_ = info_.hp = info_.hpm;

		setImage(UnitImageKind::NORMAL);
	}

	/**
	 * @fn
	 * ���j�b�g�̏�Ԃ����ɉ摜�Z�b�g
	 * @param (state) ���
	 */
	void Unit::setImage(int state)
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();
		imageId_ = rm.getImage(ImageType::UNIT, state, !isEnemy_ ? info_.kind : info_.kind + UnitKey::UNIT_KIND_LEN);
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

		renderExtra();
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
			return updateAtackAnimation();
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
	 * �U���A�j���[�V�����̍X�V����
	 * @return �A�j���[�V�������ԁims�j
	 */
	bool Unit::updateAtackAnimation()
	{
		int baseX = Map::getRealX(x_);
		int baseY = Map::getRealY(y_);
		int targetRealX = baseX + (targetRealX_ - baseX) / 2;
		int targetRealY = baseY + (targetRealY_ - baseY) / 2;
		return animation_.update(&shape_.x, &shape_.y, baseX, baseY, targetRealX, targetRealY);
	}

	/**
	 * @fn
	 * �w��̔{���Ōv�Z�����A�j���[�V�������Ԃ�Ԃ�
	 * @param (baseMs) ��̃~���b
	 * @return �A�j���[�V�������ԁims�j
	 */
	int Unit::getAnimationMS(int baseMs)
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

			if (x_ != baseX_ || y_ != baseY_)
			{
				Utility::ResourceManager::playSound(SoundKind::MOVE);
			}

			return true;
		}
		else if (animationId == AnimationKind::ATACK) // �U��
		{
			createAtackAnimation();
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
	 * �U�����A�j���[�V�����쐬
	 */
	void Unit::createAtackAnimation()
	{
		Utility::ResourceManager::playSound(SoundKind::ATTACK_CLOSE);
		animation_ = Animation(getAnimationMS(ANIME_ATACK_MS), Animation::Direction::AlTERNATE, 2);
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
	 * ����X�e�[�^�X�̃��x���Ɛ�������Ԃ�
	 * @param (list) �Q�Ɠn���p�̕ϐ� <���x��, ������>
	 */
	void Unit::getExtraStatusList(vector<pair<string, string>>& list) const
	{
		// �X�L��
		list.push_back(make_pair(info_.ability.getLabel(), info_.ability.getDescription()));

		// �R�n���j�b�g�ł��邩
		if (isHorse())
		{
			list.push_back(make_pair("�R�n", "�X�тł̈ړ��� -1"));
		}

		getExtraStatusListExtra(list);
	}

	/**
	 * @fn
	 * �_���[�W�l
	 * @param (damage) �_���[�W��
	 * @param (direction) �U�����ꂽ����
	 * @param (isCloseAttack) �ߐڍU���ł��邩
	 * @return ���S�� true��Ԃ�
	 */
	bool Unit::damage(int damage, int direction, bool isCloseAtack)
	{
		prevHp_ = info_.hp;
		info_.hp -= damage;
		joinAnimationList(AnimationKind::DAMAGE);

		// �U�����ꂽ���������O�Ɏc��
		if (isCloseAtack)
		{
			if (!(closeAttackedLogs_ & direction)) // ���ɒǉ�����Ă���ꍇ�͒ǉ����Ȃ�
			{
				closeAttackedLogs_ += direction;
			}
		}

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
	 * HP���V�X�e����̏����ŋ����I�ɍ��ꍇ�̃��\�b�h
	 * @param (decrease) ������
	 */
	void Unit::forceDecreaseHp(int decrease)
	{
		info_.hp -= decrease;
		viewHp_ = info_.hp;
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
	 * @param (isOwnEnd) �^�[���G���h�������̃��j�b�g�̏ꍇtrue
	 */
	void Unit::turnEnd(bool isOwnEnd)
	{
		isActed_ = false;
		setImage(UnitImageKind::NORMAL);
		turnEndExtra(isOwnEnd);
		closeAttackedLogs_ = Direction::NONE_DIRECTION;
	}

	/**
	 * @fn
	 * �s���I��
	 */
	void Unit::endAction()
	{
		isActed_ = true;
		setImage(UnitImageKind::ACTED);
	}

	/**
	 * @fn
	 * �U��
	 * @param (targetRealX) �U���Ώۂ�X���W
	 * @param (targetRealY) �U���Ώۂ�Y���W
	 */
	void Unit::atack(int targetRealX, int targetRealY)
	{
		targetRealX_ = targetRealX;
		targetRealY_ = targetRealY;
		joinAnimationList(AnimationKind::ATACK);
	}
}