#include "Overlay.h"

namespace Entity {
	const int Overlay::COLOR = DxLib::GetColor(0, 0, 0); //! �I�[�o�[���C�̐F

	/**
	 * @fn
	 * �R���X�g���N�^
	 */
	Overlay::Overlay(bool isOpen) : isOpen_(isOpen), alpha_(MAX_ALPHA), animation_({})
	{
		type_ = OVERLAY;
		shape_ = Shape(0, 0, WIN_W, WIN_H);
	}

	/**
	 * @fn
	 * �A�j���[�V�����쐬(ObjectsControl::addAnimationObj���\�b�h��p�ŌĂяo��)
	 * @return �A�j���[�V�����쐬�\�ȏꍇtrue��Ԃ�
	 */
	bool Overlay::createAnimation(int animationId)
	{
		// ���Ɏ��s�ς݂̃A�j���[�V����������ꍇ�͍쐬�s��
		if (animationId == animationId_)
		{
			return false;
		}

		int direction = Animation::REVERSE;

		if (!isOpen_)
		{
			direction = Animation::NORMAL;
		}

		animation_ = Animation(ANIMATION_TIME, direction);
		return true;
	}

	/**
	 * @fn
	 * �A�j���[�V����
	 * @return �A�j���[�V�����I������true
	 */
	bool Overlay::animationUpdate()
	{
		if (animation_.update(&alpha_, 0, MAX_ALPHA))
		{
			return true;
		}
		return false;
	}

	/**
	 * @fn
	 * �`�揈��
	 */
	void Overlay::render() const
	{

 		DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_);

		DxLib::DrawBox(shape_.x, shape_.y, shape_.getX2(), shape_.getY2(), COLOR, TRUE);

		DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}