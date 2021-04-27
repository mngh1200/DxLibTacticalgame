#include "ExtraEffect.h"

namespace Entity {

	/**
	 * @fn
	 * ��������
	 * �_���[�W�G�t�F�N�g�𐶐�
	 * @param (atkUnit) �U�������j�b�g
	 * @param (defUnit) �h�䑤���j�b�g
	 * @param (text) �e�L�X�g
	 * @param (num) �A��
	 */
	void ExtraEffect::initExtraEffect(shared_ptr<Unit> atkUnit, shared_ptr<Unit> defUnit, const char* text, int num)
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		shape_.set(-W, BASE_Y + (H + MARGIN) * num);
		shape_.setSize(W, H);

		// �U�����܂��́A�h�䑤���j�b�g���\���ʒu�Əd�Ȃ�\��������ꍇ�A�ʒu����
		if (atkUnit->getX() < W || defUnit->getX() < W)
		{
			shape_.x = WIN_W;
			isLeft_ = false;
		}

		num_ = num;
		text_ = text;

		isPlayer_ = !atkUnit->isEnemy();

		joinAnimationList(AnimationKind::SHOW);
	}

	/**
	 * @fn
	 * ��\��
	 */
	void ExtraEffect::hide()
	{
		joinAnimationList(AnimationKind::HIDE);
	}

	/**
	 * @fn
	 * �`��
	 */
	void ExtraEffect::render() const
	{
		Utility::ResourceManager& rm = Utility::ResourceManager::getInstance();

		int colorType = ColorType::PLAYER_COLOR_LITE;
		if (!isPlayer_)
		{
			colorType = ColorType::ENEMY_COLOR_LITE;
		}

		DxLib::DrawBox(shape_.x, shape_.y, shape_.getX2(), shape_.getY2(), rm.getColor(ColorType::MAIN_COLOR), TRUE);
		DxLib::DrawBox(shape_.x, shape_.y, shape_.x + COLOR_W, shape_.getY2(), rm.getColor(colorType), TRUE);
		DxLib::DrawStringToHandle(shape_.x + PADDING_LEFT, shape_.y + PADDING_V, text_.c_str(), rm.getColor(ColorType::SUB_COLOR), rm.getHdlFont(FontType::NORMAL_S20));
	}

	/**
	 * @fn
	 * �A�j���[�V�����X�V
	 */
	bool ExtraEffect::animationUpdate()
	{
		if (waitTimer_.isWorking() && animationId_ == AnimationKind::SHOW) // �I���x���^�C�}�[�ғ����̏ꍇ
		{
			if (waitTimer_.checkAndCountTimer()) // �J�E���g�I���ŃA�j���[�V�����I��
			{
				return true;
			}

			if (animation_.getDelayFinishLog_()) // �A�j���[�V�����J�n�x���̏I����
			{
				Utility::ResourceManager::playSound(SoundKind::EXTRA_EFFECT); // ���ʉ�
			}
		}
		else if (animationId_ == AnimationKind::SHOW || animationId_ == AnimationKind::HIDE) // �\��
		{
			bool isFin = false;
			if (isLeft_)
			{
				isFin = animation_.update(&shape_.x, -W, 0);
			}
			else
			{
				isFin = animation_.update(&shape_.x, WIN_W, WIN_W - W);
			}

			if (isFin && animationId_ == AnimationKind::SHOW)
			{
				// �I���x���^�C�}�[
				waitTimer_.setTimer(EnemyTurnCont::getAnimationMs(SHOW_WAIT_MS));
				isFin = false;
			}
			else if (isFin && animationId_ == AnimationKind::HIDE)
			{
				destroy(); // ��\���A�j���[�V�����I�����A�v�f����
			}

			return isFin;
		}
		return false;
	}

	/**
	 * @fn
	 * �A�j���[�V�����쐬
	 * @param (animationId) �A�j���[�V�����̎��
	 */
	bool ExtraEffect::createAnimation(int animationId)
	{
		if (animationId == AnimationKind::SHOW)
		{			
			animation_ = Animation(EnemyTurnCont::getAnimationMs(ANIMATION_TIME), 0, 1, EnemyTurnCont::getAnimationMs(NUM_DELAY * num_));
			return true;
		}
		else if (animationId == AnimationKind::HIDE)
		{
			animation_ = Animation(EnemyTurnCont::getAnimationMs(ANIMATION_TIME), Animation::Direction::REVERSE);
			return true;
		}
		return false;
	}
}