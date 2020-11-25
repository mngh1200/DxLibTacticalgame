#include "Mask.h"

namespace Entity {
	/**
	 * @fn
	 * �R���X�g���N�^
	 */
	Mask::Mask() : isOpen_(true), alpha_(MAX_ALPHA)
	{
		type_ = MASK;
		shape_ = Shape(0, 0, WIN_W, WIN_H);
	}

	/**
	 * @fn
	 * �R���X�g���N�^
	 */
	Mask::Mask(bool isOpen) : Mask()
	{
		isOpen_ = isOpen;
		if (isOpen)
		{
			alpha_ = MAX_ALPHA;
		}
		else
		{
			alpha_ = 0;
		}
	}

	/**
	 * @fn
	 * �A�j���[�V����
	 * @return �A�j���[�V�����I������true
	 */
	bool Mask::animationUpdate()
	{
		if (isOpen_) // ��ʕ\����
		{
			alpha_ -= ADD_ALPHA;
			if (alpha_ <= 0)
			{
				alpha_ = 0;
				animationId_ = -1;
				return true;
			}
		}
		else // ��ʏI����
		{
			alpha_ += ADD_ALPHA;
			if (alpha_ >= MAX_ALPHA)
			{
				alpha_ = MAX_ALPHA;
				animationId_ = -1;
				return true;
			}
		}

		return false;
	}

	/**
	 * @fn
	 * �`�揈��
	 */
	void Mask::render() const
	{
		DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_);

		DxLib::DrawBox(shape_.x, shape_.y, shape_.getX2(), shape_.getY2(), COLOR, TRUE);

		DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}