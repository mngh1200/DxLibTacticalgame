#include "Mask.h"

namespace Entity {
	/**
	 * @fn
	 * コンストラクタ
	 */
	Mask::Mask() : isOpen_(true), alpha_(MAX_ALPHA)
	{
		type_ = MASK;
		shape_ = Shape(0, 0, WIN_W, WIN_H);
	}

	/**
	 * @fn
	 * コンストラクタ
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
	 * アニメーション
	 * @return アニメーション終了時にtrue
	 */
	bool Mask::animationUpdate()
	{
		if (isOpen_) // 画面表示時
		{
			alpha_ -= ADD_ALPHA;
			if (alpha_ <= 0)
			{
				alpha_ = 0;
				animationId_ = -1;
				return true;
			}
		}
		else // 画面終了時
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
	 * 描画処理
	 */
	void Mask::render() const
	{
		DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_);

		DxLib::DrawBox(shape_.x, shape_.y, shape_.getX2(), shape_.getY2(), COLOR, TRUE);

		DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}